#ifndef _HEADERVERSION_HH
#define _HEADERVERSION_HH

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "headererror.hh"
#include "intl.h"

namespace yapet {
enum HEADER_VERSION {
    /**
     * The header version for pre version 0.6.
     */
    VERSION_1 = 1,
    /**
     * The header version for version 0.6.
     */
    VERSION_2 = 2
};

inline HEADER_VERSION intToHeaderVersion(int version) {
    switch (version) {
        case 1:
            return VERSION_1;
        case 2:
            return VERSION_2;
        default:
            std::string msg = std::to_string(version);
            msg += _(" is not a known header version");
            throw HeaderError(msg.c_str());
    }
}
}  // namespace yapet

#endif