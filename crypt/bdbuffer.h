// -*- c++ -*-
//
// $Id$

#ifndef _BDBUFFER_H
#define _BDBUFFER_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#include <stdlib.h>
#include <stdexcept>

#include "gpsexception.h"

namespace GPSAFE {
    class BDBuffer {
	private:
	    size_t _size;
	    uint8_t* data;

	protected:
	    uint8_t* alloc_mem(size_t s) throw(GPSException);
	    void free_mem(uint8_t* d, size_t s);

	public:
	    BDBuffer(size_t is) throw(GPSException);
	    BDBuffer();
	    BDBuffer(const BDBuffer& ed) throw(GPSException);
	    ~BDBuffer();

	    void resize(size_t ns) throw(GPSException);
	    size_t size() const { return _size; }

	    uint8_t* at(size_t pos) throw(std::out_of_range);
	    const uint8_t* at(size_t pos) const throw(std::out_of_range);

	    const uint8_t* operator()() const { return data; }
	    uint8_t* operator()() { return data; }
	    

	    operator uint8_t*() { return data; }
	    operator const uint8_t*() const { return data; }

	    operator const void*() { return (void*)data; }
	    const BDBuffer& operator=(const BDBuffer& ed);
	    
    };
	
}

#endif // _BDBUFFER_H

