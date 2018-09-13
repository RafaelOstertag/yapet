#include <cstdio>

#include "consts.h"
#include "header10.hh"
#include "headererror.hh"
#include "intl.h"
#include "ods.hh"

using namespace yapet;

constexpr std::uint8_t Header10::CONTROL_STRING[];

void Header10::testIfControlStringMatchesOrThrow(
    const SecureArray& serializedHeader) const {
    auto pointerToControlString = (*serializedHeader) + VERSION_SIZE;
    auto result = std::memcmp(pointerToControlString, CONTROL_STRING,
                              CONTROL_STRING_SIZE);
    if (result != 0) {
        throw HeaderError{_("Control string does not match")};
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
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("Expected header version 1 got %d"), _version);
        throw HeaderError{msg};
    }

    _passwordSetTime =
        unserializePasswordSetTime<std::uint32_t>(serializedHeader);
}

void Header10::deserializeVersion2Header(const SecureArray& serializedHeader) {
    // We have to test the control string before testing the version in order
    // to raise an invalid password error. If we test the version before the
    // control string we don't know whether the password is wrong or the version
    // is wrong.
    testIfControlStringMatchesOrThrow(serializedHeader);

    _version = **serializedHeader;

    // Pre 0.6 headers (version 1) created on certain 64bit machines use 64bits
    // time_t types and thus are identified as as version 2 headers by taking
    // the size into consideration only
    if ((_version != VERSION_2) && (_version != VERSION_1)) {
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("Expected header version 1 or 2, got %d"), _version);
        throw HeaderError(msg);
    }

    _passwordSetTime =
        unserializePasswordSetTime<std::uint64_t>(serializedHeader);
}

void Header10::deserializeHeader(const SecureArray& serializedHeader) {
    auto headerSize{serializedHeader.size()};

    // The header size of headers create by previous version may be bigger than
    // the exact total byte size, since previous versions encrypted the entire
    // struct{} instead of the serialized struct{}
    if (headerSize >= TOTAL_HEADER_SIZE_VERSION_2) {
        deserializeVersion2Header(serializedHeader);
    } else if ((headerSize >= TOTAL_HEADER_SIZE_VERSION_1) &&
               (headerSize < TOTAL_HEADER_SIZE_VERSION_2)) {
        deserializeVersion1Header(serializedHeader);
    } else {
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("Invalid header size %d"), headerSize);
        throw HeaderError{msg};
    }
}

Header10::Header10(const SecureArray& serializedHeader) {
    deserializeHeader(serializedHeader);
}

Header10& Header10::operator=(const SecureArray& serializedHeader) {
    deserializeHeader(serializedHeader);
    return *this;
}

SecureArray Header10::serialize() const {
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