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

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_ERRNO_H
# include <errno.h>
#endif

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif

#ifdef HAVE_SYS_STAT_H
# include <sys/stat.h>
#endif

#ifdef HAVE_FCNTL_H
# include <fcntl.h>
#endif

#ifdef HAVE_STDIO_H
# include <stdio.h>
#endif

#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif

#ifdef HAVE_STRING_H
# include <string.h>
#endif

#ifdef HAVE_ASSERT_H
# include <assert.h>
#endif

#include "../../intl.h"

#include "pwgenexception.h"
#include "rng.h"


using namespace YAPET::PWGEN;

int RNG::rng_available = 0;

void
RNG::check_availability() throw (PWGenException) {
    if (access ("/dev/random", R_OK) == 0)
        rng_available |= DEVRANDOM;

    if (access ("/dev/urandom", R_OK) == 0 )
        rng_available |= DEVURANDOM;

#ifdef HAVE_LRAND48
    rng_available |= LRAND48;
#endif
#ifdef HAVE_RAND
    rng_available |= RAND;
#endif
    assert (rng_available != 0);

    if (rng_available == 0)
        throw PWGenNoRNGException();
}

int
RNG::getAvailableRNGs() {
    check_availability();
    return rng_available;
}

/**
 * Initializes the given engine.
 *
 * Expects \c check_availabilit() is already called.
 *
 * @param request the engine requested.
 */
void
RNG::init_rng (RNGENGINE request) throw (PWGenException) {
    assert (rng_available != 0);

    if (rng_available == 0)
        throw PWGenException (_ ("Unable to initialize RNG when none is available") );

    if (! (rng_available & request) )
        throw PWGenRNGNotAvailable();

    switch (request) {
        case DEVRANDOM:
            fd = open ("/dev/random", O_RDONLY);

            if ( fd < 0 )
                throw PWGenException (_ ("Unable to open /dev/random") );

            break;
        case DEVURANDOM:
            fd = open ("/dev/urandom", O_RDONLY);

            if ( fd < 0 )
                throw PWGenException (_ ("Unable to open /dev/urandom") );

            break;
        case LRAND48:
#if defined(HAVE_SRAND48) && defined(HAVE_TIME)
            srand48 (time (NULL) );
#endif
            break;
        case RAND:
#if defined(HAVE_RAND) && defined(HAVE_TIME)
            srand (time (NULL) );
#endif
            break;
        case AUTO:
            assert (0);
            throw PWGenException (_ ("Unexpected RNG Engine (AUTO)") );
            // To make the compiler (gcc -pedantic) happy
        case NONE:
            throw PWGenException (_ ("The requested RNG Engine (NONE) is invalid.") );
    }

    rng_used = request;
    rng_initialized = true;
}

/**
 * @brief returns a random number using the kernel random number generator
 *
 * return a random number from either /dev/random or /dev/urandom, depending on which is used.
 *
 * @param ceil the maximum value to return.
 *
 * @return a random number
 */
size_t
RNG::devrandom (size_t ceil) throw (PWGenException) {
    assert (rng_initialized);
    assert (rng_used == DEVRANDOM || rng_used == DEVURANDOM);
    assert (fd > -1);
    size_t buff;
    // This is an attempt to circumvent short reads appearing on some lx systems.
    //
    // Code courtesy of Richard W. Stevens. Thanks man!
    size_t nleft;
    ssize_t nread;
    size_t *ptr;

    ptr = &buff;
    nleft = sizeof(size_t);
    while( nleft > 0) {
	errno = 0;
	if ( (nread = read (fd, ptr, nleft )) < 0) {
	    // Error
	    switch (errno) {
		case EAGAIN:
		case EINTR:
		    // Just ignore and try again
		    break;
		default: {
		    char errmsg[1024];
		    snprintf(errmsg, 1024, "%s (%s)", _ ("Read to few bytes on /dev/[u]random."),
			     strerror(errno));
		    throw PWGenException ( errmsg );
		}
	    };
	} else {
	    if (nread == 0) {
		// EOF
		break;
	    }
	    nleft -= nread;
	    ptr += nread;
	}
    }

    if (buff > ceil)
        return buff % ceil;

    return buff;
}

size_t
RNG::_lrand48 (size_t ceil) throw() {
    assert (rng_initialized);
    assert (rng_used == LRAND48);
#ifdef HAVE_LRAND48
    long val = lrand48();

    if ( (size_t) val > ceil)
        return val % ceil;

    return (size_t) val;
#else
    assert (0);
    throw PWGenRNGNotAvailable (_ ("lrand48() not available on system") );
    // To make compiler happy
    return 0;
#endif
}

size_t
RNG::_rand (size_t ceil) throw() {
    assert (rng_initialized);
    assert (rng_used == RAND);
    assert (RAND_MAX >= ceil);
#ifdef HAVE_RAND
    int val = rand();

    if ( (size_t) val > ceil)
        return val % ceil;

    return (size_t) val;
#else
    assert (0);
    throw PWGenRNGNotAvailable (_ ("rand() not available on system") );
    // To make compiler happy
    return 0;
#endif
}

/**
 * Try to initialize the given RNG Engine.
 *
 * If the AUTO Engine is requested (the default value) the constructor
 * utomatically initializes a suitable random number generator using the
 * order shown below:
 *
 * - /dev/random
 * - /dev/urandom
 * - lrand48
 * - rand
 *
 * It takes the first it finds from the list above.
 *
 * @param request the requested engine or AUTO for auto determination.
 */
RNG::RNG (RNGENGINE request) throw (PWGenException) : fd (-1),
        rng_initialized (false),
        rng_used (NONE) {
    check_availability();

    if (request != AUTO) {
        init_rng (request);
    } else {
        assert (rng_available != 0);

	// Since version 0.6, /dev/urandom is the default rng used.
        if (rng_available & DEVURANDOM) {
            init_rng (DEVURANDOM);
            return;
        }

        if (rng_available & DEVRANDOM) {
            init_rng (DEVRANDOM);
            return;
        }

        if (rng_available & LRAND48) {
            init_rng (LRAND48);
            return;
        }

        if (rng_available & RAND) {
            init_rng (RAND);
            return;
        }
    }
}

//
// *** Copy Constructor
//
RNG::RNG (const RNG& r) throw (PWGenException) {
    assert (r.rng_initialized);

    // Code below takes care of the file descriptor
    if ( (r.rng_used == DEVRANDOM) ||
            (r.rng_used == DEVURANDOM) ) {
        assert (r.fd > -1);
        fd = dup (r.fd);

        if (fd < 0)
            throw PWGenException ("Unable to duplicate file descriptor");
    } else {
        assert (r.fd == -1);
        fd = r.fd;
    }

    // From here on we may more or less simply copy values
    rng_initialized = r.rng_initialized;
    rng_used = r.rng_used;
    rng_available = r.rng_available;

    if (rng_used == LRAND48)
        init_rng (LRAND48);

    if (rng_used == RAND)
        init_rng (RAND);
}

RNG::~RNG() throw() {
    if (rng_used == DEVRANDOM ||
            rng_used == DEVURANDOM) {
        assert (fd > 0);
        close (fd);
    } else {
        assert (fd == -1);
    }
}

size_t
RNG::getRandomNumber (size_t ceil) throw (PWGenException) {
    assert(ceil > 0);
    if (ceil == 0) return 0;
    assert (rng_initialized == true);

    switch (rng_used) {
        case DEVRANDOM:
        case DEVURANDOM:
            return devrandom (ceil);
        case LRAND48:
            return _lrand48 (ceil);
        case RAND:
            return _rand (ceil);
            // To make the compiler (gcc -pedantic) happy
        case AUTO:
            assert (0);
            throw PWGenException (_ ("Unexpected RNG Engine (AUTO)") );
        case NONE:
            throw PWGenException (_ ("The requested RNG Engine (NONE) is invalid.") );
    }

    // To make the compiler even more happy
    return 0;
}

const RNG&
RNG::operator= (const RNG & r) throw() {
    if (&r == this) return *this;

    assert (r.rng_initialized);

    // Make sure we close the current fd if used
    if (rng_used == DEVRANDOM ||
            rng_used == DEVURANDOM) {
        assert (fd > -1);
        close (fd);
        fd = -1;
    } else {
        assert (fd == -1);
    }

    // Code below takes care of the file descriptor from r
    if ( (r.rng_used == DEVRANDOM) ||
            (r.rng_used == DEVURANDOM) ) {
        assert (r.fd > -1);
        fd = dup (r.fd);

        if (fd < 0)
            throw PWGenException ("Unable to duplicate file descriptor");
    } else {
        assert (r.fd == -1);
        fd = r.fd;
    }

    // From here on we may more or less simply copy values
    rng_initialized = r.rng_initialized;
    rng_used = r.rng_used;
    rng_available = r.rng_available;

    if (rng_used == LRAND48)
        init_rng (LRAND48);

    if (rng_used == RAND)
        init_rng (RAND);

    return *this;
}
