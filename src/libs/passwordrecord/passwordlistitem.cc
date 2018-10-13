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

#include <algorithm>
#include <cstring>

#include "passwordlistitem.hh"

using namespace yapet;

PasswordListItem::PasswordListItem() : _name{}, _encryptedRecord{} {}

PasswordListItem::PasswordListItem(const char* host,
                                   const SecureArray& encryptedRecord)
    : _name{PasswordRecord::NAME_SIZE}, _encryptedRecord{encryptedRecord} {
    auto stringLengthIncludingZero = std::strlen(host) + 1;
    auto len = stringLengthIncludingZero > PasswordRecord::NAME_SIZE
                   ? PasswordRecord::NAME_SIZE
                   : stringLengthIncludingZero;

    std::memcpy(*_name, host, len);
    (*_name)[PasswordRecord::NAME_SIZE - 1] = '\0';
}

PasswordListItem::PasswordListItem(const PasswordListItem& item)
    : _name{item._name}, _encryptedRecord{item._encryptedRecord} {}
PasswordListItem& PasswordListItem::operator=(const PasswordListItem& item) {
    if (&item == this) {
        return *this;
    }

    _name = item._name;
    _encryptedRecord = item._encryptedRecord;

    return *this;
}

PasswordListItem::PasswordListItem(PasswordListItem&& item)
    : _name{std::move(item._name)},
      _encryptedRecord{std::move(item._encryptedRecord)} {}
PasswordListItem& PasswordListItem::operator=(PasswordListItem&& item) {
    if (&item == this) {
        return *this;
    }

    _name = std::move(item._name);
    _encryptedRecord = std::move(item._encryptedRecord);

    return *this;
}

bool PasswordListItem::operator==(const PasswordListItem& other) const {
    return std::strncmp(reinterpret_cast<const char*>(*_name),
                        reinterpret_cast<const char*>(*other._name),
                        _name.size()) == 0;
}

PasswordListItem::operator std::string() const {
    return std::string{reinterpret_cast<const char*>(*_name)};
}

bool yapet::operator<(const PasswordListItem& a, const PasswordListItem& b) {
    const char* aName = reinterpret_cast<const char*>(a.name());
    const char* bName = reinterpret_cast<const char*>(b.name());

    auto result{
        std::strncmp(aName, bName, std::min(a.nameSize(), b.nameSize()))};

    return result < 0;
}

bool yapet::operator>(const PasswordListItem& a, const PasswordListItem& b) {
    const char* aName = reinterpret_cast<const char*>(a.name());
    const char* bName = reinterpret_cast<const char*>(b.name());

    auto result{
        std::strncmp(aName, bName, std::min(a.nameSize(), b.nameSize()))};

    return result > 0;
}