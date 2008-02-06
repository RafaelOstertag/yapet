// -*- c++ -*-
//
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

#include "gpsexception.h"

#include "bdbuffer.h"
#include "structs.h"
#include "key.h"
#include "partdec.h"

namespace GPSAFE {

    class File {
	private:
	    int fd;
	    bool isopen;
	    std::string filename;
	    time_t mtime;

	    void openCreate() throw(GPSException);
	    void openNoCreate() throw(GPSException);

	    time_t lastModified() const throw(GPSException);

	    void seekCurr(off_t offset) const throw(GPSException);
	    void seekAbs(off_t offset) const throw(GPSException);

	    void preparePWSave() throw(GPSException);
	    
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

	    void seekDataSection() const throw(GPSException);

	    BDBuffer* read() const throw(GPSException);

	    void write(const BDBuffer& buff, bool append=false, bool force=false) throw(GPSException, GPSRetryException);

	    bool isempty() const throw(GPSException);

	    void initFile(const Key& key) throw(GPSException);
	    
	    void writeHeader(const Record<FileHeader>& header, const Key& key) throw(GPSException);
	    void writeHeader(const BDBuffer& enc_header) throw(GPSException);
	    BDBuffer* readHeader() const throw(GPSException);

	    void validateKey(const Key& key) throw(GPSException,GPSInvalidPasswordException);

	public:
	    File(const std::string& fn, const Key& key, bool create=false) throw(GPSException);
	    File(const File& f) throw(GPSException);
	    ~File();

	    void save(std::list<PartDec>& records) throw(GPSException);
	    std::list<PartDec> read(const Key& key) const throw(GPSException);
	    std::string getFilename() const { return filename; }
	    
	    const File& operator=(const File& f) throw(GPSException);
    };
}
#endif // _FILE_H
