// $Id$

#include "partdec.h"

#include "record.h"
#include "crypt.h"

using namespace GPSAFE;

PartDec::PartDec() {
    memset(name, 0, NAME_SIZE);
}

PartDec::PartDec(BDBuffer& bd, const Key& key)
    throw(GPSException) : enc_data(bd) {
   
    Crypt crypt(key);
    Record<PasswordRecord>* dec_pw_rec = crypt.decrypt<PasswordRecord>(bd);
    PasswordRecord* ptr_dec_pw_rec = *dec_pw_rec;

    memcpy(name, ptr_dec_pw_rec->name, NAME_SIZE);
    delete dec_pw_rec;
}

PartDec::PartDec(Record<PasswordRecord>& pr, const Key& key) throw(GPSException) {
    setRecord(pr, key);
}

PartDec::PartDec(const PartDec& pd) : enc_data(pd.enc_data) {
    memcpy(name, pd.name, NAME_SIZE);
}

PartDec::~PartDec() {
    memset(name, 0, NAME_SIZE);
}

void
PartDec::setRecord(Record<PasswordRecord>& pr, const Key& key) throw(GPSException) {

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
