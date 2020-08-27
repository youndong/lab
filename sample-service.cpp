#include <iostream>
#include <future>

#include <signal.h>
#include <unistd.h>

#include "sample-adaptor.h"

using namespace std;

class SampleService :	
	public com::example::SampleInterface_adaptor,
	public DBus::IntrospectableAdaptor,
	public DBus::PropertiesAdaptor, // for detecting the property changed
	public DBus::ObjectAdaptor 
{
public:
	SampleService(DBus::Connection* conn);
	virtual ~SampleService();

public:
	virtual void Frobate(const int32_t& foo, std::string& bar, std::map< uint32_t, std::string >& baz);
    virtual ::DBus::Variant Bazify(const ::DBus::Struct< int32_t, int32_t, uint32_t >& bar);
    virtual void Mogrify(const ::DBus::Struct< int32_t, int32_t, std::vector< ::DBus::Variant > >& bar);

	void on_set_property(DBus::InterfaceAdaptor &interface, const std::string &property, const DBus::Variant &value);
};

SampleService::SampleService(DBus::Connection* conn) 
	: DBus::ObjectAdaptor (*conn, "/com/example/sample_service") {

}

SampleService::~SampleService() {

}

void SampleService::Frobate(const int32_t& foo /*in*/, std::string& bar/*out*/, std::map< uint32_t, std::string >& baz/*out*/) {

    baz.insert(make_pair(0, "Hi~"));
    baz.insert(make_pair(1, "Bye~"));
    baz.insert(make_pair(2, "Thanks~"));

	if (foo >= 0 && foo <=2)
		bar = bar[foo];
	else
		bar = "Invalid input number. Please contact .. youndong.park_at_gmail_dot_com";

}

::DBus::Variant SampleService::Bazify(const ::DBus::Struct< int32_t, int32_t, uint32_t >& bar) {
	::DBus::Variant retVar;

	/*
	cout << "Called Bazify()" << endl;
	cout << "\tfirst (int32_t) 	: " << (int32_t)bar._1 << endl;
	cout << "\tseond (int32_t)	: " << (int32_t)bar._2 << endl;
	cout << "\tthird (uint32_t)	: " << (uint32_t)bar._3 << endl;
	*/

	::DBus::MessageIter m = retVar.writer();
 	m.append_string("Hi, I'm Bazify from SampleService ..");

	return retVar;
}
void SampleService::Mogrify(const ::DBus::Struct< int32_t, int32_t, std::vector< ::DBus::Variant > >& bar) {

	/*
	cout << "Called Mogrify()" << endl;
	cout << "\tfirst (int32_t) 			: " << (int32_t)bar._1 << endl;
	cout << "\tseond (int32_t)			: " << (int32_t)bar._2 << endl;
	cout << "\tthird (vector of variant): " << endl;
	
	for(auto iter = bar._3.begin(); iter != bar._3.end(); iter++) {
	    ::DBus::MessageIter m = (*iter).reader();
		cout << "\t\tvariant	: " << m.get_string() << endl;
	}
	*/
}
 
void SampleService::on_set_property
	(DBus::InterfaceAdaptor &interface, const std::string &property, const DBus::Variant &value)
{
	if (property == "Bar")
	{
		/*
		uint8_t _value = value;
		cout << "property value is changed : " << _value << endl;
		*/

		Changed(true);
	}
}

 //////////////////////////////


DBus::BusDispatcher	dispatcher;
SampleService*	service = NULL;

void niam(int sig)
{
	dispatcher.leave();
	pthread_exit(NULL);
}

int main()
{
	signal(SIGTERM, niam);
	signal(SIGINT, niam);

	DBus::default_dispatcher = &dispatcher;
	DBus::_init_threading();
	DBus::Connection conn = DBus::Connection::SessionBus();
	conn.request_name("com.example.SampleInterface");
 
	SampleService service(&conn);

	uint8_t count = 0;
						 
	std::future<void>fut = std::async(std::launch::async, [=] {
		dispatcher.enter();
	});
								 
	// emit Changed every seconds. 
	// : false -> true -> false -> true -> ...
	bool 	changedValue = false;
	while ( true ) {
		//printf("emit signal 'Changed' count : %d\n", count);
		//service.Changed(changedValue=!changedValue);
		//service.Changed(false);
		sleep(1);
	}

	return 0;
}

// compile
// g++ -I/usr/include/dbus-c++-1/ sample-service.cpp -ldbus-c++-1 -lpthread -o service
