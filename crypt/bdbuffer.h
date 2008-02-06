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

#ifndef _BDBUFFER_H
#define _BDBUFFER_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_INTTYPES_H
# include <inttypes.h>
#endif

#ifdef HAVE_STDLIB_h
# include <stdlib.h>
#endif

#ifdef HAVE_STDEXCEPT
# include <stdexcept>
#endif

#include "gpsexception.h"

namespace GPSAFE {
    class BDBuffer {
	private:
	    uint32_t _size;
	    uint8_t* data;

	protected:
	    uint8_t* alloc_mem(uint32_t s) throw(GPSException);
	    void free_mem(uint8_t* d, uint32_t s);

	public:
	    BDBuffer(uint32_t is) throw(GPSException);
	    BDBuffer();
	    BDBuffer(const BDBuffer& ed) throw(GPSException);
	    ~BDBuffer();

	    void resize(uint32_t ns) throw(GPSException);
	    uint32_t size() const { return _size; }

	    uint8_t* at(uint32_t pos) throw(std::out_of_range);
	    const uint8_t* at(uint32_t pos) const throw(std::out_of_range);

	    const uint8_t* operator()() const { return data; }
	    uint8_t* operator()() { return data; }
	    

	    operator uint8_t*() { return data; }
	    operator const uint8_t*() const { return data; }

	    operator const void*() { return (void*)data; }
	    const BDBuffer& operator=(const BDBuffer& ed);
	    
    };
	
}

#endif // _BDBUFFER_H

