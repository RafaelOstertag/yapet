// $Id$
//
// Test basic functionality of EventQueue by injecting events using EventQueue::inject()
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

class Handler {
    private:
	EVENT_TYPE expected_evt;
	int calls;
    public:
	inline Handler(EVENT_TYPE expected):
	    expected_evt(expected), calls(0) {}
	inline virtual int handler(EventBase& e) {
	    if (e.type() != expected_evt) std::abort();
	    calls++;
	    return 0;
	}
	inline int getCalls() { return calls; }
};

class WinChHandler: public Handler {
    public:
	inline WinChHandler(): Handler(EVT_WINCH) {}
	inline int handler(EventBase& e) {
	    Handler::handler(e);
	    Rectangle<> tmp(dynamic_cast<EventWinCh&>(e).data());
	    return 0;
	}
};

class WinAlrmHandler: public Handler {
    public:
	inline WinAlrmHandler(): Handler(EVT_ALARM) {}
	inline int handler(EventBase& e) { return Handler::handler(e); }
};

int main() {
    try {
	WinChHandler whandler;
	WinAlrmHandler ahandler;

	EventQueue::registerHandler(EventConnectorMethod1<WinChHandler>(EVT_WINCH, &whandler,&WinChHandler::handler) );
	EventQueue::registerHandler(EventConnectorMethod1<WinAlrmHandler>(EVT_ALARM, &ahandler,&WinAlrmHandler::handler) );

	EventQueue::inject(EventWinCh(Rectangle<> ()));
	EventQueue::inject(EventBase(EVT_ALARM));
	EventQueue::inject(EventBase(EVT_QUIT));

	Curses::init();
	// EventQueue:run() blocks in getch().
	ungetch('a');

	EventQueue::run();

	if (whandler.getCalls() != 1)
	    goto _ERR;
	if (ahandler.getCalls() != 1)
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
