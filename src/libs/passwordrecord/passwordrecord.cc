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

#include "passwordrecord.hh"

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <algorithm>
#include <cstdio>
#include <cstring>

#include "consts.h"
#include "intl.h"

using namespace yapet;

namespace {
inline void copyUint8(std::uint8_t* dest, const std::uint8_t* src, int srcLen,
                      int maxLen) {
    assert(dest != nullptr);
    assert(src != nullptr);

    auto len = std::min(srcLen, maxLen);
    std::memcpy(dest, src, len);
    dest[maxLen - 1] = '\0';
}

inline void copyString(std::uint8_t* dest, const char* src, size_t maxLen) {
    assert(dest != nullptr);
    assert(src != nullptr);

    copyUint8(dest, reinterpret_cast<const std::uint8_t*>(src),
              std::strlen(src) + 1, maxLen);
}

}  // namespace

PasswordRecord::PasswordRecord()
    : _name{NAME_SIZE},
      _host{HOST_SIZE},
      _username{USERNAME_SIZE},
      _password{PASSWORD_SIZE},
      _comment{COMMENT_SIZE} {}

PasswordRecord::PasswordRecord(const SecureArray& serialized)
    : _name{NAME_SIZE},
      _host{HOST_SIZE},
      _username{USERNAME_SIZE},
      _password{PASSWORD_SIZE},
      _comment{COMMENT_SIZE} {
    if (serialized.size() != TOTAL_SIZE) {
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("Password record is expected to be of size %d bytes "
                        "but got %d bytes"),
                      TOTAL_SIZE, serialized.size());

        throw DeserializationError{msg};
    }

    auto positionInBuffer = 0;

    std::memcpy(*_name, *serialized + positionInBuffer, NAME_SIZE);
    positionInBuffer += NAME_SIZE;

    std::memcpy(*_host, *serialized + positionInBuffer, HOST_SIZE);
    positionInBuffer += HOST_SIZE;

    std::memcpy(*_username, *serialized + positionInBuffer, USERNAME_SIZE);
    positionInBuffer += USERNAME_SIZE;

    std::memcpy(*_password, *serialized + positionInBuffer, PASSWORD_SIZE);
    positionInBuffer += PASSWORD_SIZE;

    std::memcpy(*_comment, *serialized + positionInBuffer, COMMENT_SIZE);
}

PasswordRecord::PasswordRecord(const PasswordRecord& p)
    : _name{p._name},
      _host{p._host},
      _username{p._username},
      _password{p._password},
      _comment{p._comment} {}

PasswordRecord::PasswordRecord(PasswordRecord&& p)
    : _name{std::move(p._name)},
      _host{std::move(p._host)},
      _username{std::move(p._username)},
      _password{std::move(p._password)},
      _comment{std::move(p._comment)} {}

PasswordRecord& PasswordRecord::operator=(const PasswordRecord& p) {
    if (&p == this) {
        return *this;
    }

    _name = p._name;
    _host = p._host;
    _username = p._username;
    _password = p._password;
    _comment = p._comment;

    return *this;
}

PasswordRecord& PasswordRecord::operator=(PasswordRecord&& p) {
    if (&p == this) {
        return *this;
    }

    _name = std::move(p._name);
    _host = std::move(p._host);
    _username = std::move(p._username);
    _password = std::move(p._password);
    _comment = std::move(p._comment);

    return *this;
}

SecureArray PasswordRecord::serialize() const {
    SecureArray serialized{TOTAL_SIZE};

    auto positionInBuffer = 0;

    std::memcpy(*serialized + positionInBuffer, *_name, NAME_SIZE);
    positionInBuffer += NAME_SIZE;

    std::memcpy(*serialized + positionInBuffer, *_host, HOST_SIZE);
    positionInBuffer += HOST_SIZE;

    std::memcpy(*serialized + positionInBuffer, *_username, USERNAME_SIZE);
    positionInBuffer += USERNAME_SIZE;

    std::memcpy(*serialized + positionInBuffer, *_password, PASSWORD_SIZE);
    positionInBuffer += PASSWORD_SIZE;

    std::memcpy(*serialized + positionInBuffer, *_comment, COMMENT_SIZE);
    positionInBuffer += COMMENT_SIZE;

    return serialized;
}

void PasswordRecord::name(const char* name) {
    copyString(*_name, name, NAME_SIZE);
}
void PasswordRecord::name(const std::uint8_t* name, int l) {
    copyUint8(*_name, name, l, NAME_SIZE);
}

void PasswordRecord::host(const char* host) {
    copyString(*_host, host, HOST_SIZE);
}
void PasswordRecord::host(const std::uint8_t* host, int l) {
    copyUint8(*_host, host, l, HOST_SIZE);
}

void PasswordRecord::username(const char* username) {
    copyString(*_username, username, USERNAME_SIZE);
}
void PasswordRecord::username(const std::uint8_t* username, int l) {
    copyUint8(*_username, username, l, USERNAME_SIZE);
}

void PasswordRecord::password(const char* password) {
    copyString(*_password, password, PASSWORD_SIZE);
}
void PasswordRecord::password(const std::uint8_t* password, int l) {
    copyUint8(*_password, password, l, PASSWORD_SIZE);
}

void PasswordRecord::comment(const char* comment) {
    copyString(*_comment, comment, COMMENT_SIZE);
}
void PasswordRecord::comment(const std::uint8_t* comment, int l) {
    copyUint8(*_comment, comment, l, COMMENT_SIZE);
}