#include <iostream>
#include <future>

#include <signal.h>
#include <unistd.h>

#include "sample-interface.h"

using namespace std;

class SampleClient :	
	public com::example::SampleInterface_proxy,
	public DBus::IntrospectableProxy,
	public DBus::ObjectProxy
{
public:
	SampleClient(DBus::Connection& conn, const char* path, const char* name);
	void Changed(const bool& new_value);

};

SampleClient::SampleClient(DBus::Connection& conn, const char* path, const char* name)
	: DBus::ObjectProxy(conn, path, name)
{
}

void SampleClient::Changed(const bool& new_value) 
{
	// Do something
	// cout << "Service's property \"Bar\" is changed to " << Bar() << endl;
}

/////////////////////

//////////////////////////////////////////////


DBus::BusDispatcher	dispatcher;
SampleClient*	client = NULL;

void* methods_test_thread(void* arg) 
{
    ///////////////////////////////////////////////////////////////////////////////////////
    // test Bazify
    {
        // ::DBus::Variant Bazify(const ::DBus::Struct< int32_t, int32_t, uint32_t >& bar)
        typedef ::DBus::Struct< int32_t, int32_t, uint32_t > _t_Bazify;
        _t_Bazify bazify_Args;
        bazify_Args._1   = 0;
        bazify_Args._2   = 1;
        bazify_Args._3   = 2;

        ::DBus::Variant ret1 = client->Bazify(bazify_Args);
        ::DBus::MessageIter m = ret1.reader();
        
        cout << "Server message is arrived : " << m.get_string() << endl;
    }

    ///////////////////////////////////////////////////////////////////////////////////////
    // test Frobate
    {
        // void Frobate(const int32_t& foo, std::string& bar, std::map< uint32_t, std::string >& baz)
        std::map< uint32_t, std::string > _mapArg;

        std::string text;

        client->Frobate(12345, text, _mapArg);

        cout << text << endl;
        for(auto iter = _mapArg.begin(); iter != _mapArg.end(); iter++) {
            cout << "\tkey 		: " << iter->first << endl;
            cout << "\tvalue 	: " << iter->second << endl;
        }
    }

    ///////////////////////////////////////////////////////////////////////////////////////
    // test Mogrify
    {
        // void Mogrify(const ::DBus::Struct< int32_t, int32_t, std::vector< ::DBus::Variant > >& bar)
        typedef ::DBus::Struct< int32_t, int32_t, std::vector< ::DBus::Variant > > _t_mogrify;
        _t_mogrify mogrify_Args;

        std::vector< ::DBus::Variant > vecVar;
        
        mogrify_Args._1 = 2020;
        mogrify_Args._2 = 702;

        for (int index=0; index < 5; index ++) {
            ::DBus::Variant var;
            ::DBus::MessageIter m = var.writer();
            
            std::string t = "client is sending messages (variant type with string) to service : " + std::to_string(index);
            m.append_string(t.c_str());

            vecVar.push_back(var);
        }

        mogrify_Args._3 = vecVar;

        client->Mogrify(mogrify_Args);
    }

    ///////////////////////////////////////////////////////////////////////////////////////
    // test Bar (set property)
    {
        client->Bar((uint8_t)'H');
        sleep(1);
        client->Bar((uint8_t)'e');
        sleep(1);
        client->Bar((uint8_t)'l');
        sleep(1);
        client->Bar((uint8_t)'l');
        sleep(1);
        client->Bar((uint8_t)'o');
        sleep(1);
        client->Bar((uint8_t)'!');
        sleep(1);
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
