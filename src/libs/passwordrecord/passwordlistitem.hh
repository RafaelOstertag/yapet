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

#ifndef _PASSWORDLISTITEM_HH
#define _PASSWORDLISTITEM_HH

#include "passwordrecord.hh"

namespace yapet {
class PasswordListItem {
   private:
    SecureArray _name;
    SecureArray _encryptedRecord;

   public:
    using size_type = SecureArray::size_type;
    PasswordListItem();
    PasswordListItem(const char* name, const SecureArray& encryptedRecord);

    PasswordListItem(const PasswordListItem& item);
    PasswordListItem& operator=(const PasswordListItem& item);

    PasswordListItem(PasswordListItem&& item);
    PasswordListItem& operator=(PasswordListItem&& item);

    bool operator==(const PasswordListItem& other) const;
    bool operator!=(const PasswordListItem& other) const {
        return !operator==(other);
    }

    const std::uint8_t* name() const { return *_name; }
    SecureArray::size_type nameSize() const { return _name.size(); }
    const SecureArray& encryptedRecord() const { return _encryptedRecord; }

    operator std::string() const;
};

bool operator<(const PasswordListItem& a, const PasswordListItem& b);
bool operator>(const PasswordListItem& a, const PasswordListItem& b);

}  // namespace yapet
#endif