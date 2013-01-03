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
	std::list<EventConnector<EventBase>*> evtconn_list;
	std::stack<EventConnector<EventBase>*> evtkeyconn_list;
};

#endif // EVENTQUEUE_H
