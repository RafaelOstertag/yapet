#ifndef _YAPETERROR_HH
#define _YAPETERROR_HH

#include "baseerror.hh"

namespace yapet {

/**
 * @brief Exception indicating that an operation may be retried
 *
 * This exception indicates that an operation may be retried. The
 * \c File class uses this exception for certain methods.
 *
 * @sa File
 */
class RetryableError : public YAPETBaseError {
   public:
    RetryableError() noexcept : YAPETBaseError{_("Retry")} {}
    RetryableError(const std::string& msg) noexcept
        : YAPETBaseError{msg.c_str()} {}
};

/**
 * @brief Indicates a wrong password.
 *
 * Indicates that a wrong password, thus key, was used for
 * decryption of a file.
 *
 * @sa File
 */
class InvalidPasswordError : public YAPETBaseError {
   public:
    InvalidPasswordError() noexcept
        : YAPETBaseError{_("Invalid password")} {}
    InvalidPasswordError(const std::string& msg) noexcept
        : YAPETBaseError{msg.c_str()} {}
};
}  // namespace yapet

#endif