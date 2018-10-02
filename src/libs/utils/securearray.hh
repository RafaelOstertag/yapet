#ifndef _SECUREARRAY_HH
#define _SECUREARRAY_HH

#include <cassert>
#include <cstdint>
#include <string>

namespace yapet {

/**
 * @brief Guarantee zeroing out of array
 *
 * Allocate an array of \c std::uint8_t on the heap and guarantee zeroing out
 * the contents of the array upon destruction
 */
class SecureArray {
   public:
    // Using int since SecureArray is most likely used in conjunction with
    // OpenSSL routines which use int
    using size_type = int;

   private:
    size_type _size;
    std::uint8_t* _array;

    void clearMemory();
    void freeMemory();
    void indexInRangeOrThrow(size_type index) const;

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
    std::uint8_t& operator[](size_type index);

    bool operator==(const SecureArray& other) const;

    inline bool operator!=(const SecureArray& other) const {
        return !this->operator==(other);
    };

    /**
     * Copy content of \c other to this.
     *
     * If this SecureArray is smaller than the source SecureArray, then only as
     * much data as fitting into this SecureArray are copied from the source
     * SecureArray.
     *
     * If the source SecureArray is smaller than this SecureArray, this
     * SecureArray is shrunk to the size of the source SecureArray.
     *
     * Copying from or to an empty SecureArray will have no effect.
     */
    SecureArray& operator<<(const SecureArray& source);

    size_type size() const { return _size; }
};

SecureArray operator+(const SecureArray& a, const SecureArray& b);

SecureArray toSecureArray(const char* str);
SecureArray toSecureArray(const std::string& str);
SecureArray toSecureArray(const std::uint8_t* ptr, SecureArray::size_type size);

}  // namespace yapet
#endif