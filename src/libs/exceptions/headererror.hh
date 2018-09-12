#ifndef _HEADERERROR_HH
#define _HEADERERROR_HH

#include "baseerror.hh"

namespace yapet {
class HeaderError : public YAPETBaseError {
   public:
    HeaderError(const char* msg) : YAPETBaseError{msg} {}
};
}  // namespace yapet

#endif