// -*- mode: c++ -*-
//
// $Id$

#ifndef EVENTQUEUE_H
#define EVENTQUEUE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_SIGNAL_H
#include <signal.h>
#endif // HAVE_SIGNAL_H

#ifdef HAVE_LIST
#include <list>
#endif

#ifdef HAVE_QUEUE
#include <queue>
#endif

#include "event.h"
#include "eventconnector.h"
#include "rectangle.h"

class EventQueue {
    private:
#if defined(SIGWINCH) || defined(SIGALRM)
	// Used by blocksignal()/unblocksignal()
	static sigset_t block_sigmask;
	// Used by blocksignal()/unblocksignal()
	static sigset_t tmp_old_sigmask;
	// Used by setupSignal()/restoreSignal()
	static sigset_t old_sigmask;

	static struct sigaction old_winch_act;
	static struct sigaction old_alrm_act;
#endif // defined(SIGWINCH) || defined(SIGALRM)
	static bool signal_blocked;
	static std::queue<EventBase*> evt_queue;
	static std::list<EventConnectorBase*> evtconn_list;

	static void setupSignal();
	static void restoreSignal();

#ifdef SA_SIGINFO
	static void signal_handler(int signo, siginfo_t* info, void *d);
#else
	static void signal_handler(int signo);
#endif // SA_SIGINFO

	static void blocksignal();
	static void unblocksignal();

    public:
	static void registerHandler(const EventConnectorBase& ec);
	static void unregisterHandler(const EventConnectorBase& ec);
	static void inject(const EventBase& ev);
	static void run();
};

#endif // EVENTQUEUE_H
