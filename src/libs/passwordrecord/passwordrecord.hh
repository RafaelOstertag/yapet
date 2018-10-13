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

#ifndef _PASSWORDRECORD_HH
#define _PASSWORDRECORD_HH

#include <stdexcept>

#include "securearray.hh"
#include "serializable.hh"

namespace yapet {
class PasswordRecord : public Serializable {
   public:
    /**
     * The maximum length of the name string of a password record.
     */
    static constexpr auto NAME_SIZE = 128;
    /**
     * The maximum length of the host string of a password
     * records.
     */
    static constexpr auto HOST_SIZE = 256;
    /**
     * The maximum length of the user name string of a password
     * record.
     */
    static constexpr auto USERNAME_SIZE = 256;
    /**
     * The maximum length of the password string of a password
     * record.
     */
    static constexpr auto PASSWORD_SIZE = 256;
    /**
     * The maximum length of the comment string of a password
     * record.
     */
    static constexpr auto COMMENT_SIZE = 512;

    static constexpr auto TOTAL_SIZE =
        NAME_SIZE + HOST_SIZE + USERNAME_SIZE + PASSWORD_SIZE + COMMENT_SIZE;

   private:
    SecureArray _name;
    SecureArray _host;
    SecureArray _username;
    SecureArray _password;
    SecureArray _comment;

   public:
    PasswordRecord();
    PasswordRecord(const SecureArray& serialized);
    virtual ~PasswordRecord(){};

    PasswordRecord(const PasswordRecord& p);
    PasswordRecord(PasswordRecord&& p);

    PasswordRecord& operator=(const PasswordRecord& p);
    PasswordRecord& operator=(PasswordRecord&& p);

    virtual SecureArray serialize() const;

    const std::uint8_t* name() const { return *_name; }
    const std::uint8_t* host() const { return *_host; }
    const std::uint8_t* username() const { return *_username; }
    const std::uint8_t* password() const { return *_password; }
    const std::uint8_t* comment() const { return *_comment; }

    void name(const char* name);
    void name(const std::uint8_t* name, int l);

    void host(const char* host);
    void host(const std::uint8_t* host, int l);

    void username(const char* username);
    void username(const std::uint8_t* username, int l);

    void password(const char* password);
    void password(const std::uint8_t* password, int l);

    void comment(const char* comment);
    void comment(const std::uint8_t* comment, int l);
};

class DeserializationError : public std::runtime_error {
   public:
    DeserializationError(const char* msg) : runtime_error{msg} {}
};

}  // namespace yapet

#endif