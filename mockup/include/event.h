// -*- mode: c++ -*-
//
// $Id$

#ifndef EVENT_H
#define EVENT_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "rectangle.h"

enum EVT_TYPE {
    EVT_ABSTRACT,
    EVT_WINCH,
    EVT_ALARM,
    EVT_KEY
};

class EventBase;

class EventHandler {
    public:
	virtual int eventhandler(const EventBase&) = 0;
};

class EventBase {
    private:
	EVT_TYPE __type;

    public:
	inline EventBase(EVT_TYPE _t): __type(_t) {}
	inline EventBase(const EventBase& _eb) { __type = _eb.__type; }
	inline EventBase& operator=(const EventBase& _eb) { 
	    __type = _eb.__type;
	    return *this;
	}
	inline operator==(const EventBase& _eb) { return __type == _eb.__type; }
	    
	inline virtual ~EventBase() {};
};

template<class T /* payload type */>
class Event : public EventBase {
    private:
	T* __payload;

    public:
	Event(EVT_TYPE _t): EventBase(_t), __payload(NULL) {}
	Event(EVT_TYPE _t, const T& _p): EventBase(_t) {
	    __payload = new T;	    
	    *__payload = _p;
	}
	Event(const Event<T>& _e): EventBase(_e) {
	    if (_e.__payload) {
		__payload = new T;
		*__payload = *(_e.__payload);
	    } else {
		__payload = NULL;
	    }
	}
	~Event() {
	    if (__payload)
		delete __payload;
	}

	Event<T>& operator=(const Event<T>& _e) {
	    EventBase::operator(_e);

	    if (__payload)
		delete __payload;

	    if (_e.__payload) {
		__payload = new T;
		*__payload = *(_e.__payload);
	    } else {
		__payload = NULL;
	    }

	    return *this;
	}

	T payload() const { return *__payload; }
	EVT_TYPE type() const { return __type; }
    
};


template<>
class EventWinCh: public Event<Rectangle<> > {
    public:
	EventWinCh(const Rectangle<>& _r) : Event<Rectangle<> >(EVT_WINCH, _r) {}
	EventWinCh(const EventWinCh& _e): Event<Rectangle<> >(_e) {}
	EventWinCh& operator=(const EventWinCh& _e) { 
	    Event<Rectangle<> >::operator=(_e);
	    return *this;
	}
};

template<>
class EventAlarm: public Event<int /* ignored */> {
    public:
	EventAlarm() : Event<int>(EVT_ALARM) {}
	EventAlarm(const EventAlarm& _e): Event<int>(_e) {}
	EventAlarm& operator=(const EventAlarm& _e) { 
	    Event<int>::operator=(_e);
	    return *this;
	}
};

template<>
class EventKey: public Event<int> {
    public:
	EventKey() : Event<int>(EVT_ALARM) {}
	EventKey(const EventKey& _e): Event<int>(_e) {}
	EventKey& operator=(const EventKey& _e) { 
	    Event<int>::operator=(_e);
	    return *this;
	}
    
};


#endif // EVENT_H
