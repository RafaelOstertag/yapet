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

#include "crypt.h"

using namespace YAPET;

EVP_CIPHER_CTX*
Crypt::create_context() {
#ifdef HAVE_EVP_CIPHER_CTX_INIT
    EVP_CIPHER_CTX *context = (EVP_CIPHER_CTX*) std::malloc(sizeof(EVP_CIPHER_CTX));
    EVP_CIPHER_CTX_init(context);
    return context;
#elif HAVE_EVP_CIPHER_CTX_NEW
    return EVP_CIPHER_CTX_new();
#else
    #error "Neither EVP_CIPHER_CTX_init() nor EVP_CIPHER_CTX_new() available"
#endif
}

void
Crypt::destroy_context(EVP_CIPHER_CTX *context) {
#ifdef HAVE_EVP_CIPHER_CTX_CLEANUP
    EVP_CIPHER_CTX_cleanup(context);
    std::free(context);
#elif HAVE_EVP_CIPHER_CTX_FREE
    EVP_CIPHER_CTX_free(context);
#else
#error "Neither EVP_CIPHER_CTX_cleanup() nor EVP_CIPHER_CTX_free() available"
#endif
}

/**
 * Initializes the class with the given key, which is used for
 * encryption and decryption.
 *
 * The constructor tries to set the key length of the cipher used to
 * the length of the key provided. If this fails, a \c YAPETException is
 * thrown.
 *
 * @param k the key used for encryption/decryption.
 *
 * @throw YAPETException in case the key length of the cipher cannot be
 * set to the length of the key provided.
 */
Crypt::Crypt (const Key& k) : cipher (0),
        iv_length (0),
        key_length (0),
        key (k) {
    cipher = EVP_bf_cbc();

    if (cipher == 0)
        throw YAPETException (_ ("Unable to get cipher") );

    // Test if key length is ok
    EVP_CIPHER_CTX* ctx = create_context();
    int retval = EVP_CipherInit_ex (ctx, cipher, 0, 0, 0, 0);

    if (retval == 0) {
        destroy_context(ctx);
        throw YAPETException (_ ("Error initializing cipher") );
    }

    retval = EVP_CIPHER_CTX_set_key_length (ctx, key.size() );

    if (retval == 0) {
	destroy_context(ctx);
        throw YAPETException (_ ("Error setting the key length") );
    }

    iv_length = EVP_CIPHER_CTX_iv_length (ctx);
    key_length = EVP_CIPHER_CTX_key_length (ctx);
    destroy_context(ctx);
}

Crypt::Crypt (const Crypt& c) : cipher (c.cipher),
        iv_length (c.iv_length),
        key_length (c.key_length),
        key (c.key) {
}

const Crypt&
Crypt::operator= (const Crypt & c) {
    if (this == &c) return *this;

    iv_length = c.iv_length;
    key_length = c.key_length;
    cipher = c.cipher;
    key = c.key;
    return *this;
}
