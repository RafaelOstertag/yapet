// -*- c++ -*-
//
// $Id$
//
// @@REPLACE@@
// Copyright (C) 2008  Rafael Ostertag
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef _RECORD_H
#define _RECORD_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_INTTYPES_H
# include <inttypes.h>
#endif

#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif

#ifdef HAVE_STRING_H
# include <string.h>
#endif

#include "bdbuffer.h"
#include "gpsexception.h"

namespace GPSAFE {

    template<class T>
    class Record {
	private:
	    uint32_t _size;
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

	    uint32_t size() const { return _size; }

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
