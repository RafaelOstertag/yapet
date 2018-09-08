#ifndef _KEY_HH
#define _KEY_HH

#include "securearray.hh"
namespace yapet {
/**
 * Interface for encryption/decryption keys
 */
class Key {
   public:
    virtual ~Key() {}
    /**
     * The password from which the key is derived.
     *
     * The password provided in \c password is expected to be terminated by \c
     * \0.
     *
     * @param password \c SecureArray containing password terminated by \c \0.
     */
    virtual void password(const SecureArray& password) = 0;

    virtual SecureArray key() const = 0;
    virtual SecureArray::size_type keySize() const = 0;

    virtual SecureArray ivec() const = 0;
    virtual SecureArray::size_type ivecSize() const = 0;

    virtual bool operator==(const Key& k) const = 0;
    //! Compares two keys for inequality
    virtual bool operator!=(const Key& k) const = 0;
};
}  // namespace yapet
#endif