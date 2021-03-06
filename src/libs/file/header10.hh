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
    std::int64_t _passwordSetTime;

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
    void deserializeHeader(const SecureArray& serializedHeader);

   public:
    Header10(std::int64_t passwordSetTime = std::time(0),
             std::uint8_t version = VERSION_2)
        : _version{version}, _passwordSetTime{passwordSetTime} {};

    Header10(const SecureArray& serializedHeader);
    virtual ~Header10() {}

    Header10& operator=(const SecureArray& serializedHeader);
    std::int64_t passwordSetTime() const { return _passwordSetTime; }
    void passwordSetTime(std::uint64_t passwordSetTime) {
        _passwordSetTime = passwordSetTime;
    }

    std::uint8_t version() const { return _version; }

    operator SecureArray() const;
    SecureArray serialize() const;
};
}  // namespace yapet

#endif
