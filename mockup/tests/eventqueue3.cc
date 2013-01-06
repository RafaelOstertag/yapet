// $Id$
//
// Test several handler for event
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif // HAVE_UNISTD_H

#ifdef HAVE_IOSTREAM
#include <iostream>
#endif // HAVE_IOSTREAM

#ifdef HAVE_CSTDLIB
#include <cstdlib>
#endif // HAVE_CSTDLIB

#include "curs.h"
#include "eventconnector.h"
#include "eventqueue.h"

class AlrmHandler {
    private:
	EVENT_TYPE expected_evt;
	int calls;

    public:
	inline AlrmHandler(): expected_evt(EVT_ALARM), calls(0) {}
	inline int handler(EventBase& e) {
	    if (e.type() != expected_evt) std::abort();
	    calls++;
	    if (getCalls() < 4) {
		std::cout << "alarm(1)" << std::endl;
		alarm(1);
		return 0;
	    }
	    std::cout << "QUIT" << std::endl;
	    EventQueue::inject(EventBase(EVT_QUIT));
	    return 0;
	}
	inline int getCalls() { return calls; }
};

class AlrmHandler2 {
    private:
	EVENT_TYPE expected_evt;
	int calls;

    public:
	inline AlrmHandler2(): expected_evt(EVT_ALARM), calls(0) {}
	inline int handler(EventBase& e) {
	    if (e.type() != expected_evt) std::abort();
	    calls++;
	    return 0;
	}
	inline int getCalls() { return calls; }
};


int main() {

    try {
	AlrmHandler ahandler;
	AlrmHandler2 ahandler2_1;
	AlrmHandler2 ahandler2_2;
	AlrmHandler2 ahandler2_3;
	AlrmHandler2 ahandler2_4;

	EventQueue::registerHandler(EventConnectorMethod1<AlrmHandler>(EVT_ALARM, &ahandler,&AlrmHandler::handler) );
	EventQueue::registerHandler(EventConnectorMethod1<AlrmHandler2>(EVT_ALARM, &ahandler2_1,&AlrmHandler2::handler) );
	EventQueue::registerHandler(EventConnectorMethod1<AlrmHandler2>(EVT_ALARM, &ahandler2_2,&AlrmHandler2::handler) );
	EventQueue::registerHandler(EventConnectorMethod1<AlrmHandler2>(EVT_ALARM, &ahandler2_3,&AlrmHandler2::handler) );
	EventQueue::registerHandler(EventConnectorMethod1<AlrmHandler2>(EVT_ALARM, &ahandler2_4,&AlrmHandler2::handler) );

	Curses::init();
	alarm(4);
	EventQueue::run();

	if (ahandler.getCalls() != 4)
	    goto _ERR;
	if (ahandler2_1.getCalls() != 4)
	    goto _ERR;
	if (ahandler2_2.getCalls() != 4)
	    goto _ERR;
	if (ahandler2_3.getCalls() != 4)
	    goto _ERR;
	if (ahandler2_4.getCalls() != 4)
	    goto _ERR;
	Curses::end();
    } catch (std::exception& e) {
	std::cerr << e.what() << std::endl;
	sleep(2);
	goto _ERR;
    }

    return 0;
 _ERR:
    Curses::end();
    return 1;
}
