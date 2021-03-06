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
#include <cstring>
#include <limits>
#include <stdexcept>

#include "consts.h"
#include "intl.h"
#include "securearray.hh"

using namespace yapet;

namespace {
inline SecureArray::size_type castToSizeTypeOrThrow(size_t otherSize) {
    int maxSize = std::numeric_limits<SecureArray::size_type>::max();
    size_t castedMaxSize = static_cast<size_t>(maxSize);
    if (otherSize > castedMaxSize) {
        throw std::invalid_argument(
            _("Provided size exceeds maximum size of SecureArray"));
    }

    return static_cast<SecureArray::size_type>(otherSize);
}
}  // namespace

void SecureArray::indexInRangeOrThrow(size_type index) const {
    if (index >= _size || index < 0) {
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("Index out of range: %d"), index);
        throw std::out_of_range{msg};
    }
}

SecureArray::SecureArray(size_type size) : _size{size}, _array{nullptr} {
    if (_size < 0) {
        throw std::invalid_argument{_("Size must not be negative")};
    }

    if (_size != 0) {
        _array = new std::uint8_t[_size];
    }
}

SecureArray::~SecureArray() {
    clearMemory();
    freeMemory();
}

SecureArray::SecureArray(const SecureArray& other)
    : _size{other._size}, _array{nullptr} {
    if (_size != 0) {
        _array = new std::uint8_t[_size];
        std::memcpy(_array, other._array, _size);
    }
}

SecureArray& SecureArray::operator=(const SecureArray& other) {
    if (this == &other) {
        return *this;
    }

    clearMemory();
    freeMemory();

    _size = other._size;
    if (_size == 0) {
        _array = other._array;
    } else {
        _array = new std::uint8_t[_size];
        std::memcpy(_array, other._array, _size);
    }

    return *this;
}

SecureArray::SecureArray(SecureArray&& other)
    : _size{other._size}, _array{other._array} {
    other._array = nullptr;
    other._size = 0;
}

SecureArray& SecureArray::operator=(SecureArray&& other) {
    if (this == &other) {
        return *this;
    }

    clearMemory();
    freeMemory();

    _size = other._size;
    _array = other._array;
    other._array = nullptr;
    other._size = 0;

    return *this;
}

inline void SecureArray::clearMemory() {
    if (_array == nullptr) return;

    std::memset(_array, 0, _size);
}

inline void SecureArray::freeMemory() {
    if (_array == nullptr) return;
    delete[] _array;
}

const std::uint8_t* SecureArray::operator*() const { return _array; }

std::uint8_t* SecureArray::operator*() { return _array; }

std::uint8_t SecureArray::operator[](size_type index) const {
    indexInRangeOrThrow(index);
    return _array[index];
}

std::uint8_t& SecureArray::operator[](size_type index) {
    indexInRangeOrThrow(index);
    return _array[index];
}

bool SecureArray::operator==(const SecureArray& other) const {
    if (other._size != _size) return false;

    if (other._array == nullptr && _array == nullptr) return true;

    return std::memcmp(_array, other._array, _size) == 0;
}

SecureArray& SecureArray::operator<<(const SecureArray& source) {
    if (this == &source) return *this;

    if (source._size == 0 || _size == 0) {
        return *this;
    }

    if (source._size > _size) {
        std::memcpy(_array, source._array, _size);
        return *this;
    }

    clearMemory();
    freeMemory();

    _array = new std::uint8_t[source._size];
    _size = source._size;
    std::memcpy(_array, source._array, _size);

    return *this;
}

SecureArray yapet::toSecureArray(const char* str) {
    auto len = std::strlen(str) + 1;
    if (len == 0) {
        return SecureArray{};
    }

    auto castedSize = castToSizeTypeOrThrow(len);

    SecureArray result{castedSize};
    std::memcpy(*result, str, len);

    return result;
}

SecureArray yapet::toSecureArray(const std::string& str) {
    return toSecureArray(str.c_str());
}

SecureArray yapet::toSecureArray(const std::uint8_t* ptr,
                                 SecureArray::size_type size) {
    if (ptr == nullptr) {
        throw std::invalid_argument(_("Pointer must not be null"));
    }

    SecureArray secureArray{size};
    std::memcpy(*secureArray, ptr, size);

    return secureArray;
}

SecureArray yapet::operator+(const SecureArray& a, const SecureArray& b) {
    auto aSize = a.size();
    auto bSize = b.size();
    SecureArray result{aSize + bSize};

    std::memcpy(*result, *a, aSize);
    auto ptrToBDest = *result + aSize;
    std::memcpy(ptrToBDest, *b, bSize);

    return result;
}
