// -*- c++ -*-
//
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

#ifndef _CRYPT_H
#define _CRYPT_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_INTTYPES_H
# include <inttypes.h>
#endif

#include <openssl/evp.h>

#include "yapetexception.h"
#include "key.h"
#include "bdbuffer.h"
#include "record.h"

namespace YAPET {
    /**
     * @brief Encrypts and decrypts data
     *
     * This class encrypts and decrypts data using the key provided by
     * \c Key.
     *
     * Encrypted data is returned in \c BDBuffer. Decrypted data is
     * returned as a struct as defined in \c structs.h using the
     * template \c Record.
     *
     * Currently, the blowfish algorithm in CBC mode from the openssl
     * library is used for encryption and decryption. The class itself
     * does not have any encryption/decryption logic.
     *
     * @sa Key, BDBuffer, Record
     */
    class Crypt {
	private:
	    /**
	     * @brief Pointer to the cipher used for
	     * encryption/decryption
	     *
	     * Pointer the cipher implemented in the openssl
	     * library. Currently, blowfish is used for
	     * encryption/decryption.
	     */
	    const EVP_CIPHER* cipher;
	    /**
	     * @brief Length of the initialization vector
	     *
	     * Length of the initialization vector as expected by the
	     * cipher implementation in bytes.
	     */
	    uint32_t iv_length;
	    /**
	     * @brief Length of the encryption/decryption key
	     *
	     * Length of the encryption/decryption key as expected by
	     * the cipher implementation in bytes.
	     */
	    uint32_t key_length;
	    /**
	     * @brief the key used for encryption/decryption
	     *
	     * The key used for encryption/decryption.
	     */
	    Key key;

	public:
	    //! Constructor
	    Crypt(const Key& k) throw(YAPETException);
	    Crypt(const Crypt& c);
	    inline ~Crypt() {}

	    /**
	     * @brief Returns the length of the initialization vector
	     *
	     * Returns the length in bytes of the initialization
	     * vector the cipher algorithm expects
	     *
	     * @return the length of the initialization vector in
	     * bytes.
	     */
	    inline uint32_t getIVLength() const { return iv_length; }
	    /**
	     * @brief Returns the length of the key
	     *
	     * Returns the length in bytes of the key the cipher
	     * algorithm expects.
	     *
	     * @return the length of the key in bytes.
	     */
	    inline uint32_t getKeyLength() const { return key_length; }

	    /**
	     * @brief Encrypts data
	     *
	     * Encrypts the data provided in \c data which has to be
	     * an instance of the \c Record template.
	     *
	     * The encrypted data is returned as \c BDBuffer with the
	     * size set exactly to the size of the encrypted data. The
	     * memory occupied by this object has to be freed by the
	     * caller.
	     *
	     * @param data the data to be encrypted.
	     *
	     * @return pointer to a \c BDBuffer holding the encrypted
	     * data. The caller is responsible for freeing the memory
	     * occupied by the object returned.
	     *
	     * @throw YAPETException
	     *
	     * @throw YAPETEncryptionException
	     *
	     * @sa Record, BDBuffer
	     */
	    template<class T>
	    BDBuffer* encrypt(const Record<T>& data) 
		throw(YAPETException, YAPETEncryptionException) {
		if (key.ivec_size() != iv_length) 
		    throw YAPETException("IVec length missmatch");

		EVP_CIPHER_CTX ctx;
		EVP_CIPHER_CTX_init(&ctx);

		int retval = EVP_EncryptInit_ex(&ctx,
						cipher,
						NULL,
						key,
						key.getIVec());
		if (retval == 0) {
		    EVP_CIPHER_CTX_cleanup(&ctx);
		    throw YAPETEncryptionException("Error initializing encryption engine");
		}

		retval = EVP_CIPHER_CTX_set_key_length(&ctx, key.size());
		if (retval == 0) {
		    EVP_CIPHER_CTX_cleanup(&ctx);
		    throw YAPETException("Error setting the key length");
		}
		
		BDBuffer* encdata =
		    new BDBuffer(data.size() + EVP_MAX_BLOCK_LENGTH);
		int outlen;
		retval = EVP_EncryptUpdate(&ctx,
					   *encdata,
					   &outlen,
					   data,
					   data.size());
		if (retval == 0) {
		    EVP_CIPHER_CTX_cleanup(&ctx);
		    delete encdata;
		    throw YAPETEncryptionException("Error encrypting data");
		}
		
		int tmplen;
		retval = EVP_EncryptFinal_ex(&ctx,
					     encdata->at(outlen),
					     &tmplen);
		if (retval == 0) {
		    EVP_CIPHER_CTX_cleanup(&ctx);
		    delete encdata;
		    throw YAPETEncryptionException("Error finalizing encryption");
		}

		encdata->resize(outlen+tmplen);

		EVP_CIPHER_CTX_cleanup(&ctx);

		return encdata;
	    }

	    /**
	     * @brief Decrypts data
	     *
	     * Decrypts the data supplied in the \c BDBuffer. It
	     * returns the decrypted data as a \c Record of the
	     * specified type.
	     *
	     * The \c Record has to be freed by the caller.
	     *
	     * @param data the \c BDBuffer to decrypt.
	     *
	     * @return pointer to a \c Record of the specified type,
	     * holding the decrypted data. The caller is responsible
	     * for freeing the memory occupied by the object returned.
	     *
	     * @throw YAPETException
	     *
	     * @throw YAPETEncryptionException
	     *
	     * @sa Record, BDBuffer
	     */
	    template<class T>
	    Record<T>* decrypt(const BDBuffer& data)
		throw(YAPETException, YAPETEncryptionException) {
		if ( ((unsigned int)key.ivec_size()) != iv_length)
		    throw YAPETException("IVec length missmatch");

		EVP_CIPHER_CTX ctx;
		EVP_CIPHER_CTX_init(&ctx);

		int retval = EVP_DecryptInit_ex(&ctx,
						cipher,
						NULL,
						key,
						key.getIVec());
		if (retval == 0) {
		    EVP_CIPHER_CTX_cleanup(&ctx);
		    throw YAPETEncryptionException("Error initializing encryption engine");
		}

		retval = EVP_CIPHER_CTX_set_key_length(&ctx, key.size());
		if (retval == 0) {
		    EVP_CIPHER_CTX_cleanup(&ctx);
		    throw YAPETException("Error setting the key length");
		}

		BDBuffer* decdata = new BDBuffer(data.size());
		int outlen;
		retval = EVP_DecryptUpdate(&ctx,
					   *decdata,
					   &outlen,
					   data,
					   data.size());
		if (retval == 0) {
		    EVP_CIPHER_CTX_cleanup(&ctx);
		    delete decdata;
		    throw YAPETEncryptionException("Error decrypting data");
		}

		int tmplen;
		retval = EVP_DecryptFinal_ex(&ctx,
					     decdata->at(outlen),
					     &tmplen);
		if (retval == 0) {
		    EVP_CIPHER_CTX_cleanup(&ctx);
		    delete decdata;
		    throw YAPETEncryptionException("Error finalizing decryption");
		}
		
		decdata->resize(outlen+tmplen);
		
		EVP_CIPHER_CTX_cleanup(&ctx);

		Record<T>* r = new Record<T>;
		*r=*decdata;
		
		delete decdata;
		return r;
	    }

	    const Crypt& operator=(const Crypt& c);
    };
}

#endif // _CRYPT_H
