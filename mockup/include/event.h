// -*- mode: c++ -*-
//
// $Id$

#ifndef EVENT_H
#define EVENT_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "rectangle.h"

enum EVENT_TYPE {
    EVT_QUIT,
    EVT_WINCH,
    EVT_ALARM,
    EVT_KEY
};

class EventBase {
    private:
	EVENT_TYPE event_type;
    
    public:
	inline EventBase(EVENT_TYPE _et): event_type(_et) {}
	inline EventBase(const EventBase& _e) {
	    event_type = _e.event_type;
	}
	inline EventBase& operator=(const EventBase& _e) {
	    event_type = _e.event_type;
	    return *this;
	}
	inline virtual ~EventBase() {}
	inline virtual bool operator==(const EventBase& _e) const {
	    return event_type == _e.event_type;
	}
	inline EVENT_TYPE type() const { return event_type; }
	virtual inline EventBase* clone() const {
	    return new EventBase(*this);
	}
};

template<class T>
class Event: public EventBase {
    private:
	T payload;

    public:
	Event(EVENT_TYPE _et, const T& _v): EventBase(_et) {
	    payload = _v;
	}
	Event(const Event<T>& _e): EventBase(_e) {
	    payload = _e.payload;
	}
	Event<T>& operator=(const Event<T>& _e) {
	    EventBase::operator=(_e);
	    payload = _e.payload;
	    return *this;
	}
	Event<T>* clone() const {
	    return new Event<T>(*this);
	}
	virtual T& data() { return payload; }
};

class EventWinCh: public Event<Rectangle<> > {
    public:
	inline EventWinCh(const Rectangle<>& _r): Event<Rectangle<> >(EVT_WINCH, _r) {}
	inline EventWinCh(const EventWinCh& _e): Event<Rectangle<> >(_e) {}
	inline EventWinCh& operator=(const EventWinCh& _e) {
	    Event<Rectangle<> >::operator=(_e);
	    return *this;
	}
	inline EventWinCh* clone() const {
	    return new EventWinCh(*this);
	}
};

class EventKey: public Event<int> {
    public:
	EventKey(const int& _r): Event<int>(EVT_WINCH, _r) {}
	EventKey(const EventKey& _e): Event<int>(_e) {}
	EventKey& operator=(const EventKey& _e) {
	    Event<int>::operator=(_e);
	    return *this;
	}
	inline EventKey* clone() const {
	    return new EventKey(*this);
	}
};

#endif // EVENT_H
