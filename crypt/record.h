// -*- c++ -*-
//
// $Id$

#ifndef _RECORD_H
#define _RECORD_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#include <stdlib.h>
#include <string.h>

#include "bdbuffer.h"
#include "gpsexception.h"

namespace GPSAFE {

    template<class T>
    class Record {
	private:
	    size_t _size;
	    T* data;

	    void alloc_mem() throw(GPSException) {
		data = (T*) malloc(sizeof(T));
		if (data == NULL)
		    throw GPSException("Memory exhausted");

		_size = sizeof(T);
	    }

	    void free_mem() {
		memset(data, 0, _size);
		free(data);
	    }

	public:
	    Record<T>(const T& d) throw(GPSException) {
		alloc_mem();
		memcpy(data, &d, sizeof(T));
	    }
		
	    Record<T>() throw (GPSException){
		alloc_mem();
	    }

	    Record<T>(const Record<T>& r) throw (GPSException) {
		alloc_mem();
		memcpy(data, r.data, _size);
	    }

	    virtual ~Record<T>() {
		free_mem();
	    }

	    size_t size() const { return _size; }

	    T* getData() { return data; }
	    const T* getData() const { return data; }

	    operator T*() { return data; }
	    operator const T*() const { return data; }

	    operator uint8_t*() { return (uint8_t*)data; }
	    operator const uint8_t*() const { return (const uint8_t*)data; }
	    
	    const Record<T>& operator=(const Record<T>& r) throw(GPSException) {
		if (this == &r) return *this;

		free_mem();
		
		// This sets _size member too
		alloc_mem();
		memcpy(data, r.data, r._size);

		return *this;
	    }

	    const Record<T>& operator=(const T& r) throw(GPSException) {
		free_mem();
		// This sets _size member too
		alloc_mem();
		memcpy(data, &r, _size);

		return *this;
	    }

	    const Record<T>& operator=(const T* r) throw(GPSException){
		free_mem();
		// This sets _size member too
		alloc_mem();
		memcpy(data, r, _size);

		return *this;
	    }

	    const Record<T>& operator=(const BDBuffer& bdb) throw(GPSException) {
		if (bdb.size() < _size) throw GPSException("BDBuffer too small");

		free_mem();
		// This sets _size member too
		alloc_mem();

		memcpy(data, bdb(), _size);

		return *this;
	    }
    };
}

#endif // _RECORD_H
