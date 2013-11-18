// -*- c++ -*-
//
// $Id$
//
// Copyright (C) 2009-2010  Rafael Ostertag
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

#ifndef _PWGENEXCEPTION_H
#define _PWGENEXCEPTION_H 1

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#include <exception>
#include <string>

#include "../../intl.h"


namespace YAPET {

    namespace PWGEN {
        /**
         * @brief PWGen exception base class.
         *
         * Base class for PWGen exceptions.
         */
        class PWGenException : public std::exception {
            private:
                std::string message;

            public:
                inline PWGenException() throw() : exception(), message (_ ("Generic exception message") ) {}
                inline PWGenException (std::string msg) throw() : exception(), message (msg) {}
                inline PWGenException (const PWGenException& ex) throw() {
                    message = ex.message;
                }
                inline virtual ~PWGenException() throw() {}
                inline const PWGenException& operator= (const PWGenException& ex)
                throw() {
                    if (this == &ex) return *this;

                    message = ex.message;
                    return *this;
                }
                inline virtual const char* what() const throw() {
                    return message.c_str();
                }
        };

        /**
         * @brief No RNG is available.
         *
         * No RNG is available.
         */
        class PWGenNoRNGException : public PWGenException {
            public:
                inline PWGenNoRNGException() throw() : PWGenException (_ ("No Random Number Generator available") ) {}
                inline PWGenNoRNGException (std::string msg) throw() : PWGenException (msg) {}
                inline PWGenNoRNGException (const PWGenNoRNGException& ex) throw() : PWGenException (ex) {}
                inline virtual ~PWGenNoRNGException() throw() {}
                inline const PWGenNoRNGException& operator= (const PWGenNoRNGException& ex)
                throw() {
		    PWGenException::operator= ( ex );
		    return *this;
                }
        };

        /**
         * @brief Requested RNG is not available
         *
         * The requested RNG is not available
         */
        class PWGenRNGNotAvailable : public PWGenException {
            public:
                inline PWGenRNGNotAvailable() throw() : PWGenException (_ ("The requested RNG is not available") ) {}
                inline PWGenRNGNotAvailable (std::string msg) throw() : PWGenException (msg) {}
                inline PWGenRNGNotAvailable (const PWGenRNGNotAvailable& ex) throw() : PWGenException (ex) {}
                inline virtual ~PWGenRNGNotAvailable() throw() {}
                inline const PWGenRNGNotAvailable& operator= (const PWGenRNGNotAvailable& ex)
                throw() {
                    PWGenException::operator= ( ex );
                    return *this;
                }
        };

    }
}

#endif // _PWGENEXCEPTION_H
