// $Id$
//
// Copyright (C) 2008, 2009  Rafael Ostertag
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

#include "../intl.h"
#include "crypt.h"
#include "record.h"
#include "structs.h"
#include "file.h"

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

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif

#ifdef HAVE_SYS_STAT_H
# include <sys/stat.h>
#endif

#ifdef HAVE_FCNTL_H
# include <fcntl.h>
#endif

#ifdef HAVE_STRING_H
# include <string.h>
#endif

#ifdef HAVE_ERRNO_H
# include <errno.h>
#endif

#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

#ifdef HAVE_ALLOCA_H
# include <alloca.h>
#elif defined __GNUC__
# define alloca __builtin_alloca
#elif defined _AIX
# define alloca __alloca
#elif defined _MSC_VER
# include <malloc.h>
# define alloca _alloca
#else
# include <stddef.h>
#endif

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
 * Creates a file with name specified in \c filename and sets \c fd to
 * the obtained file descriptor. The file is opened for read-write. It
 * is created with only the user having read-write access.
 */
void
File::openCreate() throw(YAPETException) {
    fd = ::open(filename.c_str(),
		O_RDWR | O_CREAT | O_TRUNC | O_APPEND, S_IRUSR | S_IWUSR);
    if (fd == -1)
	throw YAPETException(strerror(errno));
}

/**
 * Opens an existing file in read-write mode. If the file does not
 * exist, the method throws an exception.
 */
void
File::openNoCreate() throw(YAPETException) {
    fd = ::open(filename.c_str(), O_RDWR | O_APPEND);
    if (fd == -1)
	throw YAPETException(strerror(errno));
}

/**
 * Returns the time of the last modification of the file specified by
 * \c fd.
 *
 * @return a \c time_t holdign the last modification date.
 */
time_t
File::lastModified() const throw(YAPETException){
    struct stat st_buf;
    int retval = fstat(fd, &st_buf);
    if (retval == -1)
	throw YAPETException(strerror(errno));

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
File::seekCurr(off_t offset) const throw(YAPETException) {
    off_t pos = lseek(fd, offset, SEEK_CUR);
    if ( ((off_t)-1) == pos)
	throw YAPETException(strerror(errno));
}

/**
 * Seeks \c offset bytes from the beginning of the file.
 *
 * @param offset the offset in bytes from the beginning.
 */
void
File::seekAbs(off_t offset) const throw(YAPETException) {
    off_t pos = lseek(fd, offset, SEEK_SET);
    if ( ((off_t)-1) == pos)
	throw YAPETException(strerror(errno));

    if (pos != offset)
	throw YAPETException(_("Error seeking within file: ") + filename);
}

/**
 * Truncates the file up to the header by creating a new empty file
 * copying over the existing header.
 */
void
File::preparePWSave() throw(YAPETException) {
    BDBuffer* curr_header = readHeader();

    ::close(fd);
    try {
	openCreate();
    } catch (YAPETException& ex) {
	if (curr_header != NULL)
	    delete curr_header;
	throw;
    }

    mtime = lastModified();
    writeHeader(*curr_header);
    delete curr_header;
}

/**
 * Seeks to the first password record in the file. The file pointer is
 * set to point to the record length indicator, so that a call to \c
 * read() will return the first password record.
 */
void
File::seekDataSection() const throw(YAPETException) {
    seekAbs(strlen(RECOG_STR));
    uint32_t len;
    int retval = ::read(fd, &len, sizeof(uint32_t));
    if (retval == -1)
	throw YAPETException(strerror(errno));

    if ( ((size_t)retval) != sizeof(uint32_t))
	throw YAPETException(_("Unable to seek to data section"));

    len = uint32_from_disk(len);

    seekCurr(len);
}

#ifndef WORDS_BIGENDIAN
/**
 * Converts the length indicator provided to the big endian byte
 * order, suitable for writing to disk.
 *
 * @param i the length indicator in host byte order
 *
 * @return an unsigned 32 bits integer in big-endian format.
 */
uint32_t
File::uint32_to_disk(uint32_t i) const {
    ENDIAN endian;
    endian.abcd = i;

    uint8_t tmp = endian.dword.b.b;
    endian.dword.b.b = endian.dword.a.a;
    endian.dword.a.a = tmp;

    tmp = endian.dword.b.a;
    endian.dword.b.a = endian.dword.a.b;
    endian.dword.a.b = tmp;

    return endian.abcd;
}

/**
 * Converts the length indicator read from the file to the host byte
 * order. The length indicator is always stored in big endian order.
 *
 * @param i the length indicator as read from the file
 *
 * @return an unsigned 32 bits integer in host byte order.
 */
uint32_t
File::uint32_from_disk(uint32_t i) const {
    return uint32_to_disk(i);
}
#endif // WORDS_BIGENDIAN

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
 * caller of the method. It returns \c NULL when the end of file has
 * been reached.
 */
BDBuffer*
File::read() const throw(YAPETException) {
    uint32_t len;
    int retval = ::read(fd, &len, sizeof(uint32_t));
    if (retval == -1)
	throw YAPETException(strerror(errno));

    if (retval == 0)
	return NULL;

    if ( ((size_t)retval) < sizeof(uint32_t) )
	throw YAPETException(_("Short read on file: ") + filename);

    // Convert len to the endianess of the architecture
    len = uint32_from_disk(len);

    BDBuffer* buf = new BDBuffer(len);
    retval = ::read(fd, *buf, len);
    if (retval == -1)
	throw YAPETException(strerror(errno));

    if (retval == 0) {
	delete buf;
	return NULL;
    }

    if (((uint32_t)retval) < len) {
	delete buf;
	throw YAPETException(_("Short read on file: ") + filename);
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
 */
void
File::write(const BDBuffer& buff, bool forceappend, bool forcewrite)
    throw(YAPETException, YAPETRetryException) {
    if ( (mtime != lastModified()) && !forcewrite)
	throw YAPETRetryException(_("File has been modified"));

    if (forceappend) {
	off_t pos = lseek(fd, 0, SEEK_END);
	if ( ((off_t)-1) == pos)
	    throw YAPETException(strerror(errno));
    }
    uint32_t s = buff.size();

    // Convert s to the on-disk structure
    s = uint32_to_disk(s);

    int retval = ::write(fd, &s, sizeof(uint32_t));
    if (retval == -1)
	throw YAPETException(strerror(errno));
    if (retval != sizeof(uint32_t) )
	throw YAPETException(_("Short write on file: ") + filename);

    retval = ::write(fd, buff, buff.size());
    if (retval == -1)
	throw YAPETException(strerror(errno));

    if (((size_t)retval) < buff.size())
	throw YAPETException(_("Short write on file: ") + filename);

    mtime = lastModified();
}

/**
 * Indicates whether or not the file specified by \c fd is empty.
 *
 * @return \c true if the file's size is zero, \c false otherwise.
 */
bool
File::isempty() const throw(YAPETException){
    struct stat st_buf;
    int retval = fstat(fd, &st_buf);
    if (retval == -1)
	throw YAPETException(strerror(errno));

    if (st_buf.st_size == 0)
	return true;

    return false;
}

/**
 * Creates a header struct \c FileHeader and calls \c writeHeader. It
 * then reads and decrypts the header in order to verify.
 *
 * @param key reference to the key used to encrypt the header.
 */
void
File::initFile(const Key& key) throw(YAPETException) {
    Crypt crypt(key);

    Record<FileHeader> header;
    FileHeader* ptr = header;
    ptr->version = 1;
    memcpy(ptr->control, CONTROL_STR, HEADER_CONTROL_SIZE);
    ptr->pwset = uint32_to_disk(time(NULL));

    mtime = lastModified();

    writeHeader(header, key);

    // Sanity checks
    BDBuffer* buff = readHeader();
    if (buff == NULL)
	throw YAPETException(_("EOF encountered while reading header"));

    Record<FileHeader>* dec_hdr = crypt.decrypt<FileHeader>(*buff);

    FileHeader* ptr_dec_hdr = *dec_hdr;

    int retval = memcmp(ptr_dec_hdr->control, ptr->control, HEADER_CONTROL_SIZE);
    if (retval != 0)
	throw YAPETException(_("Sanity check for control field failed"));

    delete buff;
    delete dec_hdr;
}

/**
 * Encrypts and writes the file header provided to the file.
 *
 * @param header a reference to the header. It will be encrypted using
 * the key provided.
 *
 * @param key the key used to encrypt the header provided.
 */
void
File::writeHeader(const Record<FileHeader>& header, const Key& key)
    throw(YAPETException) {

    Crypt crypt(key);
    BDBuffer* buff = NULL;
    try {
	buff = crypt.encrypt(header);
	writeHeader(*buff);
    } catch (YAPETException& ex) {
	if (buff != NULL)
	    delete buff;
	throw;
    } catch (...) {
	if (buff != NULL)
	    delete buff;

	throw YAPETException(_("Unknown exception catched"));
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
File::writeHeader(const BDBuffer& enc_header) throw(YAPETException) {
    seekAbs(0);

    // Write the recognition string
    ssize_t retval = ::write(fd, RECOG_STR, strlen(RECOG_STR));
    if (retval == -1)
	throw YAPETException(strerror(errno));

    if (((size_t)retval) != strlen(RECOG_STR) )
	throw YAPETException(_("Short write on file: ") + filename);

    mtime = lastModified();

    write(enc_header);
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
File::readHeader() const throw(YAPETException) {
    seekAbs(0);

    char* buff = (char*) alloca(strlen(RECOG_STR));
    if (buff == NULL)
	throw YAPETException(_("Memory exhausted"));

    int retval = ::read(fd, buff, strlen(RECOG_STR));
    if (retval == -1)
	throw YAPETException(strerror(errno));

    if (((size_t)retval) != strlen(RECOG_STR) )
	throw YAPETException(_("File type not recognized"));

    retval = memcmp(RECOG_STR, buff, strlen(RECOG_STR));
    if (retval != 0)
	throw YAPETException(_("File type not recognized"));

    return read();
}

/**
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
File::validateKey(const Key& key)
    throw(YAPETException,YAPETInvalidPasswordException) {

    Crypt crypt(key);
    BDBuffer* enc_header = NULL;
    Record<FileHeader>* dec_header = NULL;
    FileHeader* ptr_dec_header = NULL;

    try {
	enc_header = readHeader();
	dec_header = crypt.decrypt<FileHeader>(*enc_header);
	ptr_dec_header = *dec_header;
    } catch (YAPETEncryptionException& ex) {
	if (enc_header != NULL) delete enc_header;
	if (dec_header != NULL) delete dec_header;
	throw YAPETInvalidPasswordException();
    } catch (YAPETException& ex) {
	if (enc_header != NULL) delete enc_header;
	if (dec_header != NULL) delete dec_header;
	throw;
    }

    int retval = memcmp(ptr_dec_header->control,
			CONTROL_STR,
			HEADER_CONTROL_SIZE);
    delete enc_header;
    delete dec_header;
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
 */
File::File(const std::string& fn, const Key& key, bool create)
    throw(YAPETException) : filename(fn) {
    if (create)
	openCreate();
    else
	openNoCreate();

    if (isempty()) {
	initFile(key);
    } else {
	validateKey(key);
    }
}

/**
 * Duplicates the file descriptor by calling \c dup().
 */
File::File(const File& f) throw(YAPETException) {
    fd = dup(f.fd);
    if (fd == -1)
	throw YAPETException(strerror(errno));

    filename = f.filename;
    mtime = f.mtime;
}

/**
 * Closes the file.
 */
File::~File() {
    close(fd);
}

/**
 * Stores the list of \c PartDec records in the file.
 *
 * @param records list of \c PartDec records
 *
 * @sa PartDec
 */
void
File::save(std::list<PartDec>& records) throw(YAPETException) {
    preparePWSave();
    std::list<PartDec>::iterator it = records.begin();
    while (it != records.end() ) {
	write( it->getEncRecord());
	it++;
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
File::read(const Key& key) const throw(YAPETException) {
    seekDataSection();

    BDBuffer* buff = NULL;
    std::list<PartDec> retval;

    try {
	buff = read();
	while (buff != NULL) {
	    retval.push_back(PartDec(*buff, key));
	    delete buff;
	    buff = read();
	}
    } catch (YAPETException& ex) {
	if (buff != NULL)
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
File::setNewKey(const Key& oldkey,
		const Key& newkey) throw (YAPETException) {
    close(fd);
    std::string backupfilename(filename + ".bak");
    int retval = rename(filename.c_str(), backupfilename.c_str());
    if (retval == -1) {
	// Reopen the old file
	openNoCreate();
	throw YAPETException(strerror(errno));
    }


    File* oldfile = NULL;
    try {
	// Reopen the old (backup) file
	oldfile = new File(backupfilename, oldkey, false);
	// Initialize the (this) file with the new key
	openCreate();
	initFile(newkey);

	// Retrieve the records encrypted with the old key
	std::list<PartDec> entries = oldfile->read(oldkey);
	std::list<PartDec>::iterator it = entries.begin();
	Crypt oldcrypt(oldkey);
	Crypt newcrypt(newkey);
	while (it != entries.end() ) {
	    Record<PasswordRecord>* dec_rec_ptr = NULL;
	    BDBuffer* new_enc_rec = NULL;
	    try {
		// Decrypt with the old key
		const BDBuffer old_enc_rec = (*it).getEncRecord();
		dec_rec_ptr =
		    oldcrypt.decrypt<PasswordRecord>(old_enc_rec);
		new_enc_rec =
		    newcrypt.encrypt(*dec_rec_ptr);
		write(*new_enc_rec);
		delete dec_rec_ptr;
		delete new_enc_rec;
	    } catch (YAPETException& ex) {
		if (dec_rec_ptr != NULL)
		    delete dec_rec_ptr;
		if (new_enc_rec != NULL)
		    delete new_enc_rec;
		throw;
	    }
	    it++;
	}
    } catch (YAPETException& ex) {
	if (oldfile != NULL)
	    delete oldfile;
	throw;
    }
    delete oldfile;
}

/**
 * Returns the time as a \c time_t when the master password was set.
 *
 * @param key the key used to decrypt the header.
 *
 * @return a \c time_t representing the time the master password was
 * set.
 */
time_t
File::getMasterPWSet(const Key& key) const
    throw(YAPETException,YAPETInvalidPasswordException) {
    Crypt crypt(key);
    BDBuffer* enc_header = NULL;
    Record<FileHeader>* dec_header = NULL;
    FileHeader* ptr_dec_header = NULL;

    try {
	enc_header = readHeader();
	dec_header = crypt.decrypt<FileHeader>(*enc_header);
	ptr_dec_header = *dec_header;
    } catch (YAPETEncryptionException& ex) {
	if (enc_header != NULL) delete enc_header;
	if (dec_header != NULL) delete dec_header;
	throw YAPETInvalidPasswordException();
    } catch (YAPETException& ex) {
	if (enc_header != NULL) delete enc_header;
	if (dec_header != NULL) delete dec_header;
	throw;
    }

    time_t t = uint32_from_disk(ptr_dec_header->pwset);
    delete enc_header;
    delete dec_header;

    return t;
}


const File&
File::operator=(const File& f) throw(YAPETException) {
    if (this == &f) return *this;

    close(fd);

    fd = dup(f.fd);
    if (fd == -1)
	throw YAPETException(strerror(errno));

    filename = f.filename;

    return *this;
}
