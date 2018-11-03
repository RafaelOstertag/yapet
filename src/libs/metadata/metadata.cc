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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cstdint>
#include <stdexcept>
#include <vector>
#ifdef DEBUG
#include <iomanip>
#include <sstream>
#endif

#include "consts.h"
#include "intl.h"
#include "logger.hh"
#include "metadata.hh"
#include "ods.hh"

using namespace yapet;

namespace {
#ifdef DEBUG
inline std::string intToHexString(int val) {
    std::stringstream hexString;
    hexString << "0x" << std::hex << val;
    return hexString.str();
}
#endif

std::string readKey(const SecureArray& serialized,
                    SecureArray::size_type& pos) {
    std::string str{};
    for (; serialized[pos] != '\0'; pos++) {
        str += static_cast<char>(serialized[pos]);
    }
    // advance to int position
    pos++;

    return str;
}

int readValue(const SecureArray& serialized, SecureArray::size_type& pos) {
    SecureArray::size_type end = pos + sizeof(int);

    int odsInt;
    std::uint8_t* ptr = reinterpret_cast<std::uint8_t*>(&odsInt);

    for (SecureArray::size_type i = 0; pos < end; i++, pos++) {
        ptr[i] = serialized[pos];
    }

    return toHost(odsInt);
}
}  // namespace
MetaData::MetaData() : _dataItems{} {}

MetaData::MetaData(const SecureArray& serialized) : _dataItems{} {
    if (serialized == SecureArray{}) {
        return;
    }

    try {
        for (SecureArray::size_type positionInSerialized = 0;
             positionInSerialized < serialized.size();) {
            std::string key = readKey(serialized, positionInSerialized);

            int value = readValue(serialized, positionInSerialized);
            _dataItems[key] = value;
        }
    } catch (...) {
        throw std::invalid_argument(_("Error deserializing meta data"));
    }

#ifdef DEBUG
    for (auto item : _dataItems) {
        LOG_MESSAGE("Deserialized " + item.first + ": " + intToHexString(item.second));
    }
#endif
}

MetaData::MetaData(const MetaData& other) : _dataItems{other._dataItems} {}

MetaData::MetaData(MetaData&& other) : _dataItems{move(other._dataItems)} {}

MetaData& MetaData::operator=(const MetaData& other) {
    if (this == &other) return *this;

    _dataItems = other._dataItems;

    return *this;
}

MetaData& MetaData::operator=(MetaData&& other) {
    if (this == &other) return *this;

    _dataItems = move(other._dataItems);

    return *this;
}

int MetaData::getValue(const std::string& key) const {
    return _dataItems.at(key);
}

void MetaData::setValue(const std::string& key, int value) {
    _dataItems[key] = value;
}

std::unordered_map<std::string, int>::size_type MetaData::size() const {
    return _dataItems.size();
}

namespace {
void addStringToBuffer(std::vector<std::uint8_t>& buffer,
                       const std::string& str) {
    for (char c : str) {
        buffer.push_back(static_cast<std::uint8_t>(c));
    }
    buffer.push_back(0);
}

void addIntToBuffer(std::vector<std::uint8_t>& buffer, const int& i) {
    int odsInt{toODS(i)};
    const std::uint8_t* ptr = reinterpret_cast<const std::uint8_t*>(&odsInt);
    for (std::vector<std::uint8_t>::size_type i = 0; i < sizeof(int); i++) {
        buffer.push_back(ptr[i]);
    }
}
}  // namespace

SecureArray MetaData::serialize() const {
    std::vector<std::uint8_t> buffer{};
    buffer.reserve(1024);

    for (auto item : _dataItems) {
        std::string key = item.first;
        addStringToBuffer(buffer, key);

        int value = item.second;
        addIntToBuffer(buffer, value);

        LOG_MESSAGE("Serialized " + item.first + ": " + intToHexString(item.second));
    }

    SecureArray::size_type bufferSize{
        static_cast<SecureArray::size_type>(buffer.size())};

    SecureArray serialized{bufferSize};
    for (SecureArray::size_type i = 0; i < bufferSize; i++) {
        (*serialized)[i] = buffer[i];
    }

    return serialized;
}