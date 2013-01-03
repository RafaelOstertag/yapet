// -*- mode: c++ -*-
//
// $Id$

#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_LIST
#include <list>
#endif

#ifdef HAVE_STACK
#include <stack>
#endif

#ifdef HAVE_QUEUE
#include <queue>
#endif

#include "event.h"
#include "eventconnector.h"

class EventQueue {
    private:
	static std::queue<EventBase*> evt_queue;
	static std::list<EventConnectorBase*> evtconn_list;
	static std::stack<EventConnectorBase*> evtkeyconn_list;

    public:
	static void registerHandler(const EventConnectorBase& ec);
	static void unregisterHandler(const EventConnectorBase& ec);
	static void injectEvent(const EventBase& ev);
};

#endif // EVENTQUEUE_H
