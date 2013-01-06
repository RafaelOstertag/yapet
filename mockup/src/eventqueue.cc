// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_CERRNO
#include <cerrno>
#endif // HAVE_CERRNO_H

#ifdef HAVE_CSTDLIB
#include <cstdlib>
#endif

#ifdef HAVE_IOSTREAM
#include <iostream>
#endif

#ifdef HAVE_ALGORITHM
#include <algorithm>
#endif // HAVE_ALGORITHM

#include "curex.h"
#include "eventqueue.h"
#include "curs.h"

#if defined(SIGWINCH) || defined(SIGALRM)
sigset_t EventQueue::block_sigmask;
sigset_t EventQueue::tmp_old_sigmask;
sigset_t EventQueue::old_sigmask;
struct sigaction EventQueue::old_winch_act;
struct sigaction EventQueue::old_alrm_act;
#endif // defined(SIGWINCH) || defined(SIGALRM)
bool EventQueue::signal_blocked = false;

std::queue<EventBase*> EventQueue::evt_queue;
std::list<EventConnectorBase*> EventQueue::evtconn_list;

//
// Functors
//

class EventConnectorEqual {
    private:
	const EventConnectorBase& __eb;
    public:
	inline EventConnectorEqual(const EventConnectorBase& _eb) : __eb(_eb) {}
	inline bool operator()(EventConnectorBase* eb) {
	    return *eb == __eb;
	}
};

class DestroyEventConnector {
    public:
	inline void operator()(EventConnectorBase* eb) {
	    delete eb;
	}
};

class CallEventConnector {
    private:
	EventBase& __eb;
	bool key_processed;

    public:
	inline CallEventConnector(EventBase& _eb) :
	    __eb(_eb), key_processed(false) {}
	inline void operator()(EventConnectorBase* _ec) {
	    if (_ec->type() == __eb.type()) {
		// A key event will only be sent to the first connector
		if (__eb.type() == EVT_KEY && key_processed) return;

		_ec->call(__eb);

		if (__eb.type() == EVT_KEY) key_processed = true;
	    }
	}
};

//
// Private
//
void
EventQueue::setupSignal() {
    int err;

    //
    // Window Size Change handler
    //
#ifdef SIGWINCH
    struct sigaction winchact;

    winchact.sa_sigaction = signal_handler;
#ifdef SA_SIGINFO
    winchact.sa_flags = SA_SIGINFO;
#else // SA_SIGINFO
    winchact.sa_flags = 0;
#endif // SA_SIGINFO

    sigemptyset(&winchact.sa_mask);

    err = sigaction(SIGWINCH, &winchact, &old_winch_act);
    if (err)
	throw SystemError(errno);
#endif // SIGWINCH

    //
    // Alarm handler
    //
#ifdef SIGALRM
    struct sigaction alrmact;

    alrmact.sa_sigaction = signal_handler;
#ifdef SA_SIGINFO
    alrmact.sa_flags = SA_SIGINFO;
#else // SA_SIGINFO
    alrmact.sa_flags = 0;
#endif // SA_SIGINFO

    sigemptyset(&alrmact.sa_mask);

    err = sigaction(SIGALRM, &alrmact, &old_alrm_act);
    if (err)
	throw SystemError(errno);
#endif // SIGALRM

    sigset_t nset;
    sigemptyset(&nset);

#ifdef SIGWINCH
    sigaddset(&nset, SIGWINCH);
#endif // SIGWINCH
#ifdef SIGALRM
    sigaddset(&nset, SIGALRM);
#endif // SIGALRM

#if defined(SIGWINCH) || defined(SIGALRM)
    err = sigprocmask(SIG_UNBLOCK, &nset, &old_sigmask);
    if (err)
	throw SystemError(errno);
#endif // defined(SIGWINCH) || defined(SIGALRM)

}

void
EventQueue::restoreSignal() {
    int err;

#ifdef SIGWINCH
    err = sigaction(SIGWINCH, &old_winch_act, NULL);
    if (err)
	throw SystemError(errno);
#endif // SIGWINCH
#ifdef SIGALRM
    err = sigaction(SIGWINCH, &old_alrm_act, NULL);
    if (err)
	throw SystemError(errno);
#endif // SIGALRM

#if defined(SIGWINCH) || defined(SIGALRM)
    err = sigprocmask(SIG_SETMASK, &old_sigmask, NULL);
    if (err)
	throw SystemError(errno);
#endif // defined(SIGWINCH) || defined(SIGALRM)
}

void
#ifdef SA_SIGINFO
EventQueue::signal_handler(int signo, siginfo_t* info, void *d)
#else
EventQueue::signal_handler(int signo)
#endif // SA_SIGINFO
{
    int olderrno = errno;

    switch (signo) {
#ifdef SIGALRM
    case SIGALRM:
	try {
	    evt_queue.push(new EventBase(EVT_ALARM));
	} catch(std::exception& e) {
	    // Intentionally empty
#ifndef NDEBUG
	    std::cerr << e.what() << std::endl;
	    std::abort();
#endif
	}
	break;
#endif // SIGALRM
#ifdef SIGWINCH
    case SIGWINCH:
	try {
	    evt_queue.push(new EventWinCh(Curses::inquiryScreenSize()));
	} catch(std::exception& e) {
	    // Intentionally empty
#ifndef NDEBUG
	    std::cerr << e.what() << std::endl;
	    std::abort();
#endif
	}
	break;
#endif
    }

    errno = olderrno;
}

void
EventQueue::blocksignal() {
#if defined(SIGWINCH) || defined(SIGALRM)
    if (signal_blocked) return;

    int err = sigprocmask(SIG_BLOCK, &block_sigmask, &tmp_old_sigmask);
    if (err)
	throw SystemError(errno);
    signal_blocked = true;
#endif // defined(SIGWINCH) || defined(SIGALRM)
}

void
EventQueue::unblocksignal() {
#if defined(SIGWINCH) || defined(SIGALRM)
    if (!signal_blocked) return;
    signal_blocked = false;

    int err = sigprocmask(SIG_SETMASK, &tmp_old_sigmask, NULL);
    if (err)
	throw SystemError(errno);
#endif // defined(SIGWINCH) || defined(SIGALRM)
}

//
// Public
//
void
EventQueue::registerHandler(const EventConnectorBase& ec) {
    evtconn_list.push_front(ec.clone());
}

void
EventQueue::unregisterHandler(const EventConnectorBase& ec) {
    std::list<EventConnectorBase*>::iterator it = 
	std::find_if(evtconn_list.begin(),
		  evtconn_list.end(),
		  EventConnectorEqual(ec));

    if (it == evtconn_list.end()) return;

    delete *it;
    evtconn_list.erase(it);
    
#if 0
    // get the elements to be erased
    std::list<EventConnectorBase*>::iterator it = 
	std::remove_if(evtconn_list.begin(),
		       evtconn_list.end(),
		       EventConnectorEqual(ec));

    // free the space occuppied
    std::for_each(it,
		  evtconn_list.end(),
		  DestroyEventConnector());

    // get rid of the element
    evtconn_list.erase(it, evtconn_list.end());
#endif
}

void
EventQueue::inject(const EventBase& ev) {
    blocksignal();
    try {
	evt_queue.push(ev.clone());
    } catch(std::exception& e) {
	// Intentionally empty
#ifndef NDEBUG
	    std::cerr << e.what() << std::endl;
	    std::abort();
#endif
    }
    unblocksignal();
}

void
EventQueue::run() {
    sigemptyset(&block_sigmask);
#ifdef SIGWINCH
    sigaddset(&block_sigmask, SIGWINCH);
#endif // SIGWINCH
#ifdef SIGALRM
    sigaddset(&block_sigmask, SIGALRM);
#endif // SIGALRM

    setupSignal();

    while(true) {
	int c=getch();
	if (c != ERR)
	    inject(EventKey(c));

	blocksignal();
	while(!evt_queue.empty()) {

	    EventBase* evt = evt_queue.front();
	    
	    if (evt->type() == EVT_QUIT) {
		unblocksignal();
		goto QUIT;
	    }
	    
	    std::for_each(evtconn_list.begin(),
			  evtconn_list.end(),
			  CallEventConnector(*evt));
	    delete evt;
	    evt_queue.pop();
	}
	unblocksignal();

    }
QUIT:
    blocksignal();
    /* Cleanup event queue */
    while(!evt_queue.empty()) {
	delete evt_queue.front();
	evt_queue.pop();
    }
    unblocksignal();
    restoreSignal();

    std::list<EventConnectorBase*>::iterator it=evtconn_list.begin();
    while(it != evtconn_list.end())
	delete *it++;

    evtconn_list.clear();
}
