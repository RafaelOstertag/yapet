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

#include <unistd.h>

#ifdef TIME_WITH_SYS_TIME
#include <sys/time.h>
#include <time.h>
#else
#ifdef HAVE_SYS_TIME_H
#include <sys/time.h>
#else
#include <time.h>
#endif
#endif  // TIME_WITH_SYS_TIME

#ifdef HAVE_INTTYPES_H
#include <inttypes.h>
#endif

#include <sys/types.h>

#include <sys/stat.h>

#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif

#ifdef HAVE_ALLOCA_H
#include <alloca.h>
#endif

#include <cerrno>

#include "crypt.h"
#include "file.h"
#include "fileutils.hh"
#include "header10.hh"
#include "intl.h"
#include "record.h"
#include "structs.h"

using namespace YAPET;
using namespace yapet;

void File::initializeEmptyFile(const Key& key) {
    // Crypt crypt(key);
    // yapet::Header10 header;

    // mtime = lastModified();
    // writeHeader(header, key);

    // // Sanity checks
    // BDBuffer* buff = readHeader();
    // if (buff == 0)
    //     throw YAPETException(_("EOF encountered while reading header"));
    // Record<FileHeader_64>* dec_hdr = crypt.decrypt<FileHeader_64>(*buff);
    // FileHeader_64* ptr_dec_hdr = *dec_hdr;
    // int retval =
    //     std::memcmp(ptr_dec_hdr->control, ptr->control, HEADER_CONTROL_SIZE);
    // if (retval != 0)
    //     throw YAPETException(_("Sanity check for control field failed"));
    // delete buff;
    // delete dec_hdr;
}

/**
 * Opens the file specified. Optionally creates the file if it does
 * not exist and \c create is set \c true.
 *
 * When opening an existing file, the key provided is validated. When
 * creating a new file, the key is used to encrypt the header.
 *
 * The file opened or created will stay open as long as the instance
 * of this class exists. There is no method for closing the file. Only
 * the destructor closes the file.
 *
 * @param fn string holding the file name
 *
 * @param key the key used for verification or encrypting the file
 * header
 *
 * @param create flag indicating whether the file should be created
 * (\c true) or just opened (\c false). Be aware that passing \c true
 * to this flag always causes the file to be created. Even if it
 * already exists. Existing files will be truncated and the data
 * stored will be lost.
 *
 * @param secure if \c true, the functions checks whether or not the file
 * permissions are secure. If \c false, file permissions are not checked for
 * security. When creating a file and the value is \c true, the file is created
 * using secure file permissions meaning only the owner has write and read
 * access. Else, the owner has read and write access, the group and world has
 * read access.
 */
File::File(const std::string& filename, const Key& key, bool create,
           bool secure)
    : _yapetFile{new yapet::Yapet10File{filename, create, secure}},
      _fileModificationTime{0} {
    // if (getFileSize(filename) == 0) {
    //     initializeEmptyFile(key);
    // } else {
    //     validateKey(key);
    // }
}

/**
 * Closes the file.
 */
File::~File() {}

/**
 * Stores the list of \c PartDec records in the file.
 *
 * @param records list of \c PartDec records
 *
 * @param forcewrite before writing any data, the method checks
 * whether the last modification date stored in \c mtime matches the
 * date returned by \c lastModified(). If they differ, and this flag
 * is set to \c false, the write operation will fail and an exception
 * is thrown. If the flag is set to \c true, it writes the data to the
 * file regardless of the last modification date.
 *
 * @throw YAPETRetryException if the file has been externally modified
 * (outside of this class), and \c forcewrite is not \c true, this
 * exception is thrown.
 *
 * @sa PartDec
 */
void File::save(const std::list<PartDec>& records, bool forcewrite) {
    // if ((_fileModificationTime != getModificationTime(_rawFile->file)) &&
    //     !forcewrite)
    //     throw YAPETRetryException(_("File has been externally modified"));

    // if (usefsecurity) setFileSecurity();

    // preparePWSave();
    // std::list<PartDec>::const_iterator it = records.begin();

    // while (it != records.end()) {
    //     write(it->getEncRecord());
    //     ++it;
    // }
}

/**
 * Reads the stored records from the file using the key provided and
 * returns a list holding the partially decrypted records. If the file
 * has no records stored, it returns an empty list.
 *
 * @param key the key used to decrypt the records. It has to be same
 * key that was used to encrypt the records, of course.
 *
 * @return a list holding the partially decrypted records. Or an empty
 * list if no records are stored in the file
 *
 * @sa PartDec
 */
std::list<PartDec> File::read(const Key& key) const {
    // seekDataSection();
    // BDBuffer* buff = 0;
    // std::list<PartDec> retval;

    // try {
    //     buff = read();

    //     while (buff != 0) {
    //         retval.push_back(PartDec(*buff, key));
    //         delete buff;
    //         buff = read();
    //     }
    // } catch (YAPETException& ex) {
    //     if (buff != 0) delete buff;

    //     throw;
    // }

    // return retval;
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
void File::setNewKey(const Key& oldkey, const Key& newkey, bool forcewrite) {
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
int64_t File::getMasterPWSet(const Key& key) const {
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
FILE_VERSION
File::getFileVersion(const Key& key) const {
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

    // FILE_VERSION v;
    // if (ptr_dec_header_32 != 0) {
    //     v = static_cast<FILE_VERSION>(ptr_dec_header_32->version);
    // } else {
    //     assert(ptr_dec_header_64 != 0);
    //     v = static_cast<FILE_VERSION>(ptr_dec_header_64->version);
    // }

    // if (dec_header_32 != 0) delete dec_header_32;
    // if (dec_header_64 != 0) delete dec_header_64;

    // return v;
}
