// -*- c++ -*-
//
// $Id$
//
// YAPET -- Yet Another Password Encryption Tool
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

#ifndef _FILE_H
#define _FILE_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_STRING
# include <string>
#endif

#ifdef HAVE_LIST
# include <list>
#endif

#include "yapetexception.h"

#include "bdbuffer.h"
#include "structs.h"
#include "key.h"
#include "partdec.h"

namespace YAPET {
    /**
     * @brief Class for storing and retrieving encrypted data to and
     * from disk
     *
     * This class takes care of storing and retrieving encrypted
     * password records to and from disk.
     *
     * Each file created by this class starts with a recognition
     * string which currently consists of the 8 bytes "YAPET1.0" as
     * depicted below.
     *
@verbatim
+--------+--------+--------+--------+--------+--------+--------+--------+
|   Y    |   A    |   P    |   E    |   T    |   1    |   .    |   0    |
| 1 Byte | 1 Byte | 1 Byte | 1 Byte | 1 Byte | 1 Byte | 1 Byte | 1 Byte |
+--------+--------+--------+--------+--------+--------+--------+--------+
@endverbatim
     *
     * After the recognition string an header totalling in 25 Bytes
     * follows. The first byte indicates the version of the file. The
     * next 20 Bytes are used as control string. After decryption, the
     * control string is compared to the predefined clear text control
     * string, to find out whether or not the key used to decrypt was
     * the one used to encrypt.
     *
@verbatim
+--------+
|Version |
| 1 Byte |
+--------+--------+--------+--...---+
|          Control String           |
|             20 Bytes              |
+--------+--------+--------+--...---+
|  Time when the Password  |
|    was set (4 Bytes)     |
+--------+--------+--------+
@endverbatim
    *
    * Each encrypted password record is prefixed by a 4 Byte unsigned
    * integer which is stored in big-endian order. The methods take
    * care returning them in the appropriate order of the host
    * system. This integer is used to indicate the length of the
    * following encrypted data chunk.
    *
@verbatim
+--------+--------+--------+--------+
|   Length indicator in big-endian  |
|         order (4 Bytes)           |
+--------+--------+--------+--------+--...---+
|  Encrypted password record of exactly as   |
|   many Bytes as indicated by the prefix    |
+--------+--------+--------+--------+--...---+
|   Length indicator in big-endian  |
|         order (4 Bytes)           |
+--------+--------+--------+--------+--...---+
|  Encrypted password record of exactly as   |
|   many Bytes as indicated by the prefix    |
+--------+--------+--------+--------+--...---+
              [ . . . ]
@endverbatim
    *
    * Putting this together, the file looks like this
    *
@verbatim
+--------+--------+--------+--------+--------+--------+--------+--------+
|   Y    |   A    |   P    |   E    |   T    |   1    |   .    |   0    |
| 1 Byte | 1 Byte | 1 Byte | 1 Byte | 1 Byte | 1 Byte | 1 Byte | 1 Byte |
+--------+--------+--------+--------+--------+--------+--------+--------+
|Version |
| 1 Byte |
+--------+--------+--------+--...---+
|          Control String           |
|             20 Bytes              |
+--------+--------+--------+--...---+
|  Time when the Password  |
|    was set (4 Bytes)     |
+--------+--------+--------+--------+
|   Length indicator in big-endian  |
|         order (4 Bytes)           |
+--------+--------+--------+--------+--...---+
|  Encrypted password record of exactly as   |
|   many Bytes as indicated by the prefix    |
+--------+--------+--------+--------+--...---+
|   Length indicator in big-endian  |
|         order (4 Bytes)           |
+--------+--------+--------+--------+--...---+
|  Encrypted password record of exactly as   |
|   many Bytes as indicated by the prefix    |
+--------+--------+--------+--------+--...---+
              [ . . . ]
@endverbatim
    *
    * Instances of this class keeps the file open for the lifetime
    * of the instance.
    */
    class File {
	private:
	    /**
	     * @brief the file descriptor of the password file
	     *
	     * The file descriptor of the password file.
	     */
	    int fd;
	    std::string filename;
	    time_t mtime;

	    void openCreate() throw(YAPETException);
	    void openNoCreate() throw(YAPETException);

	    time_t lastModified() const throw(YAPETException);

	    void seekCurr(off_t offset) const throw(YAPETException);
	    void seekAbs(off_t offset) const throw(YAPETException);

	    void preparePWSave() throw(YAPETException);

	protected:
	    struct WORD {
		    uint8_t a;
		    uint8_t b;
	    };

	    struct DWORD {
		    WORD a;
		    WORD b;
	    };

	    union ENDIAN {
		    uint32_t abcd;
		    DWORD dword;
	    };

#ifndef WORDS_BIGENDIAN
	    //! The given integer will be converted to big endian
	    uint32_t uint32_to_disk(uint32_t i) const;
	    //! The given integer will be converted to the endianess of the
	    //! platform
	    uint32_t uint32_from_disk(uint32_t i) const;
#else
	    inline uint32_t uint32_to_disk(uint32_t i) const { return i; }
	    inline uint32_t uint32_from_disk(uint32_t i) const { return i; }
#endif // WORDS_BIGENDIAN

	    void seekDataSection() const throw(YAPETException);

	    BDBuffer* read() const throw(YAPETException);

	    void write(const BDBuffer& buff,
		       bool forceappend=false,
		       bool forcewrite=false)
		throw(YAPETException, YAPETRetryException);

	    bool isempty() const throw(YAPETException);

	    void initFile(const Key& key) throw(YAPETException);

	    void writeHeader(const Record<FileHeader>& header,
			     const Key& key)
		throw(YAPETException);

	    void writeHeader(const BDBuffer& enc_header) throw(YAPETException);
	    BDBuffer* readHeader() const throw(YAPETException);

	    void validateKey(const Key& key)
		throw(YAPETException,YAPETInvalidPasswordException);

	public:
	    File(const std::string& fn,
		 const Key& key,
		 bool create=false)
		throw(YAPETException);
	    File(const File& f) throw(YAPETException);
	    ~File();

	    void save(std::list<PartDec>& records) throw(YAPETException);
	    std::list<PartDec> read(const Key& key) const throw(YAPETException);
	    std::string getFilename() const { return filename; }
	    void setNewKey(const Key& oldkey, const Key& newkey)
		throw(YAPETException);

	    const File& operator=(const File& f) throw(YAPETException);
    };
}
#endif // _FILE_H
