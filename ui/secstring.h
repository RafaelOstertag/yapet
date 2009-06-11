// -*- c++ -*-
//
// $Id$
//
// Copyright (C) 2008, 2009  Rafael Ostertag
//
// This file is part of YAPET.
//
// YAPET is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// YAPET is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// YAPET.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef _SECSTRING_H
#define _SECSTRING_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_STRING_H
# include <string.h>
#endif

#ifdef HAVE_MEMORY
# include <memory>
#endif

#ifdef HAVE_STRING
# include <string>
#endif

/**
 * @file
 *
 * @brief A string class with memory clearing allocator.
 *
 * Holds the string class with a memory clearing allocator.
 */

namespace YAPETUI {

    /**
     * @brief Memory clearing allocator.
     *
     * This template implements a \c deallocate method which zero'es out the
     * memory released.
     */
    template <class T>
    class secallocator: public std::allocator<T> {
	public:
	    void deallocate(typename std::allocator<T>::pointer p,
			    typename std::allocator<T>::size_type n) {
		memset(p, '0', n*sizeof(std::allocator<T>::value_type));
		std::allocator<T>::deallocate(p, n);
	    }

    };

    /**
     * @brief String class using the \c secallocator allocator.
     *
     * This string class uses the \c secallocator allocator in order to clear
     * the memory occupied by the string.
     */
    typedef std::basic_string<char, std::char_traits<char>, secallocator<char> > secstring;

}

#endif // _SECSTRING_H
