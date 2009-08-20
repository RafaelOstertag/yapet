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

#ifndef _PWGEN_H
#define _PWGEN_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_STDEXCEPT
# include <stdexcept>
#endif

#include "../../intl.h"

#include "rng.h"
#include "charpool.h"

namespace YAPET {
    /**
     * @brief Namespace for pwgen.
     *
     * All password generator related classes belong to this namespace.
     */
    namespace PWGEN {
        /**
         * @brief Generates a password.
         *
         * Generates a more or less secure password depending on the character pool used.
         *
         * @see CharacterPool
         */
        class PWGen {
            private:
                CharacterPool* cp;
                RNG rng;
                /**
                 * @brief will hold the password.
                 *
                 * This will hold the (zero terminated) password. It is entierly managed by this
                 * class. When PWGen returns a password, it is always a pointer
                 * to this buffer.
                 */
                const char* password;
                size_t password_len;

                void init (int p) throw (std::runtime_error);

            public:
                PWGen (SUBPOOLS p) throw (std::runtime_error);
                PWGen (int p) throw (std::runtime_error);
                PWGen (const PWGen& pw) throw();
                virtual ~PWGen() throw();

                void setNewPool (int p) throw (std::runtime_error);
                void generatePassword (size_t len) throw (std::logic_error);
                const char* getPassword() const throw();
                inline RNGENGINE getRNGUsed() const {
                    return rng.getRNGEngineUsed();
                }
		inline int getCharacterPools() const {
		    if (cp != NULL)
			return cp->getPoolsUsed();
		    return 0;
		}

                const PWGen& operator= (const PWGen& pw) throw();
        };
    }
}

#endif // _PWGEN_H
