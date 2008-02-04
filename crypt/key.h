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

/** Namespace for cryptographic stuff
 *
 * Namespace for cryptographic stuff. Has no front-end and relies on
 * openssl.
 */
namespace GPSAFE {
    /** Converts the password into the key
     *
     * Converts the password into the key which is used by the other classes.
     *
     * The key uses the maximum length of 448bits (56bytes) allowed
     * for blowfish.
     *
     * The key is computed using three passes. The first pass hashes
     * the password using the sha1 algorithm. This hash is then
     * re-hashed using md5 which is then appended to the key generated
     * by the previous pass (sha1 + md5). The last pass hashes the
     * result of the former two passes using RIPEMD-160 and appended
     * the result to the key (sha1 + md5 + ripemd-160).
     *
     * The initialization vector is computed by hashing the key using
     * the md5 algorithm and taking only the first eigth bytes.
     */
    class Key {
	private:
	    enum { 
		/** Length of the key
		 *
		 * The max length of the blowfish key in bytes (448 bits)
		 */
		KEYLENGTH = 56,
		/** Length of the md5 output
		 *
		 * The length of the output of md5 (128 bits)
		 */
		MD5_LEN = 16,

		/** Length of the sha1 output
		 *
		 * The length of the output of sha1 (160 bits)
		 */
		SHA1_LEN = 20,
		/** Length of the ripemd-160 output
		 *
		 * The lenght of the output of ripemd-160 (160 bits)
		 */
		RIPEMD160_LEN = 20,
		/** Length of the initialization vector
		 *
		 * The length of the initialization vector
		 */
		IVECLENGTH = 8
	    };
		
	    /** Holds the key
	     *
	     * This is the key used to encrypt and decrypt data.
	     */
	    uint8_t key[KEYLENGTH];
	    /** Holds the initialization vector
	     *
	     * The initialization vector used for encryption and
	     * decryption.
	     */
	    uint8_t IVec[IVECLENGTH];

	    //! Cleanup routine
	    void cleanup();

	public:
	    Key(const char* password) throw(GPSException);
	    Key(const Key& k);
	    ~Key();

	    //! Returns the key
	    inline const uint8_t* getKey(int& key_len) const {
		key_len = KEYLENGTH;
		return key;
	    }
	    //! Returns the key
	    inline const uint8_t* getKey() const { return key; }
	    //! Returns the initialization vector
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
