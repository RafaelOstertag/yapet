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

#include "partdec.h"

#include "record.h"
#include "crypt.h"

using namespace YAPET;

PartDec::PartDec() {
    memset(name, 0, NAME_SIZE);
}

PartDec::PartDec(BDBuffer& bd, const Key& key)
    throw(YAPETException) : enc_data(bd) {
   
    Crypt crypt(key);
    Record<PasswordRecord>* dec_pw_rec = crypt.decrypt<PasswordRecord>(bd);
    PasswordRecord* ptr_dec_pw_rec = *dec_pw_rec;

    memcpy(name, ptr_dec_pw_rec->name, NAME_SIZE);
    delete dec_pw_rec;
}

PartDec::PartDec(Record<PasswordRecord>& pr, const Key& key) throw(YAPETException) {
    setRecord(pr, key);
}

PartDec::PartDec(const PartDec& pd) : enc_data(pd.enc_data) {
    memcpy(name, pd.name, NAME_SIZE);
}

PartDec::~PartDec() {
    memset(name, 0, NAME_SIZE);
}

void
PartDec::setRecord(Record<PasswordRecord>& pr, const Key& key) throw(YAPETException) {

    PasswordRecord* ptr_pr = pr;
    memcpy(name, ptr_pr->name, NAME_SIZE);

    Crypt crypt(key);
    BDBuffer* enc_pr = crypt.encrypt(pr);
    enc_data = *enc_pr;
    delete enc_pr;
}

const PartDec&
PartDec::operator=(const PartDec& pd) {
    if (this == &pd) return *this;

    memset(name, 0, NAME_SIZE);
    memcpy(name, pd.name, NAME_SIZE);

    enc_data = pd.enc_data;

    return *this;
}
