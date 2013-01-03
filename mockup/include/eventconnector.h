// -*- mode: c++ -*-
//
// $Id$

#ifndef EVENTCONNECTOR_H
#define EVENTCONNECTOR_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "event.h"

class EventConnectorBase {
    private:
	EVT_TYPE evt;

    public:
	inline EventConnectorBase(EVT_TYPE _e): evt(_e) {}
	inline EventConnectorBase(const EventConnectorBase& _ec) {
	    evt = _ec.evt;
	}
	inline EventConnectorBase& operator=(const EventConnectorBase& _ec) {
	    evt = _ec.evt;
	    return *this;
	}
	inline EVT_TYPE type() { return evt; }
	inline virtual int call() = 0;
};

template<class T, /* Type of object called */
	 class D /* data type passed to eventhandler */>
class EventConnector1: public EventConnectorBase {
    public:
	typedef int (T::*_mptr_t)(D&);
    private:
	int (T::*m_ptr)(D&);
	//	_mptr_t m_ptr;
	T* obj_ptr;
	D data;

    private:
	EventConnector1(EVT_TYPE _e,
		       T& obj,
		       _mptr_t _p): EventConnectorBase(_e),
				    obj_ptr(&obj),
				    m_ptr(_p) {}
	EventConnector1(const EventConnector1<T,D>& _ec): EventConnectorBase(_ec) {
	    obj_ptr = _ec.obj_ptr;
	    m_ptr = _ec.m_ptr;
	    data = _ec.data;
	}
	EventConnector1<T,D>& operator=(const EventConnector1<T,D>& _ec) {
	    EventConnectorBase::operator=(_ec);
	    m_ptr = _ec.mptr;
	    data = _ec.data;
	    return *this;
	}
	void setData(const D& _d) { data = _d; }
	int call() {
	    return obj_ptr->*m_ptr(data);
	}
};

#endif // EVENTCONNECTOR_H
