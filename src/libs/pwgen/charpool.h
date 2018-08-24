// -*- c++ -*-
//
// $Id$
//
// Copyright (C) 2009-2013  Rafael Ostertag
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

#ifndef _CHARPOOL_H
#define _CHARPOOL_H 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cstring>
#include <cassert>
#include <stdexcept>

#include "intl.h"

namespace YAPET {
    namespace PWGEN {
        /**
         * @brief Available character pools
         *
         * Pools to choose from.
         */
        enum SUBPOOLS {
            /**
             * Letters: a-z, A-Z
             */
            LETTERS = (1 << 0),
            /**
             * Digits: 0-9
             */
            DIGITS = (1 << 1),
            /**
             * Punctuation characters: comma, semicolon, etc.
             */
            PUNCT = (1 << 2),
            /**
             * Special characters: #,(,), etc.
             */
            SPECIAL = (1 << 3),
            /**
             * Other characters
             */
            OTHER = (1 << 4),
            ALL = LETTERS | DIGITS | PUNCT | SPECIAL | OTHER,
	    // Used only by fromPool()
	    NOPOOL = 0
        };

	inline bool HAS_LETTERS(int v) { return (v & LETTERS) == LETTERS; }
	inline bool HAS_DIGITS(int v) { return (v & DIGITS) == DIGITS; }
	inline bool HAS_PUNCT(int v) { return (v & PUNCT) == PUNCT; }
	inline bool HAS_SPECIAL(int v) { return (v & SPECIAL) == SPECIAL; }
	inline bool HAS_OTHER(int v) { return (v & OTHER) == OTHER; }
	inline bool HAS_ALL(int v) { return (v & ALL) == ALL; }
	
        /**
         * @brief Generate character pools.
         *
         * Generate character pools according defined parameters.
         */
        class CharacterPool {
            private:
                static const char letters[];
                static const char digits[];
                static const char punct[];
                static const char special[];
                static const char other[];

                /**
                 * @brief hold the pool of characters returned
                 *
                 * @warning pool is NOT zero terminated!
                 *
                 * Hold the pool of characters returned to the caller later
                 * on. This class will take care of allocating and deallocating
                 * the pool.
                 */
                char* pool;

                size_t pool_length;

		/**
		 * The pools that have been allocated.
		 */
                int pools_allocated;

		/**
		 * Keeps track of which pools had at least one read.
		 */
		int pools_reads;

                void init (int p);

            protected:
                static const char* get_letters();
                static const char* get_digits();
                static const char* get_punct();
                static const char* get_special();
                static const char* get_other();

                inline size_t pool_letters_len() const {
                    return std::strlen (letters);
                }
                inline size_t pool_digits_len() const {
                    return std::strlen (digits);
                }
                inline size_t pool_punct_len() const {
                    return std::strlen (punct);
                }
                inline size_t pool_special_len() const {
                    return std::strlen (special);
                }
                inline size_t pool_other_len() const {
                    return std::strlen (other);
                }
		inline size_t pool_len(SUBPOOLS p) const {
		    switch (p) {
		    case LETTERS:
			return pool_letters_len();
		    case DIGITS:
			return pool_digits_len();
		    case PUNCT:
			return pool_punct_len();
		    case SPECIAL:
			return pool_special_len();
		    case OTHER:
			return pool_other_len();
		    case ALL:
			return pool_letters_len() +
			    pool_digits_len() +
			    pool_punct_len() +
			    pool_special_len() +
			    pool_other_len();
		    default:
			assert(0);
			return -1;
		    }
		}

                inline const char* getPool (size_t* len) const {
#ifndef NDEBUG
                    assert (len != 0);
#else

                    if (len == 0)
                        throw std::runtime_error (_ ("0 pointer passed.") );

#endif
                    *len = pool_length;
                    return pool;
                }
                inline const char* getPool() const throw() {
                    return pool;
                }

            public:
                explicit CharacterPool (int p);
                explicit CharacterPool (SUBPOOLS p);
                virtual ~CharacterPool() throw();
                CharacterPool (const CharacterPool& cp);

		/**
		 * Returns the number of pools allocated.
		 */
		int numPoolsAllocated() const;

                inline int getAllocatedPools() const {
                    return pools_allocated;
                }
                inline bool isPoolAllocated (SUBPOOLS p) const {
                    return (pools_allocated & p) ? true : false;
                }
		inline bool hadPoolReads (SUBPOOLS p) const {
		    return (pools_reads & p) ? true : false;
		}
		/**
		 * @brief Which pools had been read from.
		 *
		 * Return the pools that had at least one read so far.
		 *
		 * @return bitmask of the pools with reads.
		 */
		inline int getPoolsWithRead() const {
		    return pools_reads;
		}
		/**
		 * @brief Reset the bitmask of pools that have reads
		 *
		 * Reset (set to zero) the bitmask of the pools that have been
		 * read from.
		 */
		inline void resetPoolsWithRead() {
		    pools_reads = 0;
		}
		/**
		 * @brief Number of pools not read
		 *
		 * Return the number of pools from which not have been read so far
		 */
		int numPoolsNotRead () const;

		//! The position and length of the pool.
		size_t getPoolPos(SUBPOOLS p, size_t* start) const;

		//! Return the pool the character belongs to
		SUBPOOLS fromPool(char c) const;
   
                inline size_t getPoolLength() const throw() {
                    return pool_length;
                }

                //! Return a character at the position specified
                char operator[] (size_t pos);

                inline size_t operator() () const throw() {
                    return getPoolLength();
                }

                const CharacterPool& operator= (const CharacterPool& cp);
#ifdef DEBUG
                int print_pools_allocated() const;
#endif
        };

    }
}

#endif // _CHARPOOL_H
