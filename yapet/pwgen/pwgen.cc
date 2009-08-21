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

PWGen::PWGen (SUBPOOLS p, RNGENGINE rnge) throw (std::runtime_error) : cp (NULL), rng(rnge), password (NULL), password_len (0) {
    init (p);
    assert (cp != NULL);
    assert (password == NULL);
}

PWGen::PWGen (int p, RNGENGINE rnge) throw (std::runtime_error) : cp (NULL), rng(rnge), password (NULL), password_len (0) {
    init (p);
    assert (cp != NULL);
    assert (password == NULL);
}

//
// Copy Constructor
//
PWGen::PWGen (const PWGen& pw) throw() : cp (NULL), rng(pw.rng), password (NULL), password_len (0) {
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
    cp->resetPoolsWithRead();
    
    size_t check_for_missing_charpools = password_len / 2;
    for (size_t pw_it = 0; pw_it < password_len; pw_it++) {
RESTART:
        char suggestion = 0;

	// Do need to check for unused character pools?
	if (pw_it > check_for_missing_charpools) {
	    // Do we have any pools not read
	    if (cp->numPoolsNotRead() > 0) {
		// Find out which one(s)
		int not_read = cp->getAllocatedPools() & 
		    ~ cp->getPoolsWithRead();

		// Iterate over the bits, until we find a pool from which we
		// will grab a character. And since this pool was never used
		// before, we don't even have to care about duplicates...
		for (int pool_it=YAPET::PWGEN::LETTERS;
		     pool_it <= YAPET::PWGEN::OTHER;
		     pool_it = pool_it << 1) {
		    if ( not_read & pool_it) {
			size_t pool_start;
			// Since cp->getPoolPos the length of the pool returns,
			// we use this to get the random number which we simply
			// can add to pool_start.
			//
			// We must not forget to subtract 1 from the return
			// value, else we might get an out of range error
			size_t random_val =rng(cp->getPoolPos((YAPET::PWGEN::SUBPOOLS)pool_it,
							      &pool_start)-1); 
			suggestion = (*cp)[pool_start + random_val];
			break;
		    } // if ( not_read & i)
		} // for (int i=...
	    } //if (cp->numPoolsNotRead() > 0)
	}

	// suggestion != 0 means the code above has already choosen a character
	// for the password
	if (suggestion == 0) {
	    if (cp->getPoolLength() >= password_len) {
		// We can avoid repeating characters
		bool character_repeat;
		
		do {
		    character_repeat = false;
		    suggestion = (*cp) [rng (cp->getPoolLength() ) ];
		    
		    for (size_t pos = 0; pos < pw_it; pos++) {
			if (suggestion == password[pos]) {
			    character_repeat = true;
			    break;
			}
		    }
		} while (character_repeat);
	    } else {
		// We can't avoid repeating characters
		suggestion = (*cp) [rng (cp->getPoolLength() ) ];
	    }
	} // (suggentsion != 0)
	
        // We want to avoid spaces at the beginning or end of the password
        if ( (pw_it == 0) || (pw_it == password_len - 1) ) {
#if defined(HAVE_ISBLANK) || defined(HAVE_ISSPACE)
# ifdef HAVE_ISBLANK

            if (isblank (suggestion) != 0)
                goto RESTART;

# else

            if (isspace (suggestion) != 0)
                goto RESTART;

# endif // HAVE_ISBLANK
#else // defined(HAVE_ISBLANK) || defined(HAVE_ISSPACE)

            if (suggestion == ' ')
                goto RESTART;

#endif // defined(HAVE_ISBLANK) || defined(HAVE_ISSPACE)
        } // if ( (i == 0) || (i == password_len - 1) ) 

        ( (char*) password) [pw_it] = suggestion;
    } // for (size_t i = 0; i < password_len; i++) 

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
    rng = pw.rng;

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
