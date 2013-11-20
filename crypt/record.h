// -*- c++ -*-
//
// $Id$
//
// Copyright (C) 2008-2010  Rafael Ostertag
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
// Additional permission under GNU GPL version 3 section 7
//
// If you modify this program, or any covered work, by linking or combining it
// with the OpenSSL project's OpenSSL library (or a modified version of that
// library), containing parts covered by the terms of the OpenSSL or SSLeay
// licenses, Rafael Ostertag grants you additional permission to convey the
// resulting work.  Corresponding Source for a non-source form of such a
// combination shall include the source code for the parts of OpenSSL used as
// well as that of the covered work.
//

#ifndef _RECORD_H
#define _RECORD_H 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef HAVE_INTTYPES_H
# include <inttypes.h>
#endif

#include <cstdlib>
#include <cstring>

#include "intl.h"

#include "bdbuffer.h"
#include "yapetexception.h"

namespace YAPET {

    /**
     * @brief Template for allocating/deallocating memory for structs
     *
     * The primary intend of this template is to make sure the memory allocated
     * for a struct is zero'ed out upon deallocation.
     *
     * The template allocates enough memory on the heap for holding
     * the struct of type \c T.
     *
     * It allows direct manipulation of the struct on the heap.
     */
    template<class T>
    class Record {
        private:
            /**
             * @brief The size of the memory allocated.
             *
             * The size of the memory allocated. Used to zero out the
             * memory used by the struct.
             */
            uint32_t _size;
            /**
             * @brief Pointer to the struct.
             *
             * Pointer to the struct on the heap.
             */
            T* data;

            /**
             * @brief Allocate memory for the struct.
             *
             * Allocates the proper amount of memory for holding the
             * struct and sets the \c _size field.
             */
            void alloc_mem() throw (YAPETException) {
                data = (T*) malloc (sizeof (T) );

                if (data == NULL)
                    throw YAPETException (_ ("Memory exhausted") );

                _size = sizeof (T);
            }

            /**
             * @brief Zero out and free memory.
             *
             * Zero'es the memory out and frees it.
             */
            void free_mem() {
                memset (data, 0, _size);
                free (data);
            }

        public:
            /**
             * @brief Allocates memory.
             *
             * Allocates memory of the proper size and copies the
             * content of the given struct \c d.
             *
             * @param d reference to the struct from where the content
             * is copied to the allocated memory.
             */
            Record<T> (const T& d) throw (YAPETException) {
                alloc_mem();
                memcpy (data, &d, sizeof (T) );
            }

            /**
             * @brief Allocates memory to hold a struct of the type \c
             * T.
             *
             * Allocates memory to hold a struct of the type \c T.
             */
            Record<T>() throw (YAPETException) {
                alloc_mem();
            }

            Record<T> (const Record<T>& r) throw (YAPETException) {
                alloc_mem();
                memcpy (data, r.data, _size);
            }

            virtual ~Record<T>() {
                free_mem();
            }

            /**
             * @brief Get the size of the allocated memory.
             *
             * Get the size of the allocated memory.
             */
            uint32_t size() const {
                return _size;
            }

            /**
             * @brief Get the pointer to the struct.
             *
             * Gets the pointer to the struct on the heap.
             *
             * @return pointer to the struct on the heap.
             */
            T* getData() {
                return data;
            }
            /**
             * @brief Get the pointer to the struct.
             *
             * Gets the pointer to the struct on the heap.
             *
             * @return pointer to the struct on the heap.
             */
            const T* getData() const {
                return data;
            }

            /**
             * @brief Cast to the pointer of the struct.
             *
             * Cast to the pointer of the struct on the heap.
             *
             * @return pointer to the struct on the heap.
             */
            operator T*() {
                return data;
            }
            /**
             * @brief Cast to the pointer of the struct.
             *
             * Cast to the pointer of the struct on the heap.
             *
             * @return pointer to the struct on the heap.
             */
            operator const T*() const {
                return data;
            }
            /**
             * @brief Cast operator.
             *
             * Cast operator used by the openssl functions.
             *
             * @return pointer to the struct casted to an unsigned 8 bits
             * integer pointer.
             */
            operator uint8_t*() {
                return (uint8_t*) data;
            }
            /**
             * @brief Cast operator.
             *
             * Cast operator used by the openssl functions.
             *
             * @return pointer to the struct casted to an unsigned 8 bits
             * integer pointer.
             */
            operator const uint8_t*() const {
                return (const uint8_t*) data;
            }

            /**
             * @brief Assignment operator.
             *
             * Assigns another \c Record to \c this.
             *
             * @param r reference to a \c Record.
             *
             * @return const reference to \c this.
             */
            const Record<T>& operator= (const Record<T>& r)
            throw (YAPETException) {
                if (this == &r) return *this;

                free_mem();
                // This sets _size member too
                alloc_mem();
                memcpy (data, r.data, r._size);
                return *this;
            }

            /**
             * @brief Assignment operator.
             *
             * Assigns a struct of type \c T to \c this.
             *
             * @param r reference to a struct of type \c T.
             *
             * @return const reference to \c this.
             */
            const Record<T>& operator= (const T& r) throw (YAPETException) {
                free_mem();
                // This sets _size member too
                alloc_mem();
                memcpy (data, &r, _size);
                return *this;
            }

            /**
             * @brief Assignment operator.
             *
             * Assigns a struct of type \c T to \c this.
             *
             * @param r pointer to a struct of type \c T.
             *
             * @return const reference to \c this.
             */
            const Record<T>& operator= (const T* r) throw (YAPETException) {
                free_mem();
                // This sets _size member too
                alloc_mem();
                memcpy (data, r, _size);
                return *this;
            }

            /**
             * @brief Assignment operator.
             *
             * Assigns a \c BDBuffer. If the size of the \c BDBuffer is smaller
             * than the size of the struct, an exception is thrown.
             *
             * Only as much bytes as fit into struct are copied from the \c
             * BDBuffer.
             *
             * @param bdb reference to a \c BDBuffer.
             *
             * @return const reference to \c this.
             */
            const Record<T>& operator= (const BDBuffer& bdb)
            throw (YAPETException) {
		// As of version 0.6, the exnum is used to determine whether to use a 32 or 64 bit header
                if (bdb.size() < _size)
                    throw YAPETException (_ ("BDBuffer too small. Cannot assign to Record<T>"), BDBUFFER_TOO_SMALL );
		if (bdb.size() > _size)
		    throw YAPETException (_ ("BDBuffer too big. Cannot assign to Record<T>"), BDBUFFER_TOO_BIG );

                free_mem();
                // This sets _size member too
                alloc_mem();
                memcpy (data, bdb(), _size);
                return *this;
            }
    };
}

#endif // _RECORD_H
