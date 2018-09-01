
#include <cstring>
#include <stdexcept>

#include "intl.h"
#include "securearray.hh"

using namespace yapet;

SecureArray::SecureArray(size_type size) : _size{size}, _array{nullptr} {
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
    if (index >= _size) {
        throw std::out_of_range{_("Index out of range")};
    }

    return _array[index];
}

bool SecureArray::operator==(const SecureArray& other) const {
    if (other._size != _size) return false;

    return std::memcmp(_array, other._array, _size) == 0;
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

SecureArray yapet::toSecureArray(const char* str) {
    auto len = std::strlen(str);
    SecureArray result{len};
    std::memcpy(*result, str, len);

    return result;
}

SecureArray yapet::toSecureArray(const std::string& str) {
    SecureArray result{str.length()};
    std::memcpy(*result, str.c_str(), str.length());

    return result;
}
