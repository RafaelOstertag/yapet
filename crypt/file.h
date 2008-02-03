// -*- c++ -*-
//
// $Id$

#ifndef _FILE_H
#define _FILE_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include <string>
#include <list>

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
