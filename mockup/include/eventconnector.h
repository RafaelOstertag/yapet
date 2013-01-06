// -*- mode: c++ -*-
//
// $Id$

#ifndef EVENTCONNECTOR_H
#define EVENTCONNECTOR_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_FUNCTIONAL
#include <functional>
#endif

#ifdef HAVE_TYPEINFO
#include <typeinfo>
#endif // HAVE_TYPEINFO

#include "event.h"

template <class _Arg, class _Tt, class _A> 
class mem_fun1 : public std::binary_function<_Tt*,_A,_Arg> {
    private:
	_Arg (_Tt::*pmf)(_A);
	
    public:
	explicit mem_fun1(_Arg (_Tt::*p)(_A)) : pmf(p) { }
	bool operator==(const mem_fun1<_Arg,_Tt,_A>& _ref) const { return pmf == _ref.pmf; }
	_Arg operator()(_Tt* p, _A a) const { return (p->*pmf)(a); }
};

template <class _Arg, class _Tt, class _A> 
inline mem_fun1<_Arg,_Tt,_A> mem_fun(_Arg (_Tt::*f)(_A)) {
    return mem_fun1<_Arg,_Tt,_A>(f);
}


class EventConnectorBase {
    private:
	EVENT_TYPE evt;

    protected:
	virtual bool compare(const EventConnectorBase& ) const = 0;

    public:
	inline EventConnectorBase(EVENT_TYPE _e): evt(_e) {}
	inline EventConnectorBase(const EventConnectorBase& _ec) {
	    evt = _ec.evt;
	}
	inline virtual ~EventConnectorBase() {}
	inline EventConnectorBase& operator=(const EventConnectorBase& _ec) {
	    evt = _ec.evt;
	    return *this;
	}
	inline bool operator==(const EventConnectorBase& _ec) const {
	    return evt == _ec.evt && this->compare(_ec);
	}
	inline bool operator!=(const EventConnectorBase& _ec) const {
	    return !operator==(_ec);
	}
	inline bool operator==(const EventBase& _eb) const {
	    return _eb.type() == evt;
	}
	inline bool operator!=(const EventBase& _eb) const {
	    return !operator==(_eb);
	}
	inline EVENT_TYPE type() const { return evt; }
	virtual int call(EventBase&) const = 0;
	virtual EventConnectorBase* clone() const = 0;
};

template<class T /* Type of object called */>
class EventConnectorMethod1: public EventConnectorBase {
	typedef int (T::*__mem_fun_t)(EventBase&);
	typedef T* _obj_ptr_t;
    private:
	__mem_fun_t __func;
	_obj_ptr_t __obj_ptr;

    protected:
	bool compare(const EventConnectorBase& eb) const {
	    if (typeid(eb) == typeid(EventConnectorMethod1<T>)) {
		EventConnectorMethod1<T> tmp(dynamic_cast<const EventConnectorMethod1<T>& >(eb));
		return tmp.__func == __func && tmp.__obj_ptr == __obj_ptr;
	    }
	    return false;
	}

    public:
	EventConnectorMethod1(EVENT_TYPE _e,
			      _obj_ptr_t _obj_ptr,
			      __mem_fun_t _func): EventConnectorBase(_e),
						  __func(_func),
						  __obj_ptr(_obj_ptr) {}
	EventConnectorMethod1(const EventConnectorMethod1<T>& _ec): EventConnectorBase(_ec),
								    __func(_ec.__func),
								    __obj_ptr(_ec.__obj_ptr) {}
	EventConnectorMethod1<T>& operator=(const EventConnectorMethod1<T>& _ec) {
	    EventConnectorBase::operator=(_ec);
	    __func = _ec.__func;
	    __obj_ptr = _ec.__obj_ptr;
	    return *this;
	}
	int call(EventBase& _a) const {
	    return (__obj_ptr->*__func)(_a);
	}
	EventConnectorBase* clone() const {
	    return new EventConnectorMethod1<T>(*this);
	}
};

class EventConnectorFunction1: public EventConnectorBase {
	typedef int (*fptr_t)(EventBase&);
    private:
	fptr_t __func;

    protected:
	bool compare(const EventConnectorBase& eb) const {
	    if (typeid(eb) == typeid(EventConnectorFunction1)) {
		EventConnectorFunction1 tmp(dynamic_cast<const EventConnectorFunction1&>(eb));
		return tmp.__func == __func;
	    }
	    return false;
	}

    public:
	EventConnectorFunction1(EVENT_TYPE _e, fptr_t _func):
	    EventConnectorBase(_e), __func(_func) {}
	EventConnectorFunction1(const EventConnectorFunction1& _ec):
	    EventConnectorBase(_ec) {
	    __func = _ec.__func;
	}
	EventConnectorFunction1& operator=(const EventConnectorFunction1& _ec) {
	    EventConnectorBase::operator=(_ec);
	    __func = _ec.__func;
	    return *this;
	}
	int call(EventBase& _a) const {
	    return __func(_a);
	}
	EventConnectorBase* clone() const {
	    return new EventConnectorFunction1(*this);
	}
};

#endif // EVENTCONNECTOR_H
