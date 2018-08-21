#ifndef _SECUREARRAY_HH
#define _SECUREARRAY_HH

#include <cassert>
#include <cstdint>

namespace yapet {

/**
 * @brief Guarantee zeroing out of array
 *
 * Allocate an array of \c std::uint8_t on the heap and guarantee zeroing out
 * the contents of the array upon destruction
 */
class SecureArray {
   public:
    using size_type = unsigned int;

   private:
    size_type _size;
    std::uint8_t* _array;

    void clearMemory();
    void freeMemory();

   public:
    SecureArray(size_type size = 0);
    ~SecureArray();

    SecureArray(const SecureArray&);
    SecureArray& operator=(const SecureArray&);

    SecureArray(SecureArray&& other);
    SecureArray& operator=(SecureArray&&);

    const std::uint8_t* operator*() const;
    std::uint8_t* operator*();
    std::uint8_t operator[](size_type index) const;

    bool operator==(const SecureArray& other) const;

    inline bool operator!=(const SecureArray& other) const {
        return !this->operator==(other);
    };

    size_type size() const { return _size; }
};
}  // namespace yapet
#endif