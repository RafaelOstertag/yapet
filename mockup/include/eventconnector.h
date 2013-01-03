// -*- mode: c++ -*-
//
// $Id$

#ifndef EVENTCONNECTOR_H
#define EVENTCONNECTOR_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "event.h"

template<class T /* event type */>
class EventConnector {
    private:
	T& evt;
	EventHandler& evthandler;

    public:
	inline EventConnector(T& _e, EventHandler& _h):
	    evt(_e), evthandler(_h) {}
	inline EventConnector<T>(const EventConnector<T>& _ec) {
	    evt = _ec.evt;
	    evthandler = _ec.evthandler;
	}
	inline EventConnector<T>& operator=(const EventConnector<T>& _ec) {
	    evt = _ec.evt;
	    evthandler = _ec.evthandler;
	    return *this;
	}

	inline int call(const T& _evt) { return eventhandler.eventhandler(_evt); }

};

#endif // EVENTCONNECTOR_H
