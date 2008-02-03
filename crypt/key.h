// -*- c++ -*-
//
// $Id$

#ifndef _KEY_H
#define _KEY_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


#include <openssl/evp.h>

#include "gpsexception.h"

namespace GPSAFE {

    class Key {
	private:
	    enum { 
		/**
		 * The max length of the blowfish key in bytes (448 bits)
		 */
		KEYLENGTH = 56,
		/**
		 * The length of the output of md5 (128 bits)
		 */
		MD5_LEN = 16,

		/**
		 * The length of the output of sha1 (160 bits)
		 */
		SHA1_LEN = 20,
		/**
		 * The length of the initialization vector
		 */
		IVECLENGTH = 8
	    };
		
	    uint8_t key[KEYLENGTH];
	    uint8_t IVec[IVECLENGTH];

	    void cleanup();

	public:
	    Key(const char* password) throw(GPSException);
	    Key(const Key& k);
	    ~Key();


	    inline const uint8_t* getKey(int& key_len) const {
		key_len = KEYLENGTH;
		return key;
	    }
	    inline const uint8_t* getKey() const { return key; }

	    inline const uint8_t* getIVec(int& ivec_len) const {
		ivec_len = IVECLENGTH;
		return IVec;
	    }
	    inline const uint8_t* getIVec() const { return IVec; }

	    inline int size() const { return KEYLENGTH; }
	    inline int ivec_size() const { return IVECLENGTH; }

	    inline const uint8_t* operator()(int& key_len) const { return getKey(key_len); }
	    inline const uint8_t* operator()() const { return key; }

	    inline operator uint8_t*() { return key; }
	    inline operator const uint8_t*() const { return key; }

	    const Key& operator=(const Key& k);
	    bool operator==(const Key& k) const;
	    bool operator!=(const Key& k) const { return !operator==(k); }
    };
	    
}

#endif // _KEY_H
