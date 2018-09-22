
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
    if (index >= _size || index < 0) {
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("Index out of range: %d"), index);
        throw std::out_of_range{msg};
    }

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
