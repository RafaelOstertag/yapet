/*
 * Copyright (C) 2018 Rafael Ostertag
 *
 * This file is part of YAPET.
 *
 * YAPET is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * YAPET is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * YAPET.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Additional permission under GNU GPL version 3 section 7
 *
 * If you modify this program, or any covered work, by linking or combining it
 * with the OpenSSL project's OpenSSL library (or a modified version of that
 * library), containing parts covered by the terms of the OpenSSL or SSLeay
 * licenses, Rafael Ostertag grants you additional permission to convey the
 * resulting work.  Corresponding Source for a non-source form of such a
 * combination shall include the source code for the parts of OpenSSL used as
 * well as that of the covered work.
 */

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