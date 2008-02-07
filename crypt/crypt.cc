// $Id$
//
// YAPET -- Yet Another Password Encryption Tool
// Copyright (C) 2008  Rafael Ostertag
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#include "crypt.h"

using namespace YAPET;

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
Crypt::Crypt(const Key& k) throw(YAPETException) : cipher(NULL),
						 iv_length(0),
						 key_length(0),
						 key(k){
    cipher = EVP_bf_cbc();
    if (cipher == NULL)
	throw YAPETException("Unable to get cipher");

    // Test if key length is ok
    EVP_CIPHER_CTX ctx;
    EVP_CIPHER_CTX_init(&ctx);

    int retval = EVP_CipherInit_ex(&ctx, cipher, NULL, NULL, NULL, 0);
    if (retval == 0) {
	EVP_CIPHER_CTX_cleanup(&ctx);
	throw YAPETException("Error initializing cipher");
    }

    retval = EVP_CIPHER_CTX_set_key_length(&ctx, key.size());
    if (retval == 0) {
	EVP_CIPHER_CTX_cleanup(&ctx);
	throw YAPETException("Error setting the key length");
    }
    
    iv_length = EVP_CIPHER_CTX_iv_length(&ctx);
    key_length = EVP_CIPHER_CTX_key_length(&ctx);

    EVP_CIPHER_CTX_cleanup(&ctx);
}

Crypt::Crypt(const Crypt& c) : cipher(c.cipher),
			       iv_length(c.iv_length),
			       key_length(c.key_length),
			       key(c.key) {
}

const Crypt&
Crypt::operator=(const Crypt& c) {
    if (this == &c) return *this;

    iv_length = c.iv_length;
    key_length = c.key_length;
    cipher = c.cipher;
    key = c.key;

    return *this;
}
