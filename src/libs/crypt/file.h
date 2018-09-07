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

#ifndef _FILE_H
#define _FILE_H 1

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <list>
#include <memory>
#include <string>

#include "abstractcryptofactory.hh"
#include "crypto.hh"
#include "header10.hh"
#include "headerversion.hh"
#include "passwordlistitem.hh"
#include "passwordrecord.hh"
#include "yapet10file.hh"
#include "yapetexception.h"
#include "yapetfile.hh"

namespace YAPET {
class File {
   private:
    int64_t _fileModificationTime;
    // Required when setting new password
    std::shared_ptr<yapet::AbstractCryptoFactory> _abstractCryptoFactory;
    std::unique_ptr<yapet::YapetFile> _yapetFile;
    std::unique_ptr<yapet::Crypto> _crypto;

    yapet::Header10 readHeader();

    void initializeEmptyFile();
    void validateExistingFile();
    void notModifiedOrThrow();

   public:
    //! Constructor
    File(std::shared_ptr<yapet::AbstractCryptoFactory> abstractCryptoFactory,
         const std::string& filename, bool create = false, bool secure = true);

    File(File&& f);
    File& operator=(File&& f);

    File(const File&) = delete;
    File& operator=(const File&) = delete;
    ~File();

    //! Saves a password record list.
    void save(const std::list<yapet::PasswordListItem>& records,
              bool forcewrite = false);
    //! Reads the stored password records from the file.
    std::list<yapet::PasswordListItem> read();

    //! Sets a new encryption key for the current file.
    void setNewKey(
        const std::shared_ptr<yapet::AbstractCryptoFactory>& newCryptoFactory,
        bool forcewrite = false);
    std::int64_t getMasterPWSet();
    yapet::SecureArray getFileVersion();
    yapet::HEADER_VERSION getHeaderVersion();

    //! Returns the file name of the current file.
    std::string getFilename() const { return _yapetFile->filename(); }
    //! Returns whether or not file security is enabled
    bool filesecurityEnabled() const { return _yapetFile->isSecure(); }
};
}  // namespace YAPET
#endif  // _FILE_H
