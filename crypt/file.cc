// $Id$

#ifdef TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# ifdef HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <string.h>

#include "crypt.h"
#include "record.h"
#include "structs.h"
#include "file.h"

using namespace GPSAFE;

#define CONTROL_STR "ABCDEFGHIJKLMNOPQRSTUVW"

void
File::openCreate() throw(GPSException) {
    fd = ::open(filename.c_str(), O_RDWR | O_CREAT | O_TRUNC | O_APPEND, S_IRUSR | S_IWUSR);
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
    seekAbs(0);
    size_t len;
    int retval = ::read(fd, &len, sizeof(size_t));
    if (retval == -1)
	throw GPSException(strerror(errno));

    if (retval < sizeof(size_t))
	throw GPSException("Unable to seek to data section");

    seekCurr(len);
}

BDBuffer*
File::read() const throw(GPSException) {
    size_t len;
    int retval = ::read(fd, &len, sizeof(size_t));
    if (retval == -1)
	throw GPSException(strerror(errno));

    if (retval == 0)
	return NULL;

    if (retval < sizeof(size_t) )
	throw GPSException("Short read on file: " + filename);

    BDBuffer* buf = new BDBuffer(len);
    retval = ::read(fd, *buf, len);
    if (retval == -1) 
	throw GPSException(strerror(errno));

    if (retval == 0) {
	delete buf;
	return NULL;
    }

    if (retval < len) {
	delete buf;
	throw GPSException("Short read on file: " + filename);
    }

    return buf;
}

void
File::write(const BDBuffer& buff, bool append, bool force) throw(GPSException, GPSRetryException) {
    if ( (mtime != lastModified()) && !force)
	throw GPSRetryException("File has been modified");

    if (append) {
	off_t pos = lseek(fd, 0, SEEK_END);
	if ( ((off_t)-1) == pos)
	    throw GPSException(strerror(errno));
    }
    size_t s = buff.size();
    int retval = ::write(fd, &s, sizeof(size_t));
    if (retval == -1)
	throw GPSException(strerror(errno));
    if (retval != sizeof(size_t) )
	throw GPSException("Short write on file: " + filename);

    retval = ::write(fd, buff, buff.size());
    if (retval == -1)
	throw GPSException(strerror(errno));
    
    if (retval < buff.size()) 
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
File::writeHeader(const Record<FileHeader>& header, const Key& key) throw(GPSException) {
    seekAbs(0);

    Crypt crypt(key);
    BDBuffer* buff = NULL;
    try {
	buff = crypt.encrypt(header);
	write(*buff);
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

    write(enc_header);
}

BDBuffer*
File::readHeader() const throw(GPSException) {
    seekAbs(0);
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
    throw(GPSException) : filename(fn),
			  isopen(true) {
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
