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

#ifdef HAVE_STRING_H
# include <string.h>
#endif

#if defined(DEBUG) && defined(HAVE_IOSTREAM)
# include <iostream>
#endif

#include "../../intl.h"

#include "charpool.h"

using namespace YAPET::PWGEN;

const char CharacterPool::letters[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ";
const char CharacterPool::digits[] = "0123456789";
const char CharacterPool::punct[] = ".,;:-!?'";
const char CharacterPool::special[] = "_+\"*%&/()[]={}<>";
const char CharacterPool::other[] = "§°@#\\|$£~`^";

inline const char* CharacterPool::get_letters() {
    return letters;
}
inline const char* CharacterPool::get_digits() {
    return digits;
}
inline const char* CharacterPool::get_punct() {
    return punct;
}
inline const char* CharacterPool::get_special() {
    return special;
}
inline const char* CharacterPool::get_other() {
    return other;
}

void
CharacterPool::init (int p) throw (std::runtime_error) {
    if (p == 0)
        throw std::runtime_error (_ ("Subpools may not be zero") );

    for (int i=LETTERS; i <= OTHER; i = i << 1) {
	if (isPoolAllocated((SUBPOOLS)i))
	    pool_length += pool_len((SUBPOOLS)i);
    }

    pool = new char[pool_length];
#ifdef DEBUG
    assert (pool != NULL);
#else

    if (pool == NULL) {
        // Good luck!
        throw std::runtime_error (_ ("Out of memory") );
    }

#endif
    size_t copy_pos = 0;

    //
    //
    //
    //               .::!! HEY YOU WITH THE EDITOR !!::.
    //
    // Do not carelessly change the order of allocation of the pools. The
    // operator[] and getPoolPos() rely on the exact order to calculate which
    // pools have been read so far.
    //
    //
    //
    if  (isPoolAllocated(LETTERS)) {
        memcpy ( (void*) (pool + copy_pos), (void*) letters, pool_letters_len() );
        copy_pos += pool_letters_len();
    }

    if  (isPoolAllocated(DIGITS)) {
        memcpy ( (void*) (pool + copy_pos), (void*) digits, pool_digits_len() );
        copy_pos += pool_digits_len();
    }

    if  (isPoolAllocated(PUNCT)) {
        memcpy ( (void*) (pool + copy_pos), (void*) punct, pool_punct_len() );
        copy_pos += pool_punct_len();
    }

    if  (isPoolAllocated(SPECIAL)) {
        memcpy ( (void*) (pool + copy_pos), (void*) special, pool_special_len() );
        copy_pos += pool_special_len();
    }

    if  (isPoolAllocated(OTHER)) {
        memcpy ( (void*) (pool + copy_pos), (void*) other, pool_other_len() );
        copy_pos += pool_other_len();
    }

    assert (copy_pos == pool_length);
}

CharacterPool::CharacterPool (int p)  throw (std::runtime_error) : pool (NULL),
								   pool_length (0),
								   pools_allocated (p),
								   pools_reads(0)
{
    init (p);
}

CharacterPool::CharacterPool (SUBPOOLS p)  throw (std::runtime_error) : pool (NULL),
									pool_length (0),
									pools_allocated (p),
									pools_reads(0)
{
    init (p);
}

CharacterPool::~CharacterPool() throw() {
    assert (pool != NULL);
    delete[] pool;
}

//
// Copy constructor
//
CharacterPool::CharacterPool (const CharacterPool& cp) throw (std::runtime_error) :     pool (NULL),
        pool_length (cp.pool_length),
											pools_allocated (cp.pools_allocated),
											pools_reads (cp.pools_reads) {
    assert (cp.pool != NULL);
    assert (cp.pool_length > 0);
    pool = new char[pool_length];
#ifdef DEBUG
    assert (pool != NULL);
#else

    if (pool == NULL) {
        // Good luck!
        throw std::runtime_error (_ ("Out of memory") );
    }

#endif
    memcpy ( (void*) pool, (void*) cp.pool, pool_length);
}

int
CharacterPool::getNumPoolsNotRead() const {
    int retval = 0;
    
    for (int i=LETTERS; i <= OTHER; i = i << 1)
	if ( isPoolAllocated((SUBPOOLS)i) &&
	     ! hadPoolReads((SUBPOOLS)i) )
	    retval++;

    return retval;
}

/**
 * Return the position and length of the pool.
 *
 * Returns 0 and sets \c start=0 if the pool \c p is not allocated.
 *
 * Remember that start+length is not the last character of the pool in
 * question, but the first character of the adjacent pool (if any!!).
 *
 * @param p the pool in question.
 *
 * @param start pointer to size_t where the start position will be stored.
 *
 * @return the length of the pool. If zero, an error occured.
 */
size_t
CharacterPool::getPoolPos(SUBPOOLS p, size_t* start) const {
    if (start == NULL)
	return 0;

    if (!isPoolAllocated(p)) {
	*start=0;
	return 0;
    }

    *start = 0;
    int i = 0;
    for (i=LETTERS; i < p; i = i<<1) {
	if (isPoolAllocated((SUBPOOLS)i) ) {
	    (*start)+=pool_len((SUBPOOLS)i);
	    
	}
    }
    return pool_len((SUBPOOLS)i);
}

/**
 * Return the character at the postion \c pos specified from the pool.
 *
 * @param pos the position of the character to retrieve. Zero based, must be
 * less than the value reported by \c getPoolLength()
 *
 * @return The character at the specified position, or zero upon error, i.e. \c
 * pos greater than or equal to \c getPoolLength()
 */
char
CharacterPool::operator[] (size_t pos) throw (std::logic_error) {
    if (pos >= pool_length)
        throw std::out_of_range (_ ("No character at given position") );

    // Update the pools_reads variable. This relies on the knowledge of the
    // order we internally allocate pools
    size_t offset = 0;
    // We need to update the offset regardless of the position to be retrieved
    for (int i=LETTERS; i <= OTHER; i = i<<1) {
	if (isPoolAllocated((SUBPOOLS)i)) {
	    size_t start;
	    size_t len = getPoolPos((SUBPOOLS)i, &start);

	    if (start<=pos && 
		(start+len) > pos)
		pools_reads |= i;
	}
    }
    // Finished updating pools_reads.

    return pool[pos];
}

const CharacterPool&
CharacterPool::operator= (const CharacterPool & cp) throw (std::runtime_error) {
    assert (pool != NULL);

    if (&cp == this) return *this;

    delete[] pool;
    //CharacterPool::CharacterPool(cp);
    assert (cp.pool_length > 0);
    pool_length = cp.pool_length;
    pools_allocated = cp.pools_allocated;
    pools_reads = cp.pools_reads;
    pool = new char[pool_length];
#ifdef DEBUG
    assert (pool != NULL);
#else

    if (pool == NULL) {
        // Good luck!
        throw std::runtime_error (_ ("Out of memory") );
    }

#endif
    memcpy ( (void*) pool, (void*) cp.pool, pool_length);
    return *this;
}

#ifdef DEBUG
void
CharacterPool::print_pools_allocated() const {
    if  (pools_allocated & LETTERS)
        std::cout << "LETTERS (" << LETTERS << ")" << std::endl;

    if  (pools_allocated & DIGITS)
        std::cout << "DIGITS (" << DIGITS << ")" << std::endl;

    if  (pools_allocated & PUNCT)
        std::cout << "PUNCT (" << PUNCT << ")" << std::endl;

    if  (pools_allocated & SPECIAL)
        std::cout << "SPECIAL (" << SPECIAL << ")" << std::endl;

    if  (pools_allocated & OTHER)
        std::cout << "OTHER (" << OTHER << ")" << std::endl;
}
#endif
