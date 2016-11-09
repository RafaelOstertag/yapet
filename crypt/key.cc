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

#include "intl.h"
#include "key.h"

#include <cstring>

using namespace YAPET;

/**
 * It clears the memory occupied by the key and the initialization
 * vector by setting it to zero.
 */
void
Key::cleanup() {
    std::memset (key, 0, KEYLENGTH);
    std::memset (IVec, 0, IVECLENGTH);
}

EVP_MD_CTX*
Key::create_context() {
#ifdef HAVE_EVP_MD_CTX_CREATE
    return EVP_MD_CTX_create ();
#elif HAVE_EVP_MD_CTX_NEW
    return EVP_MD_CTX_new();
#else
#error "Neither EVP_MD_CTX_create() nor EVP_MD_CTX_new() available"
#endif
}

void
Key::destroy_context(EVP_MD_CTX *context) {
#ifdef HAVE_EVP_MD_CTX_DESTROY
    EVP_MD_CTX_destroy (context);
#elif HAVE_EVP_MD_CTX_FREE
    EVP_MD_CTX_free(context);
#else
#error "Neither EVP_MD_CTX_destroy() nor EVP_MD_CTX_free() available"
#endif
}
    

/**
 * Initializes the key and the initialization vector. Make sure you
 * securely destroy the password provided to this method.
 *
 * @param password a pointer to the location the password is
 * stored. The password has to be zero-terminated.
 */
Key::Key (const char* password) throw (YAPETException) {
    // Sentinel variable to check the size of the key
    uint8_t eff_keylength;
    //
    // First run (sha1)
    //
    const EVP_MD* md = EVP_sha1();

    if (md == 0)
        throw YAPETException (_ ("Run 1: Unable to initialize the EVP_MD structure") );

    EVP_MD_CTX *mdctx = create_context();

    int retval = EVP_DigestInit_ex (mdctx, md, 0);

    if (retval == 0) {
        destroy_context(mdctx);
        throw YAPETException (_ ("Run 1: Unable to initialize the digest") );
    }

    retval = EVP_DigestUpdate (mdctx, password, std::strlen (password) );

    if (retval == 0) {
        destroy_context(mdctx);
        throw YAPETException (_ ("Run 1: Unable to update the digest") );
    }

    unsigned int tmplen;
    retval = EVP_DigestFinal_ex (mdctx, key, &tmplen);

    if (retval == 0) {
        destroy_context(mdctx);
        cleanup();
        throw YAPETException (_ ("Run 1: Unable to finalize the digest") );
    }

    if (tmplen != SHA1_LEN) {
        destroy_context(mdctx);
        cleanup();
        throw YAPETException (_ ("Run 1: Digest does not have expected length") );
    }

    eff_keylength = tmplen;
    destroy_context(mdctx);
    //
    // Second run (md5)
    //
    md = EVP_md5();

    if (md == 0) {
        cleanup();
        throw YAPETException (_ ("Run 2: Unable to initialize the EVP_MD structure") );
    }

    mdctx=create_context();
    retval = EVP_DigestInit_ex (mdctx, md, 0);

    if (retval == 0) {
        destroy_context(mdctx);
        cleanup();
        throw YAPETException (_ ("Run 2: Unable to initialize the digest") );
    }

    retval = EVP_DigestUpdate (mdctx, key, SHA1_LEN);

    if (retval == 0) {
        destroy_context(mdctx);
        cleanup();
        throw YAPETException (_ ("Run 2: Unable to update the digest") );
    }

    retval = EVP_DigestFinal_ex (mdctx, key + SHA1_LEN, &tmplen);

    if (retval == 0) {
        destroy_context(mdctx);
        cleanup();
        throw YAPETException (_ ("Run 2: Unable to finalize the digest") );
    }

    if (tmplen != MD5_LEN) {
        destroy_context(mdctx);
        cleanup();
        throw YAPETException (_ ("Run 2: Digest does not have expected length") );
    }

    eff_keylength += tmplen;
    destroy_context(mdctx);
    //
    // Third run (ripemd160)
    //
    md = EVP_ripemd160();

    if (md == 0) {
        cleanup();
        throw YAPETException (_ ("Run 3: Unable to initialize the EVP_MD structure") );
    }

    mdctx = create_context();
    retval = EVP_DigestInit_ex (mdctx, md, 0);

    if (retval == 0) {
        destroy_context(mdctx);
        cleanup();
        throw YAPETException (_ ("Run 3: Unable to initialize the digest") );
    }

    retval = EVP_DigestUpdate (mdctx, key, SHA1_LEN + MD5_LEN);

    if (retval == 0) {
        destroy_context(mdctx);
        cleanup();
        throw YAPETException (_ ("Run 3: Unable to update the digest") );
    }

    retval = EVP_DigestFinal_ex (mdctx, key + SHA1_LEN + MD5_LEN, &tmplen);

    if (retval == 0) {
        destroy_context(mdctx);
        cleanup();
        throw YAPETException (_ ("Run 3: Unable to finalize the digest") );
    }

    if (tmplen != RIPEMD160_LEN) {
        destroy_context(mdctx);
        cleanup();
        throw YAPETException (_ ("Run 3: Digest does not have expected length") );
    }

    eff_keylength += tmplen;
    destroy_context(mdctx);

    if (eff_keylength != KEYLENGTH) {
        cleanup();
        char tmp[100];
        snprintf (tmp,
                  100,
                  _ ("Effective key length of %d does not match expected key length %d"),
                  eff_keylength,
                  KEYLENGTH);
        throw YAPETException (tmp);
    }

    //
    // The initialization vector
    //
    uint8_t ivec_hash_buf[MD5_LEN];
    md = EVP_md5();

    if (md == 0) {
        cleanup();
        throw YAPETException (_ ("IVec: Unable to initialize the EVP_MD structure") );
    }

    mdctx = create_context();
    retval = EVP_DigestInit_ex (mdctx, md, 0);

    if (retval == 0) {
        destroy_context(mdctx);
        cleanup();
        throw YAPETException (_ ("IVec: Unable to initialize the digest") );
    }

    retval = EVP_DigestUpdate (mdctx, key, SHA1_LEN + MD5_LEN + RIPEMD160_LEN);

    if (retval == 0) {
        destroy_context(mdctx);
        cleanup();
        throw YAPETException (_ ("IVec: Unable to update the digest") );
    }

    retval = EVP_DigestFinal_ex (mdctx, ivec_hash_buf, &tmplen);

    if (retval == 0) {
        destroy_context(mdctx);
        cleanup();
        throw YAPETException (_ ("IVec: Unable to finalize the digest") );
    }

    if (tmplen != MD5_LEN) {
        destroy_context(mdctx);
        cleanup();
        throw YAPETException (_ ("IVec: Digest does not have expected length") );
    }

    destroy_context(mdctx);
    std::memcpy (IVec, ivec_hash_buf, IVECLENGTH);
    std::memset (ivec_hash_buf, 0, MD5_LEN);
}

Key::Key (const Key& k) {
    std::memcpy (key, k.key, KEYLENGTH);
    std::memcpy (IVec, k.IVec, IVECLENGTH);
}

Key::~Key() {
    cleanup();
}

const Key&
Key::operator= (const Key & k) {
    if (this == &k) return *this;

    cleanup();
    std::memcpy (key, k.key, KEYLENGTH);
    std::memcpy (IVec, k.IVec, IVECLENGTH);
    return *this;
}

/**
 * Compares the key \c k provided for equality with this key.
 *
 * Both, the key itself and the initialization vector are compared.
 *
 * @param k reference to the key compared with this.
 *
 * @return \c true if both keys and initialization vectors are equal,
 * \c false otherwise.
 */
bool
Key::operator== (const Key& k) const {
    if (k.size() != size() ) return false;

    if (k.ivec_size() != ivec_size() ) return false;

    int retval = std::memcmp (k.key, key, size() );

    if (retval != 0)
        return false;

    retval = std::memcmp (k.IVec, IVec, ivec_size() );

    if (retval != 0)
        return false;

    return true;
}
