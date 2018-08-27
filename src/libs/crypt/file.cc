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

#include "intl.h"
#include "crypt.h"
#include "record.h"
#include "structs.h"
#include "file.h"

#include <unistd.h>

#ifdef TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# ifdef HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif // TIME_WITH_SYS_TIME

#ifdef HAVE_INTTYPES_H
# include <inttypes.h>
#endif

#include <sys/types.h>

#include <sys/stat.h>

#ifdef HAVE_FCNTL_H
# include <fcntl.h>
#endif

#ifdef HAVE_ALLOCA_H
# include <alloca.h>
#endif

#include <cstring>
#include <cerrno>


using namespace YAPET;

/**
 * @brief The control string for the file header.
 *
 * This is the control string used for the file header to verify the
 * decryption of the header. It is stored in the file header. When the
 * key is verified, the header is read and this string is compared to
 * the one stored in the file header. If they don't match, the class
 * assumes the key provided is invalid.
 */
const char CONTROL_STR[] = "ABCDEFGHIJKLMNOPQRSTUVW";

/**
 * @brief The recognition string.
 *
 * The recognition string is saved plain-text at the very beginning of
 * each file. When files are opened, the methods check for the
 * existence of this string. If this string is not found, the methods
 * report an unknown file.
 */
const char RECOG_STR[] = "YAPET1.0";

/**
 * Checks the permission and owner of a file for security.
 *
 * It throws a \c YAPETRetryException if the owner of the file does not match
 * the uid of the owner of the process, or if the mode is not \c
 * (S_IRUSR|S_IWUSR).
 */
void
File::checkFileSecurity() {
    struct stat buf;
    int err = fstat (fd, &buf);

    if (err == -1)
        throw YAPETException (std::strerror (errno) );

    uid_t uid = getuid();

    if (buf.st_uid != uid) {
        std::string tmp (_ ("You are not the owner of ") );
        throw YAPETRetryException (tmp + filename);
    }

    if (buf.st_mode != (S_IFREG | S_IRUSR | S_IWUSR) ) {
        std::string tmp1 (_ ("Permissions of ") );
        std::string tmp2 (_ (" not secure.") );
        throw YAPETRetryException (tmp1 + filename + tmp2);
    }
}

/**
 * Sets the owner and permissions on a file in a manner that \c
 * File::checkFileSecurity does not complain.
 *
 * If it cannot set the security permissions, it throws a \c
 * YAPETRetryException if the error can be avoided using non-secure settings.
 */
void
File::setFileSecurity() {
    struct stat buf;
    int err = fstat (fd, &buf);

    if (err == -1)
        throw YAPETException (std::strerror (errno) );

    err = fchown (fd, getuid(), buf.st_gid);

    if (err == -1) {
        std::string tmp (_ ("Cannot set the owner of ") );
        throw YAPETRetryException (tmp + filename);
    }

    err = fchmod (fd, S_IRUSR | S_IWUSR);

    if (err == -1) {
        std::string tmp (_ ("Cannot set file permissions on ") );
        throw YAPETRetryException (tmp + filename);
    }
}


/**
 * Creates a file with name specified in \c filename and sets \c fd to
 * the obtained file descriptor. The file is opened for read-write.
 *
 */
void
File::openCreate() {
    fd = ::open (filename.c_str(),
                 O_RDWR | O_CREAT | O_TRUNC | O_APPEND,
                 (usefsecurity ? S_IRUSR | S_IWUSR :
                  S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH ) );

    if (fd == -1)
        throw YAPETException (std::strerror (errno) );

    mtime = lastModified();

    if (usefsecurity)
        checkFileSecurity();
}

/**
 * Opens an existing file in read-write mode. If the file does not
 * exist, the method throws an exception.
 */
void
File::openNoCreate() {
    fd = ::open (filename.c_str(), O_RDWR | O_APPEND);

    if (fd == -1)
        throw YAPETException (std::strerror (errno) );

    mtime = lastModified();

    if (usefsecurity)
        checkFileSecurity();
}

/**
 * Returns the time of the last modification of the file specified by
 * \c fd.
 *
 * @return a \c time_t holdign the last modification date.
 */
int64_t
File::lastModified() const {
    struct stat st_buf;
    int retval = fstat (fd, &st_buf);

    if (retval == -1)
        throw YAPETException (std::strerror (errno) );

    return st_buf.st_mtime;
}

/**
 * Seeks \c offset bytes from the current position in the file
 * specified by \c fd.
 *
 * @param offset the offset in bytes from the current position in the
 * file.
 */
void
File::seekCurr (off_t offset) const {
    off_t pos = lseek (fd, offset, SEEK_CUR);

    if ( ( (off_t) - 1) == pos)
        throw YAPETException (std::strerror (errno) );
}

/**
 * Seeks \c offset bytes from the beginning of the file.
 *
 * @param offset the offset in bytes from the beginning.
 */
void
File::seekAbs (off_t offset) const {
    off_t pos = lseek (fd, offset, SEEK_SET);

    if ( ( (off_t) - 1) == pos)
        throw YAPETException (std::strerror (errno) );

    if (pos != offset)
        throw YAPETException (_ ("Error seeking within file: ") + filename);
}

/**
 * Truncates the file up to the header by creating a new empty file
 * copying over the existing header.
 *
 */
void
File::preparePWSave() {
    BDBuffer* curr_header = readHeader();
    ::close (fd);

    try {
        openCreate();
    } catch (YAPETException& ex) {
        if (curr_header != 0)
            delete curr_header;

        throw;
    }

    mtime = lastModified();
    writeHeader (*curr_header);
    delete curr_header;
}

/**
 * Seeks to the first password record in the file. The file pointer is
 * set to point to the record length indicator, so that a call to \c
 * read() will return the first password record.
 */
void
File::seekDataSection() const {
    seekAbs (std::strlen (RECOG_STR) );
    uint32_t len;
    int retval = ::read (fd, &len, sizeof (uint32_t) );

    if (retval == -1)
        throw YAPETException (std::strerror (errno) );

    if ( ( (size_t) retval) != sizeof (uint32_t) )
        throw YAPETException (_ ("Unable to seek to data section") );

    len = int_from_disk<uint32_t> (len);
    seekCurr (len);
}

/**
 * Reads from the current position in the file. The method expects the
 * file pointer to point a record length indicator. In other words, it
 * first reads four bytes from the current file position, which will
 * give it a clue about the length of the encrypted record to read.
 *
 * It then reads as many bytes as indicated from the file and returns
 * the data read in a \c BDBuffer. The memory allocated by the \c
 * BDBuffer has to be freed by the caller.
 *
 * The record length indicator needs to be stored in big-endian order.
 *
 * @return a pointer to a \c BDBuffer holding the encrypted password
 * record. The memory occupied by the buffer has to be freed by the
 * caller of the method. It returns \c 0 when the end of file has
 * been reached.
 */
BDBuffer*
File::read() const {
    uint32_t len;
    int retval = ::read (fd, &len, sizeof (uint32_t) );

    if (retval == -1)
        throw YAPETException (std::strerror (errno) );

    if (retval == 0)
        return 0;

    if ( ( (size_t) retval) < sizeof (uint32_t) )
        throw YAPETException (_ ("Short read on file: ") + filename);

    // Convert len to the endianess of the architecture
    len = int_from_disk<uint32_t> (len);
    BDBuffer* buf = new BDBuffer (len);
    retval = ::read (fd, *buf, len);

    if (retval == -1)
        throw YAPETException (std::strerror (errno) );

    if (retval == 0) {
        delete buf;
        return 0;
    }

    if ( ( (uint32_t) retval) < len) {
        delete buf;
        throw YAPETException (_ ("Short read on file: ") + filename);
    }

    return buf;
}

/**
 * Writes the supplied \c BDBuffer to disk. It writes in front of the
 * \c BDBuffer its length. The length is stored in big-endian order.
 *
 * @param buff reference to a \c BDBuffer holding the encrypted data
 * to write
 *
 * @param forceappend if this flag is set to \c true, the method first
 * seeks to the end of the file, if set to \c false, it writes at the
 * position the file pointer points to.
 */
void
File::write (const BDBuffer& buff, bool forceappend) {
    if (forceappend) {
        off_t pos = lseek (fd, 0, SEEK_END);

        if ( ( (off_t) - 1) == pos)
            throw YAPETException (std::strerror (errno) );
    }

    uint32_t s = buff.size();
    // Convert s to the on-disk structure
    s = int_to_disk<uint32_t> (s);
    int retval = ::write (fd, &s, sizeof (uint32_t) );

    if (retval == -1)
        throw YAPETException (std::strerror (errno) );

    if (retval != sizeof (uint32_t) )
        throw YAPETException (_ ("Short write on file: ") + filename);

    retval = ::write (fd, buff, buff.size() );

    if (retval == -1)
        throw YAPETException (std::strerror (errno) );

    if ( ( (size_t) retval) < buff.size() )
        throw YAPETException (_ ("Short write on file: ") + filename);

    mtime = lastModified();
}

/**
 * Indicates whether or not the file specified by \c fd is empty.
 *
 * @return \c true if the file's size is zero, \c false otherwise.
 */
bool
File::isempty() const {
    struct stat st_buf;
    int retval = fstat (fd, &st_buf);

    if (retval == -1)
        throw YAPETException (std::strerror (errno) );

    if (st_buf.st_size == 0)
        return true;

    return false;
}

/**
 * Modified in version 0.6.
 *
 * As of version 0.6, creates a header struct \c FileHeader_64 and calls \c writeHeader. It
 * then reads and decrypts the header in order to verify.
 *
 * @param key reference to the key used to encrypt the header.
 */
void
File::initFile (const Key& key) {
    Crypt crypt (key);
    Record<FileHeader_64> header;
    FileHeader_64* ptr = header;
    ptr->version = VERSION_2;
    std::memcpy (ptr->control, CONTROL_STR, HEADER_CONTROL_SIZE);
    ptr->pwset = int_to_disk<int64_t> (time (0) );
    mtime = lastModified();
    writeHeader (header, key);
    // Sanity checks
    BDBuffer* buff = readHeader();

    if (buff == 0)
        throw YAPETException (_ ("EOF encountered while reading header") );

    Record<FileHeader_64>* dec_hdr = crypt.decrypt<FileHeader_64> (*buff);
    FileHeader_64* ptr_dec_hdr = *dec_hdr;
    int retval = std::memcmp (ptr_dec_hdr->control, ptr->control, HEADER_CONTROL_SIZE);

    if (retval != 0)
        throw YAPETException (_ ("Sanity check for control field failed") );

    delete buff;
    delete dec_hdr;
}

/**
 * Modified in version 0.6.
 *
 * Encrypts and writes the file header provided to the file.
 *
 * Since version 0.6 it expects by default a FileHeader_64 header.
 *
 * @param header a reference to the header. It will be encrypted using
 * the key provided.
 *
 * @param key the key used to encrypt the header provided.
 */
void
File::writeHeader (const Record<FileHeader_64>& header, const Key& key)
{
    Crypt crypt (key);
    BDBuffer* buff = 0;

    try {
        buff = crypt.encrypt (header);
        writeHeader (*buff);
    } catch (YAPETException& ex) {
        if (buff != 0)
            delete buff;

        throw;
    } catch (...) {
        if (buff != 0)
            delete buff;

        throw YAPETException (_ ("Unknown exception catched") );
    }

    delete buff;
}

/**
 * Writes the recognition string at the beginning of the file and
 * eventually the provided header \c enc_header
 *
 * @param enc_header reference to a BDBuffer holding the encrypted
 * header
 */
void
File::writeHeader (const BDBuffer& enc_header) {
    seekAbs (0);
    // Write the recognition string
    ssize_t retval = ::write (fd, RECOG_STR, std::strlen (RECOG_STR) );

    if (retval == -1)
        throw YAPETException (std::strerror (errno) );

    if ( ( (size_t) retval) != std::strlen (RECOG_STR) )
        throw YAPETException (_ ("Short write on file: ") + filename);

    mtime = lastModified();
    write (enc_header);
}

/**
 * Reads the encrypted header from the file. It also checks whether or
 * not the file is of the expected type by reading the recognition
 * string. If the string read does not match the one predefined, it
 * throws an \c YAPETException.
 *
 * The memory occupied by the \c BDBuffer returned has to be freed by
 * the caller.
 *
 * @return pointer to a \c BDBuffer holding the encrypted file
 * header. The memory occupied has to be freed by the caller.
 */
BDBuffer*
File::readHeader() const {
    seekAbs (0);
    char* buff = (char*) alloca (std::strlen (RECOG_STR) );

    if (buff == 0)
        throw YAPETException (_ ("Memory exhausted") );

    int retval = ::read (fd, buff, std::strlen (RECOG_STR) );

    if (retval == -1)
        throw YAPETException (std::strerror (errno) );

    if ( ( (size_t) retval) != std::strlen (RECOG_STR) )
        throw YAPETException (_ ("File type not recognized") );

    retval = std::memcmp (RECOG_STR, buff, std::strlen (RECOG_STR) );

    if (retval != 0)
        throw YAPETException (_ ("File type not recognized") );

    return read();
}

/**
 * New in version 0.6.
 *
 * It returns the header decrypted. The caller have to check the pointers
 * returned for null values in order to determine which header to use, e.g.
 *
 @verbatim
 if (ptr32 != 0) {
    // do something
 } else if (ptr64 != 0) {
    // do something
 } else {
    // error
 }
 @endverbatim
 *
 * The memory allocated by the method for the records have to be freed by the
 * caller.
 *
 * @param key reference to a Key object used to decrypt the header.
 *
 * @param ptr32 the 32bit header record. If the yapet file does not contain a
 * FileHeader_32, 0 is returned.
 *
 * @param ptr64 the 64bit header record. If the yapet file does not contain a
 * FileHeader_64, 0 is returned.
 */
void
File::readHeader(const Key& key, Record<FileHeader_32>** ptr32, Record<FileHeader_64>** ptr64) const {
    assert(ptr32 != 0 && ptr64 != 0);
    if (ptr32 == 0)
	throw YAPETException(_("Null pointer passed in ptr32"), ZEROPOINTER);
    if (ptr64 == 0)
	throw YAPETException(_("Null pointer passed in ptr64"), ZEROPOINTER);

    Crypt crypt (key);
    BDBuffer* enc_header = 0;

    try {
        enc_header = readHeader();
	// First try to read a 32bit header
	try {
	    *ptr32 = crypt.decrypt<FileHeader_32> (*enc_header);
	} catch (YAPETException& ex) {

	    if (*ptr32 != 0) {
		delete *ptr32;
		*ptr32 = 0;
	    }

	    try {
		*ptr64 = crypt.decrypt<FileHeader_64> (*enc_header);
	    } catch (YAPETEncryptionException& ex) { // Catch invalid password
		if (enc_header != 0) {
		    delete enc_header;
		    enc_header = 0;
		}
		
		if (*ptr32 != 0) {
		    delete *ptr32;
		    *ptr32 = 0;
		}
		if (*ptr64 != 0) {
		    delete *ptr64;
		    *ptr64 = 0;
		}
		
		throw YAPETInvalidPasswordException();
	    } catch (YAPETException &ex) {
		// Ok, we got another problem
		if (*ptr64 != 0) {
		    delete *ptr64;
		    *ptr64 = 0;
		}
		throw;
	    }
	}
    } catch (YAPETEncryptionException& ex) { // Catch invalid password
        if (enc_header != 0) delete enc_header;

        if (*ptr32 != 0) {
	    delete *ptr32;
	    *ptr32 = 0;
	}
	if (*ptr64 != 0) {
	    delete *ptr64;
	    *ptr64 = 0;
	}

        throw YAPETInvalidPasswordException();
    } catch (YAPETException& ex) {
        if (enc_header != 0) delete enc_header;

        if (*ptr32 != 0) {
	    delete *ptr32;
	    *ptr32 = 0;
	}
	if (*ptr64 != 0) {
	    delete *ptr64;
	    *ptr64 = 0;
	}

        throw;
    }

    delete enc_header;
}

/**
 * Modified in version 0.6
 *
 * Validates the key provided by reading the file header, decrypting
 * it and comparing the control string of the file header with the
 * string \c RECOG_STR. If those operations succeed, the key is
 * considered valid. In case of an error, an exception is thrown.
 *
 * Since this method relies on \c readHeader(), the file type is
 * checked automatically.
 *
 * @param key the key to validate against the file.
 */
void
File::validateKey (const Key& key)
{
    // Expect either a 32bit or 64bit header
    Record<FileHeader_32>* dec_header_32 = 0;
    Record<FileHeader_64>* dec_header_64 = 0;

    readHeader(key, &dec_header_32, &dec_header_64);
    assert(dec_header_32 != 0 || dec_header_64 != 0);

    FileHeader_32* ptr_dec_header_32 = 
	(dec_header_32 != 0) ? static_cast<FileHeader_32*>(*dec_header_32) : 0;
    FileHeader_64* ptr_dec_header_64 = 
	(dec_header_64 != 0) ? static_cast<FileHeader_64*>(*dec_header_64) : 0;

    int retval;
    if (ptr_dec_header_32 != 0) {
	retval = std::memcmp (ptr_dec_header_32->control,
                         CONTROL_STR,
                         HEADER_CONTROL_SIZE);
    } else {
	assert(ptr_dec_header_64 != 0);
	retval = std::memcmp (ptr_dec_header_64->control,
                         CONTROL_STR,
                         HEADER_CONTROL_SIZE);
    }
    if (dec_header_32 != 0)
	delete dec_header_32;
    if (dec_header_64 != 0)
	delete dec_header_64;

    if (retval != 0)
        throw YAPETInvalidPasswordException();
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
File::File (const std::string& fn, const Key& key, bool create, bool secure)
    : filename (fn), mtime(0), usefsecurity (secure) {
    if (create)
        openCreate();
    else
        openNoCreate();

    if (isempty() ) {
        initFile (key);
    } else {
        validateKey (key);
    }
}

/**
 * Duplicates the file descriptor by calling \c dup().
 */
File::File (const File& f) {
    fd = dup (f.fd);

    if (fd == -1)
        throw YAPETException (std::strerror (errno) );

    filename = f.filename;
    mtime = f.mtime;
    usefsecurity = f.usefsecurity;
}

/**
 * Closes the file.
 */
File::~File() {
    close (fd);
}

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
void
File::save (const std::list<PartDec>& records, bool forcewrite) {
    if ( (mtime != lastModified() ) && !forcewrite)
        throw YAPETRetryException (_ ("File has been externally modified") );

    if (usefsecurity)
        setFileSecurity();

    preparePWSave();
    std::list<PartDec>::const_iterator it = records.begin();

    while (it != records.end() ) {
        write ( it->getEncRecord() );
        ++it;
    }
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
std::list<PartDec>
File::read (const Key& key) const {
    seekDataSection();
    BDBuffer* buff = 0;
    std::list<PartDec> retval;

    try {
        buff = read();

        while (buff != 0) {
            retval.push_back (PartDec (*buff, key) );
            delete buff;
            buff = read();
        }
    } catch (YAPETException& ex) {
        if (buff != 0)
            delete buff;

        throw;
    }

    return retval;
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
void
File::setNewKey (const Key& oldkey,
                 const Key& newkey,
		 bool forcewrite) {
    if ( (mtime != lastModified() ) && !forcewrite)
        throw YAPETRetryException (_ ("File has been externally modified") );

    close (fd);
    std::string backupfilename (filename + ".bak");
    int retval = rename (filename.c_str(), backupfilename.c_str() );

    if (retval == -1) {
        // Reopen the old file
        openNoCreate();
        throw YAPETException (std::strerror (errno) );
    }

    File* oldfile = 0;

    try {
        // Reopen the old (backup) file
        oldfile = new File (backupfilename, oldkey, false, false);
        // Initialize the (this) file with the new key
        openCreate();
        initFile (newkey);
        // Retrieve the records encrypted with the old key
        std::list<PartDec> entries = oldfile->read (oldkey);
        std::list<PartDec>::iterator it = entries.begin();
        Crypt oldcrypt (oldkey);
        Crypt newcrypt (newkey);

        while (it != entries.end() ) {
            Record<PasswordRecord>* dec_rec_ptr = 0;
            BDBuffer* new_enc_rec = 0;

            try {
                // Decrypt with the old key
                const BDBuffer old_enc_rec = (*it).getEncRecord();
                dec_rec_ptr =
                    oldcrypt.decrypt<PasswordRecord> (old_enc_rec);
                new_enc_rec =
                    newcrypt.encrypt (*dec_rec_ptr);
                write (*new_enc_rec);
                delete dec_rec_ptr;
                delete new_enc_rec;
            } catch (YAPETException& ex) {
                if (dec_rec_ptr != 0)
                    delete dec_rec_ptr;

                if (new_enc_rec != 0)
                    delete new_enc_rec;

                throw;
            }

            ++it;
        }
    } catch (YAPETException& ex) {
        if (oldfile != 0)
            delete oldfile;

        throw;
    }

    delete oldfile;
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
int64_t
File::getMasterPWSet (const Key& key) const {
    // Expect either a 32bit or 64bit header
    Record<FileHeader_32>* dec_header_32 = 0;
    Record<FileHeader_64>* dec_header_64 = 0;

    readHeader(key, &dec_header_32, &dec_header_64);
    assert(dec_header_32 != 0 || dec_header_64 != 0);

    FileHeader_32* ptr_dec_header_32 = 
	(dec_header_32 != 0) ? static_cast<FileHeader_32*>(*dec_header_32) : 0;
    FileHeader_64* ptr_dec_header_64 = 
	(dec_header_64 != 0) ? static_cast<FileHeader_64*>(*dec_header_64) : 0;

    int64_t t;
    if (ptr_dec_header_32 != 0) {
	t = int_from_disk<int32_t> (ptr_dec_header_32->pwset);
    } else {
	assert(ptr_dec_header_64 != 0);
	t = int_from_disk<int64_t> (ptr_dec_header_64->pwset);
    }

    if (dec_header_32 != 0)
	delete dec_header_32;
    if (dec_header_64 != 0)
	delete dec_header_64;

    return t;
}

/**
 * New since version 0.6.
 *
 * Return the file version.
 */
FILE_VERSION
File::getFileVersion(const Key& key) const {
    // Expect either a 32bit or 64bit header
    Record<FileHeader_32>* dec_header_32 = 0;
    Record<FileHeader_64>* dec_header_64 = 0;

    readHeader(key, &dec_header_32, &dec_header_64);
    assert(dec_header_32 != 0 || dec_header_64 != 0);

    FileHeader_32* ptr_dec_header_32 = 
	(dec_header_32 != 0) ? static_cast<FileHeader_32*>(*dec_header_32) : 0;
    FileHeader_64* ptr_dec_header_64 = 
	(dec_header_64 != 0) ? static_cast<FileHeader_64*>(*dec_header_64) : 0;

    FILE_VERSION v;
    if (ptr_dec_header_32 != 0) {
	v = static_cast<FILE_VERSION>(ptr_dec_header_32->version);
    } else {
	assert(ptr_dec_header_64 != 0);
	v = static_cast<FILE_VERSION>(ptr_dec_header_64->version);
    }

    if (dec_header_32 != 0)
	delete dec_header_32;
    if (dec_header_64 != 0)
	delete dec_header_64;

    return v;
}

const File&
File::operator= (const File & f) {
    if (this == &f) return *this;

    close (fd);
    fd = dup (f.fd);

    if (fd == -1)
        throw YAPETException (std::strerror (errno) );

    filename = f.filename;
    return *this;
}