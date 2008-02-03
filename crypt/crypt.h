// -*- c++ -*-
//
// $Id$

#ifndef _CRYPT_H
#define _CRYPT_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#include "gpsexception.h"
#include "key.h"
#include "bdbuffer.h"
#include "record.h"

namespace GPSAFE {
    class Crypt {
	private:
	    const EVP_CIPHER* cipher;
	    unsigned int iv_length;
	    unsigned int key_length;
	    Key key;

	public:
	    Crypt(const Key& k) throw(GPSException);
	    Crypt(const Crypt& c);
	    inline ~Crypt() {}

	    inline unsigned int getMaxIVLength() const { return iv_length; }
	    inline unsigned int getMaxKeyLength() const { return key_length; }

	    template<class T>
	    BDBuffer* encrypt(const Record<T>& data) throw(GPSException, GPSEncryptionException) {
		if (key.ivec_size() != iv_length) throw GPSException("IVec length missmatch");

		EVP_CIPHER_CTX ctx;
		EVP_CIPHER_CTX_init(&ctx);

		int retval = EVP_EncryptInit_ex(&ctx, cipher, NULL, key, key.getIVec());
		if (retval == 0) {
		    EVP_CIPHER_CTX_cleanup(&ctx);
		    throw GPSEncryptionException("Error initializing encryption engine");
		}

		retval = EVP_CIPHER_CTX_set_key_length(&ctx, key.size());
		if (retval == 0) {
		    EVP_CIPHER_CTX_cleanup(&ctx);
		    throw GPSException("Error setting the key length");
		}
		
		BDBuffer* encdata = new BDBuffer(data.size() + EVP_MAX_BLOCK_LENGTH);
		int outlen;
		retval = EVP_EncryptUpdate(&ctx, *encdata, &outlen, data, data.size());
		if (retval == 0) {
		    EVP_CIPHER_CTX_cleanup(&ctx);
		    delete encdata;
		    throw GPSEncryptionException("Error encrypting data");
		}
		
		int tmplen;
		retval = EVP_EncryptFinal_ex(&ctx, encdata->at(outlen), &tmplen);
		if (retval == 0) {
		    EVP_CIPHER_CTX_cleanup(&ctx);
		    delete encdata;
		    throw GPSEncryptionException("Error finalizing encryption");
		}

		encdata->resize(outlen+tmplen);

		EVP_CIPHER_CTX_cleanup(&ctx);

		return encdata;
	    }

	    template<class T>
	    Record<T>* decrypt(const BDBuffer& data) throw(GPSException, GPSEncryptionException) {
		if ( ((unsigned int)key.ivec_size()) != iv_length) throw GPSException("IVec length missmatch");

		EVP_CIPHER_CTX ctx;
		EVP_CIPHER_CTX_init(&ctx);

		int retval = EVP_DecryptInit_ex(&ctx, cipher, NULL, key, key.getIVec());
		if (retval == 0) {
		    EVP_CIPHER_CTX_cleanup(&ctx);
		    throw GPSEncryptionException("Error initializing encryption engine");
		}

		retval = EVP_CIPHER_CTX_set_key_length(&ctx, key.size());
		if (retval == 0) {
		    EVP_CIPHER_CTX_cleanup(&ctx);
		    throw GPSException("Error setting the key length");
		}

		BDBuffer* decdata = new BDBuffer(data.size());
		int outlen;
		retval = EVP_DecryptUpdate(&ctx, *decdata, &outlen, data, data.size());
		if (retval == 0) {
		    EVP_CIPHER_CTX_cleanup(&ctx);
		    delete decdata;
		    throw GPSEncryptionException("Error decrypting data");
		}

		int tmplen;
		retval = EVP_DecryptFinal_ex(&ctx, decdata->at(outlen), &tmplen);
		if (retval == 0) {
		    EVP_CIPHER_CTX_cleanup(&ctx);
		    delete decdata;
		    throw GPSEncryptionException("Error finalizing decryption");
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
