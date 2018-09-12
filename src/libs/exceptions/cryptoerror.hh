#ifndef _CRYPTOERROR_HH
#define _CRYPTOERROR_HH

#include "baseerror.hh"

namespace yapet {

class CipherError : public YAPETBaseError {
   public:
    CipherError() noexcept : YAPETBaseError{_("Cipher error")} {}
    CipherError(const char* msg) noexcept : YAPETBaseError{msg} {}
};

/**
 * @brief Indicates an error while encrypting/decrypting.
 *
 * This function indicates an error while encrypting/decrypting
 * data.
 */
class EncryptionError : public YAPETBaseError {
   public:
    EncryptionError() noexcept : YAPETBaseError{_("Encryption error")} {}
    EncryptionError(const char* msg) noexcept : YAPETBaseError{msg} {}
};
}  // namespace yapet

#endif