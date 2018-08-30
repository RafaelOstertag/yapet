#include "intl.h"

#include "header10.hh"
#include "headererror.hh"
#include "ods.hh"

using namespace yapet;

constexpr std::uint8_t Header10::CONTROL_STRING[];

void Header10::testIfControlStringMatchesOrThrow(
    const SecureArray& serializedHeader) const {
    auto pointerToControlString = (*serializedHeader) + VERSION_SIZE;
    auto result = std::memcmp(pointerToControlString, CONTROL_STRING,
                              CONTROL_STRING_SIZE);
    if (result != 0) {
        throw InvalidPasswordError(_("Invalid password"));
    }
}

void Header10::deserializeVersion1Header(const SecureArray& serializedHeader) {
    // We have to test the controll string before testing the version in order
    // to raise an invalid password error. If we test the version before the
    // control string we don't know whether the password is wrong or the version
    // is wrong.
    testIfControlStringMatchesOrThrow(serializedHeader);

    _version = **serializedHeader;
    if (_version != VERSION_1) {
        throw HeaderError(_("Expected header version 1"));
    }

    _passwordSetTime =
        unserializePasswordSetTime<std::uint32_t>(serializedHeader);
}

void Header10::deserializeVersion2Header(const SecureArray& serializedHeader) {
    // We have to test the controll string before testing the version in order
    // to raise an invalid password error. If we test the version before the
    // control string we don't know whether the password is wrong or the version
    // is wrong.
    testIfControlStringMatchesOrThrow(serializedHeader);

    _version = **serializedHeader;
    if (_version != VERSION_2) {
        throw HeaderError(_("Expected header version 2"));
    }

    _passwordSetTime =
        unserializePasswordSetTime<std::uint64_t>(serializedHeader);
}

Header10::Header10(const SecureArray& serializedHeader) {
    if (serializedHeader.size() == TOTAL_HEADER_SIZE_VERSION_2) {
        deserializeVersion2Header(serializedHeader);
    } else if (serializedHeader.size() == TOTAL_HEADER_SIZE_VERSION_1) {
        deserializeVersion1Header(serializedHeader);
    } else {
        throw HeaderError{_("Invalid header size")};
    }
}

Header10& Header10::operator=(const SecureArray& serializedHeader) {
    if (serializedHeader.size() == TOTAL_HEADER_SIZE_VERSION_2) {
        deserializeVersion2Header(serializedHeader);
    } else if (serializedHeader.size() == TOTAL_HEADER_SIZE_VERSION_1) {
        deserializeVersion1Header(serializedHeader);
    } else {
        throw HeaderError{_("Invalid header size")};
    }

    return *this;
}

Header10::operator SecureArray() const {
    // Copy version to SecureArray
    SecureArray serializedHeader{TOTAL_HEADER_SIZE_VERSION_2};
    **serializedHeader = _version;

    // Copy control string to SecureArray
    auto pointerToControlString = (*serializedHeader) + VERSION_SIZE;
    std::memcpy(pointerToControlString, CONTROL_STRING, CONTROL_STRING_SIZE);

    // Copy password modification time to SecureArray
    auto passwordSetTimeInODSFormat = toODS(_passwordSetTime);
    auto pointerToSetTime =
        (*serializedHeader) + VERSION_SIZE + CONTROL_STRING_SIZE;
    std::memcpy(pointerToSetTime, &passwordSetTimeInODSFormat,
                PASSWORD_SET_TIME_SIZE);

    return serializedHeader;
}