#ifndef _CRYPTOERROR_HH
#define _CRYPTOERROR_HH

#include "baseerror.hh"

namespace yapet {

class CipherError : public YAPETBaseError {
   public:
    CipherError() noexcept : YAPETBaseError{_("Cipher error")} {}
    CipherError(const std::string& msg) noexcept
        : YAPETBaseError{msg.c_str()} {}
};

/**
 * @brief Indicates an error while encrypting/decrypting.
 *
 * This function indicates an error while encrypting/decrypting
 * data.
 */
class EncryptionError : public YAPETBaseError {
   public:
    EncryptionError() noexcept
        : YAPETBaseError{_("Encryption error")} {}
    EncryptionError(const std::string& msg) noexcept
        : YAPETBaseError{msg.c_str()} {}
};
};  // namespace yapet

#endif