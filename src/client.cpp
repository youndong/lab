#include <iostream>
#include <future>

#include <signal.h>
#include <unistd.h>

#include "service_proxy.h"
#include "flatbuffers/flatbuffers.h"
#include "payload_generated.h"

using namespace std;
using namespace Sample::Service;

class SampleClient :	
	public com::example::SampleInterface_proxy,
	public DBus::IntrospectableProxy,
	public DBus::ObjectProxy
{
public:
	SampleClient(DBus::Connection& conn, const char* path, const char* name);
	void Changed(const bool& new_value);
	void PropertyChanged(const std::string& arg1, const ::DBus::Variant& arg2); 

};

SampleClient::SampleClient(DBus::Connection& conn, const char* path, const char* name)
	: DBus::ObjectProxy(conn, path, name)
{
}

void SampleClient::Changed(const bool& new_value) 
{
	// Do something
	cout << "Service's property \"Bar\" is changed to " << Bar() << endl;
}

void SampleClient::PropertyChanged(const std::string& arg1, const ::DBus::Variant& arg2) 
{
	// Do something
	cout << "SampleClient::PropertyChanged is called : " << endl;
}


//////////////////////////////////////////////


DBus::BusDispatcher	dispatcher;
SampleClient*	client = NULL;

void* methods_test_thread(void* arg) 
{

    ///////////////////////////////////////////////////////////////////////////////////////
    // test fbs 
    // to-be: prepare to use flatbuffers
    {
        // const std::vector< uint8_t >& payload)
        /*
        {
            uint8_t src[] = { 10, 20, 30, 40, 50 };
            std::vector<uint8_t> fbs_Args(std::begin(src), std::end(src));
            
            std::vector< uint8_t > ret1 = client->fbs(fbs_Args);
            cout << "Server message is arrived : " << ret1.size() << endl;
            cout << unsigned(ret1[0]) << endl;
        }
        */


        std::async([]{
            
            flatbuffers::FlatBufferBuilder builder;
            const uint8_t* payload = nullptr;
            
            {
                Sample::Service::Status status = Sample::Service::Status_NO_ERROR;
                auto data = builder.CreateString("0123456789");
                builder.Finish(CreatePayload(builder, status, data));

                payload = builder.GetBufferPointer();

                std::vector<uint8_t> fbs_Args(payload, payload+builder.GetSize());
                std::vector< uint8_t > ret1 = client->fbs(fbs_Args);
            
                const Result* r = flatbuffers::GetRoot<Sample::Service::Result>(&ret1[0]);

                std::cout << r->status() << std::endl;
                std::cout << GetString(r->data()) << std::endl;          
            }
            

        });
        

        //::DBus::MessageIter m = ret1.reader();
        
    }


    ///////////////////////////////////////////////////////////////////////////////////////
    // test Method_A
    {
        // void Method_A(const int32_t& foo, std::string& bar, std::map< uint32_t, std::string >& baz)
        std::map< uint32_t, std::string > _mapArg;

        std::string text;

        client->Method_A(12345, text, _mapArg);

        cout << text << endl;
        for(auto iter = _mapArg.begin(); iter != _mapArg.end(); iter++) {
            cout << "\tkey 		: " << iter->first << endl;
            cout << "\tvalue 	: " << iter->second << endl;
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////
    // test Method_B 
    {
        // ::DBus::Variant Method_B(const ::DBus::Struct< int32_t, int32_t, uint32_t >& bar)
        typedef ::DBus::Struct< int32_t, int32_t, uint32_t > _t_Method_B;
        _t_Method_B mb_Args;
        mb_Args._1   = 0;
        mb_Args._2   = 1;
        mb_Args._3   = 2;

        ::DBus::Variant ret1 = client->Method_B(mb_Args);
        ::DBus::MessageIter m = ret1.reader();
        
        cout << "Server message is arrived : " << m.get_string() << endl;
    }

    ///////////////////////////////////////////////////////////////////////////////////////
    // test Method_C
    {
        // void Method_C(const ::DBus::Struct< int32_t, int32_t, std::vector< ::DBus::Variant > >& bar)
        typedef ::DBus::Struct< int32_t, int32_t, std::vector< ::DBus::Variant > > _t_mc;
        _t_mc mc_Args;

        std::vector< ::DBus::Variant > vecVar;
        
        mc_Args._1 = 2020;
        mc_Args._2 = 702;

        for (int index=0; index < 5; index ++) {
            ::DBus::Variant var;
            ::DBus::MessageIter m = var.writer();
            
            std::string t = "client is sending messages (variant type with string) to service : " + std::to_string(index);
            m.append_string(t.c_str());

            vecVar.push_back(var);
        }

        mc_Args._3 = vecVar;

        client->Method_C(mc_Args);
    }

    ///////////////////////////////////////////////////////////////////////////////////////
    // test Bar (set property)
    {
        client->Bar((uint8_t)'H');
        sleep(3);
        client->Bar((uint8_t)'e');
        sleep(3);
        client->Bar((uint8_t)'l');
        sleep(3);
        client->Bar((uint8_t)'l');
        sleep(3);
        client->Bar((uint8_t)'o');
        sleep(3);
        client->Bar((uint8_t)'!');
        sleep(3);
    }
    
    dispatcher.leave();

	return NULL;
}

void niam(int sig)
{
	dispatcher.leave();
}

int main()
{
	signal(SIGTERM, niam);
	signal(SIGINT, niam);

	DBus::default_dispatcher	= &dispatcher;

	DBus::_init_threading();
	new DBus::DefaultTimeout(100, false, &dispatcher);

	pthread_t	th[1];

	DBus::Connection conn = DBus::Connection::SessionBus();

	SampleClient _client(conn, 
			/*ECHO_SERVER_PATH*/ "/com/example/sample_service", 
			/*ECHO_SERVER_IF_NAME*/ "com.example.SampleInterface");

    client = &_client;	

    std::future<void>   fut[3];

    fut[0] = std::async(std::launch::async, [=] {
		methods_test_thread(NULL);
	});


    fut[1] = std::async(std::launch::async, [=] {
		dispatcher.enter();
	});

}

// g++ -I/usr/include/dbus-c++-1/ client.cpp -ldbus-c++-1 -lpthread -o client
