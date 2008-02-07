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

#ifndef _PARTDEC_H
#define _PARTDEC_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_INTTYPES_H
# include <inttypes.h>
#endif

#include "record.h"
#include "key.h"
#include "yapetexception.h"
#include "bdbuffer.h"
#include "structs.h"

namespace YAPET {
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
		    const Key& key) throw(YAPETException);

	    PartDec(Record<PasswordRecord>& pr,
		    const Key& key) throw(YAPETException);

	    PartDec(const PartDec& pd);

	    ~PartDec();

	    void setRecord(Record<PasswordRecord>& pr,
			   const Key& key) throw(YAPETException);
	    const BDBuffer& getEncRecord() const { return enc_data; }
	    const uint8_t* getName() const { return name; }
	    const char* c_str() const { return (char*)name; }
	    const PartDec& operator=(const PartDec& pd);
    };
}

#endif // _PARTDEC_H
