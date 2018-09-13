#ifndef _HEADERVERSION_HH
#define _HEADERVERSION_HH

#include <cstdio>
#include <string>

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "consts.h"
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
            char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
            std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                          _("%d is not a known header version"), version);
            throw HeaderError(msg);
    }
}
}  // namespace yapet

#endif