// -*- c++ -*-
//
// $Id$

#ifndef _SECSTRING_H
#define _SECSTRING_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string.h>

#include <memory>
#include <string>

namespace GPSUI {

    template <class T>
    class secallocator: public std::allocator<T> {
	public:
	    void deallocate(typename std::allocator<T>::pointer p,
			    typename std::allocator<T>::size_type n) {
		memset(p, '0', n*sizeof(std::allocator<T>::value_type));
		std::allocator<T>::deallocate(p, n);
	    }
	    
    };
    
    typedef std::basic_string<char, std::char_traits<char>, secallocator<char> > secstring;

}

#endif // _SECSTRING_H
