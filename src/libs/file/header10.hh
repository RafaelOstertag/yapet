#ifndef _HEADER10_HH
#define _HEADER10_HH

#include <cstdint>
#include <cstring>
#include <ctime>

#include "headerversion.hh"
#include "ods.hh"
#include "securearray.hh"
#include "serializable.hh"

namespace yapet {
/**
 * YAPET header of YAPET 1.0 files.
 *
 * Supports only unencrypted data.
 */
class Header10 : public Serializable {
   private:
    std::uint8_t _version;
    std::uint64_t _passwordSetTime;

    static constexpr std::uint8_t CONTROL_STRING[]{
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
        'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T'};
    static constexpr std::uint8_t CONTROL_STRING_SIZE{sizeof(CONTROL_STRING)};
    static constexpr std::uint8_t VERSION_SIZE{sizeof(_version)};
    static constexpr std::uint8_t PASSWORD_SET_TIME_SIZE{
        sizeof(_passwordSetTime)};

    static constexpr std::uint8_t TOTAL_HEADER_SIZE_VERSION_2{
        VERSION_SIZE + PASSWORD_SET_TIME_SIZE + CONTROL_STRING_SIZE};
    static constexpr std::uint8_t TOTAL_HEADER_SIZE_VERSION_1{
        VERSION_SIZE + sizeof(std::uint32_t) + CONTROL_STRING_SIZE};

    template <class T>
    T unserializePasswordSetTime(const SecureArray& serializedHeader) {
        T passwordSetTimeInODSFormat;
        auto pointerToPasswordSetTime =
            (*serializedHeader) + VERSION_SIZE + CONTROL_STRING_SIZE;

        std::memcpy(&passwordSetTimeInODSFormat, pointerToPasswordSetTime,
                    sizeof(T));

        return toHost(passwordSetTimeInODSFormat);
    }

    void testIfControlStringMatchesOrThrow(
        const SecureArray& serializedHeader) const;
    void deserializeVersion2Header(const SecureArray& serializedHeader);
    void deserializeVersion1Header(const SecureArray& serializedHeader);

   public:
    Header10(std::uint64_t passwordSetTime = std::time(0),
             std::uint8_t version = VERSION_2)
        : _version{version}, _passwordSetTime{passwordSetTime} {};

    Header10(const SecureArray& serializedHeader);
    virtual ~Header10() {}

    Header10& operator=(const SecureArray& serializedHeader);
    std::uint64_t passwordSetTime() const { return _passwordSetTime; }
    void passwordSetTime(std::uint64_t passwordSetTime) {
        _passwordSetTime = passwordSetTime;
    }

    std::uint8_t version() const { return _version; }

    operator SecureArray() const;
    SecureArray serialize() const;
};
}  // namespace yapet

#endif
