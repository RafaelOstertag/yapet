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

#include <cstdio>

#include "consts.h"
#include "header10.hh"
#include "headererror.hh"
#include "intl.h"
#include "ods.hh"

using namespace yapet;

constexpr std::uint8_t Header10::CONTROL_STRING[];

void Header10::testIfControlStringMatchesOrThrow(
    const SecureArray& serializedHeader) const {
    auto pointerToControlString = (*serializedHeader) + VERSION_SIZE;
    auto result = std::memcmp(pointerToControlString, CONTROL_STRING,
                              CONTROL_STRING_SIZE);
    if (result != 0) {
        throw HeaderError{_("Control string does not match")};
    }
}

void Header10::deserializeVersion1Header(const SecureArray& serializedHeader) {
    // We have to test the controll string before testing the version in order
    // to raise an invalid password error. If we test the version before the
    // control string we don't know whether the password is wrong or the version
    // is wrong.
    testIfControlStringMatchesOrThrow(serializedHeader);

    _version = **serializedHeader;
    if (_version != VERSION_1) {
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("Expected header version 1 got %d"), _version);
        throw HeaderError{msg};
    }

    _passwordSetTime =
        unserializePasswordSetTime<std::uint32_t>(serializedHeader);
}

void Header10::deserializeVersion2Header(const SecureArray& serializedHeader) {
    // We have to test the control string before testing the version in order
    // to raise an invalid password error. If we test the version before the
    // control string we don't know whether the password is wrong or the version
    // is wrong.
    testIfControlStringMatchesOrThrow(serializedHeader);

    _version = **serializedHeader;

    // Pre 0.6 headers (version 1) created on certain 64bit machines use 64bits
    // time_t types and thus are identified as as version 2 headers by taking
    // the size into consideration only
    if ((_version != VERSION_2) && (_version != VERSION_1)) {
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("Expected header version 1 or 2, got %d"), _version);
        throw HeaderError(msg);
    }

    _passwordSetTime =
        unserializePasswordSetTime<std::uint64_t>(serializedHeader);
}

void Header10::deserializeHeader(const SecureArray& serializedHeader) {
    auto headerSize{serializedHeader.size()};

    // The header size of headers create by previous version may be bigger than
    // the exact total byte size, since previous versions encrypted the entire
    // struct{} instead of the serialized struct{}
    if (headerSize >= TOTAL_HEADER_SIZE_VERSION_2) {
        deserializeVersion2Header(serializedHeader);
    } else if ((headerSize >= TOTAL_HEADER_SIZE_VERSION_1) &&
               (headerSize < TOTAL_HEADER_SIZE_VERSION_2)) {
        deserializeVersion1Header(serializedHeader);
    } else {
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("Invalid header size %d"), headerSize);
        throw HeaderError{msg};
    }
}

Header10::Header10(const SecureArray& serializedHeader) {
    deserializeHeader(serializedHeader);
}

Header10& Header10::operator=(const SecureArray& serializedHeader) {
    deserializeHeader(serializedHeader);
    return *this;
}

SecureArray Header10::serialize() const {
    // Copy version to SecureArray
    SecureArray serializedHeader{TOTAL_HEADER_SIZE_VERSION_2};
    **serializedHeader = _version;

    // Copy control string to SecureArray
    auto pointerToControlString = (*serializedHeader) + VERSION_SIZE;
    std::memcpy(pointerToControlString, CONTROL_STRING, CONTROL_STRING_SIZE);

    // Copy password modification time to SecureArray
    auto passwordSetTimeInODSFormat = toODS(_passwordSetTime);
    auto pointerToSetTime =
        (*serializedHeader) + VERSION_SIZE + CONTROL_STRING_SIZE;
    std::memcpy(pointerToSetTime, &passwordSetTimeInODSFormat,
                PASSWORD_SET_TIME_SIZE);

    return serializedHeader;
}