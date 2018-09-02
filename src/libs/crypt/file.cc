// $Id$
//
// Copyright (C) 2008-2011, 2013  Rafael Ostertag
//
// This file is part of YAPET.
//
// YAPET is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// YAPET is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// YAPET.  If not, see <http://www.gnu.org/licenses/>.
//
// Additional permission under GNU GPL version 3 section 7
//
// If you modify this program, or any covered work, by linking or combining it
// with the OpenSSL project's OpenSSL library (or a modified version of that
// library), containing parts covered by the terms of the OpenSSL or SSLeay
// licenses, Rafael Ostertag grants you additional permission to convey the
// resulting work.  Corresponding Source for a non-source form of such a
// combination shall include the source code for the parts of OpenSSL used as
// well as that of the covered work.
//

#include <algorithm>

#include "file.h"
#include "fileutils.hh"
#include "header10.hh"
#include "intl.h"

using namespace YAPET;
using namespace yapet;

void File::initializeEmptyFile() {
    Header10 header{time(0)};

    _yapetFile->writeIdentifier();

    auto encryptedSerializedHeader = _crypto->encrypt(header.serialize());

    _yapetFile->writeMetaData(encryptedSerializedHeader);
}

void File::validateExistingFile() {
    auto encryptedSerializedHeader = _yapetFile->readMetaData();
    auto serializedHeader = _crypto->decrypt(encryptedSerializedHeader);

    Header10 header{serializedHeader};
    // If we read an invalid header version, intToHeaderVersion() will throw an
    // exception
    intToHeaderVersion(header.version());
}

void File::notModifiedOrThrow() {
    auto currentFileModificationTime{
        yapet::getModificationTime(_yapetFile->filename())};
    if (currentFileModificationTime != _fileModificationTime) {
        throw YAPET::YAPETRetryException(
            _("File has been externally modified"));
    }
}

File::File(const yapet::AbstractCryptoFactory& abstractCryptoFactory,
           const std::string& filename, const SecureArray& password,
           bool create, bool secure)
    : _fileModificationTime{0},
      _yapetFile{abstractCryptoFactory.file(filename, create, secure)},
      _crypto{abstractCryptoFactory.crypto(password)} {
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
}

std::list<PasswordListItem> File::read() const {
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

    return result;
}

/**
 * Uses a new key to encrypt the records in the file. The records
 * stored in the file are decrypted using the old key and then
 * encrypted using the new key.
 *
 * Before performing this operation, it renames the file encrypted
 * with the old key to 'filename + ".bak"'. It then reads the data
 * from this file and writes it to the newly created file named
 * 'filename'.
 *
 * @param oldkey the old key used to encrypt the records
 *
 * @param newkey the new key used to encrypt the records
 */
void File::setNewKey(const SecureArray& newPassword, bool forcewrite) {
    // if ((mtime != lastModified()) && !forcewrite)
    //     throw YAPETRetryException(_("File has been externally modified"));

    // close(fd);
    // std::string backupfilename(filename + ".bak");
    // int retval = rename(filename.c_str(), backupfilename.c_str());

    // if (retval == -1) {
    //     // Reopen the old file
    //     openNoCreate();
    //     throw YAPETException(std::strerror(errno));
    // }

    // File* oldfile = 0;

    // try {
    //     // Reopen the old (backup) file
    //     oldfile = new File(backupfilename, oldkey, false, false);
    //     // Initialize the (this) file with the new key
    //     openCreate();
    //     initFile(newkey);
    //     // Retrieve the records encrypted with the old key
    //     std::list<PartDec> entries = oldfile->read(oldkey);
    //     std::list<PartDec>::iterator it = entries.begin();
    //     Crypt oldcrypt(oldkey);
    //     Crypt newcrypt(newkey);

    //     while (it != entries.end()) {
    //         Record<PasswordRecord>* dec_rec_ptr = 0;
    //         BDBuffer* new_enc_rec = 0;

    //         try {
    //             // Decrypt with the old key
    //             const BDBuffer old_enc_rec = (*it).getEncRecord();
    //             dec_rec_ptr = oldcrypt.decrypt<PasswordRecord>(old_enc_rec);
    //             new_enc_rec = newcrypt.encrypt(*dec_rec_ptr);
    //             write(*new_enc_rec);
    //             delete dec_rec_ptr;
    //             delete new_enc_rec;
    //         } catch (YAPETException& ex) {
    //             if (dec_rec_ptr != 0) delete dec_rec_ptr;

    //             if (new_enc_rec != 0) delete new_enc_rec;

    //             throw;
    //         }

    //         ++it;
    //     }
    // } catch (YAPETException& ex) {
    //     if (oldfile != 0) delete oldfile;

    //     throw;
    // }

    // delete oldfile;
}

/**
 * Modified in version 0.6.
 *
 * Returns the time as a \c uint64_t when the master password was set.
 *
 * @param key the key used to decrypt the header.
 *
 * @return a \c uint64_t representing the time the master password was
 * set.
 */
int64_t File::getMasterPWSet() const {
    // Expect either a 32bit or 64bit header
    // Record<FileHeader_32>* dec_header_32 = 0;
    // Record<FileHeader_64>* dec_header_64 = 0;

    // readHeader(key, &dec_header_32, &dec_header_64);
    // assert(dec_header_32 != 0 || dec_header_64 != 0);

    // FileHeader_32* ptr_dec_header_32 =
    //     (dec_header_32 != 0) ? static_cast<FileHeader_32*>(*dec_header_32) :
    //     0;
    // FileHeader_64* ptr_dec_header_64 =
    //     (dec_header_64 != 0) ? static_cast<FileHeader_64*>(*dec_header_64) :
    //     0;

    // int64_t t;
    // if (ptr_dec_header_32 != 0) {
    //     t = int_from_disk<int32_t>(ptr_dec_header_32->pwset);
    // } else {
    //     assert(ptr_dec_header_64 != 0);
    //     t = int_from_disk<int64_t>(ptr_dec_header_64->pwset);
    // }

    // if (dec_header_32 != 0) delete dec_header_32;
    // if (dec_header_64 != 0) delete dec_header_64;

    // return t;
}

/**
 * New since version 0.6.
 *
 * Return the file version.
 */
SecureArray File::getFileVersion() const {
    return _yapetFile->readIdentifier();
}

HEADER_VERSION File::getHeaderVersion() const {
    auto encryptedSerializedHeader{_yapetFile->readMetaData()};
    auto serializedHeader{_crypto->decrypt(encryptedSerializedHeader)};

    Header10 header{serializedHeader};
    return yapet::intToHeaderVersion(header.version());
}
