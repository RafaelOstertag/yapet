// $Id$
//
// @@REPLACE@@
// Copyright (C) 2008  Rafael Ostertag
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

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

using namespace GPSAFE;

#define CONTROL_STR "ABCDEFGHIJKLMNOPQRSTUVW"

char recog_string[] = "GPS1.0";

void
File::openCreate() throw(GPSException) {
    fd = ::open(filename.c_str(),
		O_RDWR | O_CREAT | O_TRUNC | O_APPEND, S_IRUSR | S_IWUSR);
    if (fd == -1)
	throw GPSException(strerror(errno));
}

void
File::openNoCreate() throw(GPSException) {
    fd = ::open(filename.c_str(), O_RDWR | O_APPEND);
    if (fd == -1)
	throw GPSException(strerror(errno));
}

time_t
File::lastModified() const throw(GPSException){
    struct stat st_buf;
    int retval = fstat(fd, &st_buf);
    if (retval == -1)
	throw GPSException(strerror(errno));
    
    return st_buf.st_mtime;
}

void
File::seekCurr(off_t offset) const throw(GPSException) {
    off_t pos = lseek(fd, offset, SEEK_CUR);
    if ( ((off_t)-1) == pos)
	throw GPSException(strerror(errno));
}

void
File::seekAbs(off_t offset) const throw(GPSException) {
    off_t pos = lseek(fd, offset, SEEK_SET);
    if ( ((off_t)-1) == pos)
	throw GPSException(strerror(errno));

    if (pos != offset)
	throw GPSException("Error seeking within file: " + filename);
}

/** Truncates the file up to the header
 *
 * Truncates the file up to the header by creating a new empty file
 * copying over the existing header.
 */
void
File::preparePWSave() throw(GPSException) {
    BDBuffer* curr_header = readHeader();

    ::close(fd);
    try {
	openCreate();
    } catch (GPSException& ex) {
	if (curr_header != NULL)
	    delete curr_header;
	throw;
    }

    mtime = lastModified();
    writeHeader(*curr_header);
    delete curr_header;
}

void
File::seekDataSection() const throw(GPSException) {
    seekAbs(strlen(recog_string));
    uint32_t len;
    int retval = ::read(fd, &len, sizeof(uint32_t));
    if (retval == -1)
	throw GPSException(strerror(errno));

    if ( ((size_t)retval) != sizeof(uint32_t))
	throw GPSException("Unable to seek to data section");

    len = uint32_from_disk(len);

    seekCurr(len);
}

#ifndef WORDS_BIGENDIAN
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
 * The same swapping as \c uint32_to_disk().
 */
uint32_t
File::uint32_from_disk(uint32_t i) const {
    return uint32_to_disk(i);
}
#endif // WORDS_BIGENDIAN

BDBuffer*
File::read() const throw(GPSException) {
    uint32_t len;
    int retval = ::read(fd, &len, sizeof(uint32_t));
    if (retval == -1)
	throw GPSException(strerror(errno));

    if (retval == 0)
	return NULL;

    if ( ((size_t)retval) < sizeof(uint32_t) )
	throw GPSException("Short read on file: " + filename);

    // Convert len to the endianess of the architecture
    len = uint32_from_disk(len);

    BDBuffer* buf = new BDBuffer(len);
    retval = ::read(fd, *buf, len);
    if (retval == -1) 
	throw GPSException(strerror(errno));

    if (retval == 0) {
	delete buf;
	return NULL;
    }

    if (((uint32_t)retval) < len) {
	delete buf;
	throw GPSException("Short read on file: " + filename);
    }

    return buf;
}

void
File::write(const BDBuffer& buff, bool append, bool force) 
    throw(GPSException, GPSRetryException) {
    if ( (mtime != lastModified()) && !force)
	throw GPSRetryException("File has been modified");

    if (append) {
	off_t pos = lseek(fd, 0, SEEK_END);
	if ( ((off_t)-1) == pos)
	    throw GPSException(strerror(errno));
    }
    uint32_t s = buff.size();

    // Convert s to the on-disk structure
    s = uint32_to_disk(s);

    int retval = ::write(fd, &s, sizeof(uint32_t));
    if (retval == -1)
	throw GPSException(strerror(errno));
    if (retval != sizeof(uint32_t) )
	throw GPSException("Short write on file: " + filename);

    retval = ::write(fd, buff, buff.size());
    if (retval == -1)
	throw GPSException(strerror(errno));
    
    if (((size_t)retval) < buff.size()) 
	throw GPSException("Short write on file: " + filename);

    mtime = lastModified();
}

bool
File::isempty() const throw(GPSException){
    struct stat st_buf;
    int retval = fstat(fd, &st_buf);
    if (retval == -1)
	throw GPSException(strerror(errno));

    if (st_buf.st_size == 0) 
	return true;
    
    return false;
}

void
File::initFile(const Key& key) throw(GPSException) {
    Crypt crypt(key);

    Record<FileHeader> header;
    FileHeader* ptr = header;
    ptr->version = 1;
    memcpy(ptr->control, CONTROL_STR, HEADER_CONTROL_SIZE);
    ptr->pwset = time(NULL);

    mtime = lastModified();

    writeHeader(header, key);

    // Sanity checks
    BDBuffer* buff = readHeader();
    if (buff == NULL)
	throw GPSException("EOF encountered while reading header");

    Record<FileHeader>* dec_hdr = crypt.decrypt<FileHeader>(*buff);

    FileHeader* ptr_dec_hdr = *dec_hdr;

    int retval = memcmp(ptr_dec_hdr->control, ptr->control, HEADER_CONTROL_SIZE);
    if (retval != 0)
	throw GPSException("Sanity check for control field failed");

    delete buff;
    delete dec_hdr;
}

void
File::writeHeader(const Record<FileHeader>& header, const Key& key) 
    throw(GPSException) {

    Crypt crypt(key);
    BDBuffer* buff = NULL;
    try {
	buff = crypt.encrypt(header);
	writeHeader(*buff);
    } catch (GPSException& ex) {
	if (buff != NULL)
	    delete buff;
	throw;
    } catch (...) {
	if (buff != NULL)
	    delete buff;

	throw GPSException("Unknown exception catched");
    }

    delete buff;
}

void
File::writeHeader(const BDBuffer& enc_header) throw(GPSException) {
    seekAbs(0);

    // Write the recognition string
    ssize_t retval = ::write(fd, recog_string, strlen(recog_string));
    if (retval == -1)
	throw GPSException(strerror(errno));

    if (((size_t)retval) != strlen(recog_string) )
	throw GPSException("Short write on file " + filename);

    write(enc_header);
}

BDBuffer*
File::readHeader() const throw(GPSException) {
    seekAbs(0);

    char* buff = (char*) alloca(strlen(recog_string));
    if (buff == NULL)
	throw GPSException("Memory exhausted");

    int retval = ::read(fd, buff, strlen(recog_string));
    if (retval == -1)
	throw GPSException(strerror(errno));

    if (((size_t)retval) != strlen(recog_string) )
	throw GPSException("File type not recognized");

    retval = memcmp(recog_string, buff, strlen(recog_string));
    if (retval != 0)
	throw GPSException("File type not recognized");
    
    return read();
}

void
File::validateKey(const Key& key) throw(GPSException,GPSInvalidPasswordException) {
    Crypt crypt(key);
    BDBuffer* enc_header = NULL;
    Record<FileHeader>* dec_header = NULL;
    FileHeader* ptr_dec_header = NULL;

    try {
	enc_header = readHeader();
	dec_header = crypt.decrypt<FileHeader>(*enc_header);
	ptr_dec_header = *dec_header;
    } catch (GPSEncryptionException& ex) {
	if (enc_header != NULL) delete enc_header;
	if (dec_header != NULL) delete dec_header;
	throw GPSInvalidPasswordException();
    } catch (GPSException& ex) {
	if (enc_header != NULL) delete enc_header;
	if (dec_header != NULL) delete dec_header;
	throw;
    }

    int retval = memcmp(ptr_dec_header->control, CONTROL_STR, HEADER_CONTROL_SIZE);
    delete enc_header;
    delete dec_header;
    if (retval != 0)
	throw GPSInvalidPasswordException();
}

File::File(const std::string& fn, const Key& key, bool create) 
    throw(GPSException) : isopen(true),
			  filename(fn) {
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

File::File(const File& f) throw(GPSException) {
    if (f.isopen) {
	fd = dup(f.fd);
	if (fd == -1)
	    throw GPSException(strerror(errno));
    } else {
	fd = -1;
    }

    isopen = f.isopen;
    filename = f.filename;
    mtime = f.mtime;
}

File::~File() {
    if (isopen)
	close(fd);
}

void
File::save(std::list<PartDec>& records) throw(GPSException) {
    preparePWSave();
    std::list<PartDec>::iterator it = records.begin();
    while (it != records.end() ) {
	write( it->getEncRecord());
	it++;
    }
}

std::list<PartDec>
File::read(const Key& key) const throw(GPSException) {
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
    } catch (GPSException& ex) {
	if (buff != NULL)
	    delete buff;
	throw;
    }

    return retval;
}


const File&
File::operator=(const File& f) throw(GPSException) {
    if (this == &f) return *this;

    if (isopen)
	close(fd);

    if (f.isopen) {
	fd = dup(f.fd);
	if (fd == -1)
	    throw GPSException(strerror(errno));
    } else {
	fd = -1;
    }

    isopen = f.isopen;
    filename = f.filename;

    return *this;
}
