// -*- c++ -*-
//
// $Id$
//
// Copyright (C) 2008-2010  Rafael Ostertag
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

#ifndef _CRYPT_H
#define _CRYPT_H 1

#include <openssl/evp.h>
#include <memory>

#include "bdbuffer.h"
#include "crypto.hh"
#include "intl.h"
#include "key.hh"
#include "record.h"
#include "securearray.hh"
#include "yapetexception.h"

namespace YAPET {
/**
 * Encrypt/decrypt data using Blowfish algorithm.
 */
class Blowfish : public yapet::Crypto {
   protected:
    const EVP_CIPHER* getCipher() const { return EVP_bf_cbc(); }

   public:
    //! Constructor
    Blowfish(const std::shared_ptr<yapet::Key>& key);
    Blowfish(const Blowfish&);
    Blowfish& operator=(const Blowfish& c);

    Blowfish(Blowfish&& c);
    Blowfish& operator=(Blowfish&& c);

    ~Blowfish() {}
};
}  // namespace YAPET

#endif  // _CRYPT_H
