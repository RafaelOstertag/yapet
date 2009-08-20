// -*- c++ -*-
//
// $Id$
//
// Copyright (C) 2009  Rafael Ostertag
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
#define _CHARPOOL_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_ASSERT_H
# include <assert.h>
#endif

#ifdef HAVE_STDEXCEPT
# include <stdexcept>
#endif

#include "../../intl.h"

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
            ALL = LETTERS | DIGITS | PUNCT | SPECIAL
        };
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

                int pools_used;

                void init (int p) throw (std::runtime_error);

            protected:
                static const char* get_letters();
                static const char* get_digits();
                static const char* get_punct();
                static const char* get_special();
                static const char* get_other();

                inline size_t pool_letters_len() const {
                    return strlen (letters);
                }
                inline size_t pool_digits_len() const {
                    return strlen (digits);
                }
                inline size_t pool_punct_len() const {
                    return strlen (punct);
                }
                inline size_t pool_special_len() const {
                    return strlen (special);
                }
                inline size_t pool_other_len() const {
                    return strlen (other);
                }

                inline const char* getPool (size_t* len) const throw (std::runtime_error) {
#ifndef NDEBUG
                    assert (len != NULL);
#else

                    if (len == NULL)
                        throw std::runtime_error (_ ("NULL pointer passed.") );

#endif
                    *len = pool_length;
                    return pool;
                }
                inline const char* getPool() const throw() {
                    return pool;
                }

            public:
                explicit CharacterPool (int p) throw (std::runtime_error);
                explicit CharacterPool (SUBPOOLS p) throw (std::runtime_error);
                virtual ~CharacterPool() throw();
                CharacterPool (const CharacterPool& cp) throw (std::runtime_error);

                inline int getPoolsUsed() const {
                    return pools_used;
                }
                inline bool isPoolUsed (SUBPOOLS p) const {
                    return (pools_used & p) ? true : false;
                }

                inline size_t getPoolLength() const throw() {
                    return pool_length;
                }

                //! Return a character at the position specified
                char operator[] (size_t pos) throw (std::logic_error);

                inline size_t operator() () const throw() {
                    return getPoolLength();
                }

                const CharacterPool& operator= (const CharacterPool& cp) throw (std::runtime_error);
#ifdef DEBUG
                void print_pools_used() const;
#endif
        };

    }
}

#endif // _CHARPOOL_H
