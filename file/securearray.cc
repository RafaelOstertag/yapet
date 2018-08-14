
#include <stdexcept>

#include "intl.h"
#include "securearray.hh"

using namespace yapet;

SecureArray::SecureArray(size_t s) : _size{s} {
    if (_size < 1) {
        throw std::invalid_argument(_("Size must be greater than zero"));
    }

    array = new std::uint8_t[_size];
}

SecureArray::~SecureArray() {
    // move constructor may set it to nullptr
    if (array == nullptr) {
        return;
    }

    clearMemory();
    freeMemory();
}

SecureArray::SecureArray(const SecureArray& other)
    : _size{other._size}, array{new std::uint8_t[other._size]} {
    for (size_t i = 0; i < _size; array[i] = other.array[i], i++)
        ;
}

SecureArray& SecureArray::operator=(const SecureArray& other) {
    if (this == &other) {
        return *this;
    }

    clearMemory();
    freeMemory();

    _size = other._size;

    array = new std::uint8_t[_size];
    for (size_t i = 0; i < _size; array[i] = other.array[i], i++)
        ;

    return *this;
}

SecureArray::SecureArray(SecureArray&& other)
    : _size{other._size}, array{other.array} {
    other.array = nullptr;
}

SecureArray& SecureArray::operator=(SecureArray&& other) {
    if (this == &other) {
        return *this;
    }

    clearMemory();
    freeMemory();

    _size = other._size;
    array = other.array;
    other.array = nullptr;

    return *this;
}

const std::uint8_t* SecureArray::operator*() const { return array; }

std::uint8_t* SecureArray::operator*() { return array; }

bool SecureArray::operator==(const SecureArray& other) const {
    if (other._size != _size) return false;

    for (size_t i = 0; i < _size; i++) {
        if (array[i] != other.array[i]) return false;
    }

    return true;
}
