
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
        for (size_t i = 0; i < _size; _array[i] = other._array[i], i++)
            ;
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
        for (size_t i = 0; i < _size; _array[i] = other._array[i], i++)
            ;
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

    for (size_t i = 0; i < _size; _array[i] = 0, i++)
        ;
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

    for (size_t i = 0; i < _size; i++) {
        if (_array[i] != other._array[i]) return false;
    }

    return true;
}
