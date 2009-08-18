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

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#if !defined(NDEBUG) && defined(HAVE_ASSERT_H)
# include <assert.h>
#endif

#include "../../intl.h"

#include "pwgen.h"

using namespace YAPET::PWGEN;

void
PWGen::init (int p) throw (std::runtime_error) {
    cp = new CharacterPool (p);
}

PWGen::PWGen (SUBPOOLS p) throw (std::runtime_error) : cp (NULL), password (NULL), password_len (0) {
    init (p);
    assert (cp != NULL);
    assert (password == NULL);
}

PWGen::PWGen (int p) throw (std::runtime_error) : cp (NULL), password (NULL), password_len (0) {
    init (p);
    assert (cp != NULL);
    assert (password == NULL);
}

//
// Copy Constructor
//
PWGen::PWGen (const PWGen& pw) throw() : cp (NULL), password (NULL), password_len (0) {
    cp = new CharacterPool (* (pw.cp) );

    if (pw.password != NULL) {
        assert (pw.password_len > 0);
        password = new char[pw.password_len + 1];
        memcpy ( (void*) password, pw.password, pw.password_len);
        // Don't forget to zero terminate!
        ( (char*) password) [pw.password_len + 1] = '\0';
        password_len = pw.password_len;
    }
}

PWGen::~PWGen() throw() {
    assert (cp != NULL);
    delete cp;

    if (password_len != 0) assert (password != NULL);

    if (password != NULL) {
        assert (password_len > 0);
        memset ( (void*) password, 0, password_len);
        delete[] password;
    }
}

void
PWGen::setNewPool (int p) throw (std::runtime_error) {
    assert (cp != NULL);
    delete cp;
    cp = new CharacterPool (p);
}

void
PWGen::generatePassword (size_t len) throw (std::logic_error) {
    if (len == 0) return;

    if (password != NULL) {
        assert (password_len > 0);
        memset ( (void*) password, 0, password_len);
        delete[] password;
    }

    password_len = len;
    password = new char[password_len + 1];

    for (size_t i = 0; i < password_len; i++) {
RESTART:
        char sugestion;

        if (cp->getPoolLength() >= len) {
            // We can avoid repeating characters
            bool character_repeat;

            do {
                character_repeat = false;
                sugestion = (*cp) [rng (cp->getPoolLength() ) ];

                for (size_t pos = 0; pos < i; pos++) {
                    if (sugestion == password[pos]) {
                        character_repeat = true;
                        break;
                    }
                }
            } while (character_repeat);
        } else {
            // We can't avoid repeating characters
            sugestion = (*cp) [rng (cp->getPoolLength() ) ];
        }

        // We want to avoid spaces at the beginning or end of the password
        if ( (i == 0) || (i == password_len - 1) ) {
#if defined(HAVE_ISBLANK) || defined(HAVE_ISSPACE)
# ifdef HAVE_ISBLANK

            if (isblank (sugestion) != 0)
                goto RESTART;

# else

            if (isspace (sugestion) != 0)
                goto RESTART;

# endif // HAVE_ISBLANK
#else // defined(HAVE_ISBLANK) || defined(HAVE_ISSPACE)

            if (sugestion == ' ')
                goto RESTART;

#endif // defined(HAVE_ISBLANK) || defined(HAVE_ISSPACE)
        }

        ( (char*) password) [i] = sugestion;
    }

    // Don't forget to zero terminate!
    ( (char*) password) [password_len] = '\0';
}

const char*
PWGen::getPassword() const throw() {
    assert ( ( (password != NULL) && (password_len > 0) ) ||
             ( (password == NULL) && (password_len == 0) ) );

    if (password == NULL) return NULL;

    assert (password[password_len] == '\0');
    return password;
}

const PWGen&
PWGen::operator= (const PWGen & pw) throw() {
    assert (cp != NULL);

    if (&pw == this) return *this;

    delete cp;
    cp = new CharacterPool (* (pw.cp) );

    if (password != NULL) {
        assert (password_len > 0);
        memset ( (void*) password, 0, password_len);
        delete[] password;
    }

    if (pw.password != NULL) {
        assert (pw.password_len > 0);
        password = new char[pw.password_len];
        memcpy ( (void*) password, pw.password, pw.password_len);
        // Don't forget to zero terminate
        ( (char*) password) [pw.password_len + 1] = '\0';
        password_len = pw.password_len;
    }

    return *this;
}
