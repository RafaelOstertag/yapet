// -*- c++ -*-
//
// $Id$

#ifndef _PARTDEC_H
#define _PARTDEC_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#include "record.h"
#include "key.h"
#include "gpsexception.h"
#include "bdbuffer.h"
#include "structs.h"

namespace GPSAFE {
    /** Holds a partially decrypted record
     * 
     * Partially decrypted records have their name stored plaintext.
     */
    class PartDec {
	private:
	    uint8_t name[NAME_SIZE];
	    BDBuffer enc_data;

	public:
	    PartDec();

	    PartDec(BDBuffer& bd,
		    const Key& key) throw(GPSException);

	    PartDec(Record<PasswordRecord>& pr,
		    const Key& key) throw(GPSException);

	    PartDec(const PartDec& pd);

	    ~PartDec();

	    void setRecord(Record<PasswordRecord>& pr,
			   const Key& key) throw(GPSException);
	    const BDBuffer& getEncRecord() const { return enc_data; }
	    const uint8_t* getName() const { return name; }
	    const char* c_str() const { return (char*)name; }
	    const PartDec& operator=(const PartDec& pd);
    };
}

#endif // _PARTDEC_H
