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

#include <algorithm>

#include "cryptoerror.hh"
#include "file.hh"
#include "fileutils.hh"
#include "intl.h"
#include "logger.hh"
#include "yapeterror.hh"

using namespace YAPET;
using namespace yapet;

Header10 File::readHeader() {
    auto encryptedSerializedHeader{_yapetFile->readHeader()};
    auto serializedHeader{_crypto->decrypt(encryptedSerializedHeader)};
    return Header10{serializedHeader};
}

void File::initializeEmptyFile() {
    Header10 header{time(0)};

    _yapetFile->writeIdentifier();
    _yapetFile->writeUnencryptedMetaData(
        _crypto->getKey()->keyingParameters().serialize());

    auto encryptedSerializedHeader = _crypto->encrypt(header.serialize());

    _yapetFile->writeHeader(encryptedSerializedHeader);
}

void File::validateExistingFile() {
    auto encryptedSerializedHeader = _yapetFile->readHeader();
    yapet::SecureArray serializedHeader;
    try {
        serializedHeader = _crypto->decrypt(encryptedSerializedHeader);
    } catch (EncryptionError& e) {
        LOG_MESSAGE(std::string{__func__} + ": invalid password");
        // most likely caused by invalid password
        throw InvalidPasswordError{_("Invalid password")};
    }

    Header10 header;
    try {
        header = Header10{serializedHeader};
    } catch (ControlStringMismatch& e) {
        LOG_MESSAGE(std::string{__func__} + ": invalid password");
    }

    // If we read an invalid header version, intToHeaderVersion() will throw an
    // exception
    intToHeaderVersion(header.version());
}

void File::notModifiedOrThrow() {
    auto currentFileModificationTime{
        yapet::getModificationTime(_yapetFile->filename())};
    if (currentFileModificationTime != _fileModificationTime) {
        throw RetryableError{_("File has been externally modified")};
    }
}

File::File(std::shared_ptr<yapet::AbstractCryptoFactory> abstractCryptoFactory,
           const std::string& filename, bool create, bool secure)
    : _fileModificationTime{0},
      _abstractCryptoFactory{abstractCryptoFactory},
      _yapetFile{abstractCryptoFactory->file(filename, create, secure)},
      _crypto{abstractCryptoFactory->crypto()} {
    _yapetFile->open();
    if (getFileSize(filename) == 0) {
        initializeEmptyFile();
    } else {
        validateExistingFile();
    }

    _fileModificationTime = getModificationTime(_yapetFile->filename());
}

File::~File() {}

void File::save(const std::list<PasswordListItem>& records, bool forcewrite) {
    if (!forcewrite) {
        notModifiedOrThrow();
    }

    std::list<SecureArray> encryptedPasswordRecords{};
    std::for_each(records.begin(), records.end(),
                  [&encryptedPasswordRecords](const PasswordListItem& i) {
                      encryptedPasswordRecords.push_back(i.encryptedRecord());
                  });

    _yapetFile->writePasswordRecords(encryptedPasswordRecords);
    _fileModificationTime = yapet::getModificationTime(_yapetFile->filename());
    LOG_MESSAGE("Save yapet file");
}

std::list<PasswordListItem> File::read() {
    auto encryptedPasswordRecords{_yapetFile->readPasswordRecords()};

    std::list<SecureArray>::iterator it = encryptedPasswordRecords.begin();
    std::list<PasswordListItem> result;
    while (it != encryptedPasswordRecords.end()) {
        auto decryptedSerializedPasswordRecord{_crypto->decrypt(*it)};
        PasswordRecord passwordRecord{decryptedSerializedPasswordRecord};

        result.push_back(PasswordListItem{
            reinterpret_cast<const char*>(passwordRecord.name()), *it});
        it++;
    }

    LOG_MESSAGE("Read yapet file");
    return result;
}

void File::setNewKey(
    const std::shared_ptr<yapet::AbstractCryptoFactory>& newCryptoFactory,
    bool forcewrite) {
    if (!forcewrite) {
        notModifiedOrThrow();
    }

    bool isSecure = _yapetFile->isSecure();
    std::string filename{_yapetFile->filename()};

    // close and destroy old file
    LOG_MESSAGE("File::setNewKey(): close currently open file");
    _yapetFile.reset();

    LOG_MESSAGE("File::setNewKey(): rename file");
    std::string backupfilename(filename + ".bak");
    yapet::renameFile(filename, backupfilename);

    LOG_MESSAGE("File::setNewKey(): open renamed file");
    std::unique_ptr<yapet::YapetFile> oldFile{
        _abstractCryptoFactory->file(backupfilename, false, false)};
    oldFile->open();

    LOG_MESSAGE("File::setNewKey(): swap crypto factories");
    auto cryptoFactory{newCryptoFactory};
    auto otherCrypto{cryptoFactory->crypto()};
    _abstractCryptoFactory.swap(cryptoFactory);

    _crypto.swap(otherCrypto);

    LOG_MESSAGE("File::setNewKey(): create new file");
    _yapetFile = _abstractCryptoFactory->file(filename, true, isSecure);
    _yapetFile->open();

    LOG_MESSAGE("File::setNewKey(): initialize new file");
    initializeEmptyFile();
    std::list<yapet::SecureArray> newlyEncryptedRecords{};
    LOG_MESSAGE("File::setNewKey(): read password records from renamed file");
    for (auto encryptedSerializedRecord : oldFile->readPasswordRecords()) {
        auto serializedRecord{otherCrypto->decrypt(encryptedSerializedRecord)};
        auto newlyEncryptedSerializedRecord{_crypto->encrypt(serializedRecord)};
        newlyEncryptedRecords.push_back(newlyEncryptedSerializedRecord);
    }
    LOG_MESSAGE("File::setNewKey(): write password records to new file");
    _yapetFile->writePasswordRecords(newlyEncryptedRecords);
    _fileModificationTime = yapet::getModificationTime(_yapetFile->filename());
}

int64_t File::getMasterPWSet() {
    auto header{readHeader()};
    return header.passwordSetTime();
}

/**
 * New since version 0.6.
 *
 * Return the file version.
 */
SecureArray File::getFileVersion() { return _yapetFile->readIdentifier(); }

HEADER_VERSION File::getHeaderVersion() {
    auto encryptedSerializedHeader{_yapetFile->readHeader()};
    auto serializedHeader{_crypto->decrypt(encryptedSerializedHeader)};

    Header10 header{serializedHeader};
    return yapet::intToHeaderVersion(header.version());
}
