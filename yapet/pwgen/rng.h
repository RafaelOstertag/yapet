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

#ifndef _RNG_H
#define _RNG_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_STDEXCEPT
# include <stdexcept>
#endif

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif

#include "../../intl.h"

#include "pwgenexception.h"

namespace YAPET {

    namespace PWGEN {

        enum RNGENGINE {
            DEVRANDOM = (1 << 0),
            DEVURANDOM = (1 << 1),
            LRAND48 = (1 << 2),
            RAND = (1 << 3),
            AUTO = (1 << 4),
            NONE = 0
        };

        /**
         * @brief class for interfacing random number generators.
         *
         * Class for interfacing random number generators. It tries the
         * following random generators:
         *
         * - /dev/random
         * - /dev/urandom
         * - lrand48
         * - rand
         *
         * The random numbers are in the range of \c size_t, since this is the
         * range of the pool \c CharacterPool.
         *
         * @see CharacterPool
         */
        class RNG {
            private:
                /**
                 * @brief file descriptor
                 *
                 * File descriptor, used with /dev/[u]random.
                 */
                int fd;

                /**
                 * @brief is rng initialized
                 *
                 * Indicates that whatever steps are need to initialize the
                 * rng, they have been taken.
                 */
                bool rng_initialized;

                /**
                 * @brief which rng is used
                 *
                 * stores which rng is used.
                 */
                RNGENGINE rng_used;

                /**
                 * @brief which rng are available
                 *
                 * stores which rng are available.
                 */
                static int rng_available;

                static void check_availability() throw (PWGenException);
                //! Initializes the given engine.
                void init_rng (RNGENGINE request) throw (PWGenException);


                size_t devrandom (size_t ceil) throw (PWGenException);
                size_t _lrand48 (size_t ceil) throw();
                size_t _rand (size_t ceil) throw();

            public:
                //! Try a specific RNG Engine or use automatically determination
                RNG (RNGENGINE request = AUTO) throw (PWGenException);
                RNG (const RNG& r) throw (PWGenException);
                virtual ~RNG() throw();

                size_t getRandomNumber (size_t ceil) throw (PWGenException);

		static int getAvailableRNGs();

                RNGENGINE getRNGEngineUsed() const throw() {
                    return rng_used;
                }

                inline size_t operator() (size_t ceil) throw (PWGenException) {
                    return getRandomNumber (ceil);
                }
                const RNG& operator= (const RNG& r) throw();
        };
    }
}

#endif // _RNG_H
