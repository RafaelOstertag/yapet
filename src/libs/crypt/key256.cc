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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <argon2.h>
#include <openssl/rand.h>
#include <cstdio>
#include <cstring>
#include <typeinfo>
#ifdef DEBUG_LOG
#include <iomanip>
#include <sstream>
#endif

#include "consts.h"
#include "cryptoerror.hh"
#include "globals.h"
#include "intl.h"
#include "key256.hh"
#include "logger.hh"
#include "ods.hh"

using namespace yapet;

namespace {
#ifdef DEBUG_LOG
constexpr char hexPrefix[]{"0x"};
#endif
/**
 * The max length of key in bytes (256 bits)
 */
constexpr int KEY_LENGTH{32};
constexpr int SSL_SUCCESS{1};

// in bytes
constexpr int SALT_NIBBLE_SIZE{sizeof(int)};
constexpr int NUMBER_OF_SALT_NIBBLES{4};
// In bytes
constexpr int SALT_LENGTH = SALT_NIBBLE_SIZE * NUMBER_OF_SALT_NIBBLES;

union architecture_agnostic_salt_type {
    int nibbles[NUMBER_OF_SALT_NIBBLES];
    uint8_t bytes[SALT_LENGTH];
};

#ifdef DEBUG_LOG
std::string saltNibblesToHexString(const int* salt) {
    std::stringstream hexValue;
    hexValue << hexPrefix << std::hex;
    for (int i = 0; i < NUMBER_OF_SALT_NIBBLES; i++) {
        hexValue << salt[i];
    }
    return hexValue.str();
}

std::string saltBytesToHexString(const std::uint8_t* salt) {
    std::stringstream hexValue;
    hexValue << hexPrefix << std::hex;
    for (int i = 0; i < SALT_LENGTH; i++) {
        hexValue << static_cast<int>(salt[i]);
    }
    return hexValue.str();
}

std::string metaDataSaltToHexString(const MetaData& metaData) {
    std::stringstream hexValue;
    hexValue << hexPrefix << std::hex
             << metaData.getValue(YAPET::Consts::ARGON2_SALT1_KEY)
             << metaData.getValue(YAPET::Consts::ARGON2_SALT2_KEY)
             << metaData.getValue(YAPET::Consts::ARGON2_SALT3_KEY)
             << metaData.getValue(YAPET::Consts::ARGON2_SALT4_KEY);
    return hexValue.str();
}

std::string secureArrayToHexString(const SecureArray& secureArray) {
    std::stringstream hexValue;
    hexValue << hexPrefix << std::hex;
    for (SecureArray::size_type i = 0; i < secureArray.size(); i++) {
        hexValue << static_cast<int>(secureArray[i]);
    }

    return hexValue.str();
}
#endif

inline void composeSaltFromIntegers(const int* nibbles, uint8_t* saltBuffer) {
    architecture_agnostic_salt_type archAgnosticSalt;
    for (int i = 0; i < NUMBER_OF_SALT_NIBBLES; i++) {
        archAgnosticSalt.nibbles[i] = toODS(nibbles[i]);
    }

    std::memcpy(saltBuffer, archAgnosticSalt.bytes, SALT_LENGTH);
}

inline SecureArray hash(const SecureArray& password,
                        const MetaData& parameters) {
    int timeCost = parameters.getValue(YAPET::Consts::ARGON2_TIME_COST_KEY);
    int memoryCost = parameters.getValue(YAPET::Consts::ARGON2_MEMORY_COST_KEY);
    int parallelism =
        parameters.getValue(YAPET::Consts::ARGON2_PARALLELISM_KEY);

    int saltNibbles[NUMBER_OF_SALT_NIBBLES];
    saltNibbles[0] = parameters.getValue(YAPET::Consts::ARGON2_SALT1_KEY);
    saltNibbles[1] = parameters.getValue(YAPET::Consts::ARGON2_SALT2_KEY);
    saltNibbles[2] = parameters.getValue(YAPET::Consts::ARGON2_SALT3_KEY);
    saltNibbles[3] = parameters.getValue(YAPET::Consts::ARGON2_SALT4_KEY);

    uint8_t saltBuffer[SALT_LENGTH];
    composeSaltFromIntegers(saltNibbles, saltBuffer);

    SecureArray hash{KEY_LENGTH};

    int status = argon2i_hash_raw(timeCost, memoryCost, parallelism, *password,
                                  password.size(), saltBuffer, SALT_LENGTH,
                                  *hash, hash.size());

    LOG_MESSAGE(std::string{__func__} + " ## Hash Password Begin");
    LOG_MESSAGE(std::string{__func__} +
                " Salt from Meta data: " + metaDataSaltToHexString(parameters));
    LOG_MESSAGE(std::string{__func__} +
                " Salt as nibbles: " + saltNibblesToHexString(saltNibbles));
    LOG_MESSAGE(std::string{__func__} +
                " Salt as bytes: " + saltBytesToHexString(saltBuffer));
    LOG_MESSAGE(std::string{__func__} + " Password '" +
                secureArrayToString(password) + "' is hashed as " +
                secureArrayToHexString(hash));
    LOG_MESSAGE(std::string{__func__} + " ## Hash Password End");

    if (status != ARGON2_OK) {
        throw HashError(_("Error hashing password using Argon2"));
    }
    return hash;
}

int randomInt() {
    int i;
    auto result = RAND_bytes(reinterpret_cast<unsigned char*>(&i), sizeof(int));
    if (result != SSL_SUCCESS) {
        throw HashError{_("Cannot generate random salt")};
    }
    return i;
}

}  // namespace

MetaData Key256::newDefaultKeyingParameters() {
    MetaData metaData{};
    metaData.setValue(YAPET::Consts::ARGON2_MEMORY_COST_KEY,
                      YAPET::Globals::config.argon2_memory.get());
    metaData.setValue(YAPET::Consts::ARGON2_PARALLELISM_KEY,
                      YAPET::Globals::config.argon2_parallelism.get());
    metaData.setValue(YAPET::Consts::ARGON2_TIME_COST_KEY,
                      YAPET::Globals::config.argon2_iterations.get());
    metaData.setValue(YAPET::Consts::ARGON2_SALT1_KEY, randomInt());
    metaData.setValue(YAPET::Consts::ARGON2_SALT2_KEY, randomInt());
    metaData.setValue(YAPET::Consts::ARGON2_SALT3_KEY, randomInt());
    metaData.setValue(YAPET::Consts::ARGON2_SALT4_KEY, randomInt());

    LOG_MESSAGE(std::string{__func__} + ": Created random salt " +
                metaDataSaltToHexString(metaData));
    return metaData;
}

/**
 * Initializes the key and the initialization vector. Make sure you
 * securely destroy the password provided to this method.
 *
 * @param password a pointer to the location the password is
 * stored. The password has to be zero-terminated.
 */
Key256::Key256() : _key{0}, _keyingParameters{} {}

void Key256::keyingParameters(const MetaData& parameters) {
    _keyingParameters = parameters;
}

const MetaData& Key256::keyingParameters() const { return _keyingParameters; }

void Key256::password(const SecureArray& password) {
    SecureArray passwordWithoutZeroTerminator{password.size() - 1};
    passwordWithoutZeroTerminator << password;

    _key = hash(passwordWithoutZeroTerminator, _keyingParameters);

    if (_key.size() != KEY_LENGTH) {
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(
            msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
            _("Effective key length of %d does not match expected key "
              "length %d"),
            _key.size(), KEY_LENGTH);
        throw HashError{msg};
    }
}

Key256::Key256(Key256&& k)
    : _key{std::move(k._key)},
      _keyingParameters{std::move(k._keyingParameters)} {}

Key256::Key256(const Key256& k)
    : _key{k._key}, _keyingParameters{k._keyingParameters} {}

Key256& Key256::operator=(const Key256& k) {
    if (this == &k) return *this;

    _key = k._key;
    _keyingParameters = k._key;

    return *this;
}

Key256& Key256::operator=(Key256&& k) {
    if (this == &k) return *this;

    _key = std::move(k._key);
    _keyingParameters = std::move(k._keyingParameters);

    return *this;
}

bool Key256::operator==(const Key256& k) const { return _key == k._key; }
bool Key256::operator==(const Key& k) const {
    if (typeid(k) != typeid(*this)) {
        return false;
    }

    return operator==(dynamic_cast<const Key256&>(k));
}

bool Key256::operator!=(const Key256& k) const { return !operator==(k); }

bool Key256::operator!=(const Key& k) const {
    if (typeid(k) != typeid(*this)) {
        return true;
    }

    return operator!=(dynamic_cast<const Key256&>(k));
}