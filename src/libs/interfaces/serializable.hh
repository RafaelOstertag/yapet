#ifndef _SERIALIZEABLE_HH
#define _SERIALIZEABLE_HH

#include "securearray.hh"

namespace yapet {
class Serializable {
   public:
    virtual ~Serializable(){};
    virtual SecureArray serialize() const = 0;
};
}  // namespace yapet

#endif