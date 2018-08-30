#ifndef _HEADERERROR_HH
#define _HEADERERROR_HH

#include <stdexcept>

namespace yapet {
class HeaderError : public std::runtime_error {
   public:
    HeaderError(const char* msg) : runtime_error{msg} {}
};

class InvalidPasswordError : public std::runtime_error {
   public:
    InvalidPasswordError(const char* msg) : runtime_error{msg} {};
};
}  // namespace yapet

#endif