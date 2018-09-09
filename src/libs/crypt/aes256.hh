// -*- c++ -*-
//
// $Id$
//
// Copyright (C) 2018  Rafael Ostertag
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
// Additional permission under GNU GPL version 3 section 7
//
// If you modify this program, or any covered work, by linking or combining it
// with the OpenSSL project's OpenSSL library (or a modified version of that
// library), containing parts covered by the terms of the OpenSSL or SSLeay
// licenses, Rafael Ostertag grants you additional permission to convey the
// resulting work.  Corresponding Source for a non-source form of such a
// combination shall include the source code for the parts of OpenSSL used as
// well as that of the covered work.
//

#ifndef _AES256_HH
#define _AES256_HH 1

#include <openssl/evp.h>
#include <memory>

#include "crypto.hh"
#include "intl.h"
#include "key.hh"
#include "securearray.hh"
#include "yapetexception.h"

namespace yapet {
/**
 * Encrypt/decrypt data using Blowfish algorithm.
 */
class Aes256 : public Crypto {
   private:
    SecureArray randomIV() const;
    SecureArray extractIVFromRecord(const SecureArray& record) const;
    SecureArray extractCipherTextFromRecord(const SecureArray& record) const;

   protected:
    const EVP_CIPHER* getCipher() const { return EVP_aes_256_cbc(); }

    EVP_CIPHER_CTX* initializeOrThrow(const SecureArray& ivec, MODE mode);

    void checkIVSizeOrThrow(const SecureArray& ivec);
    void validateCipherOrThrow(const SecureArray& ivec);

   public:
    //! Constructor
    Aes256(const std::shared_ptr<Key>& key);
    Aes256(const Aes256&);
    Aes256& operator=(const Aes256& c);

    Aes256(Aes256&& c);
    Aes256& operator=(Aes256&& c);

    ~Aes256() {}

    virtual SecureArray encrypt(const SecureArray& plainText);
    virtual SecureArray decrypt(const SecureArray& cipherText);
};
}  // namespace yapet

#endif  // _AES256_HH
