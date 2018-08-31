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

#include <cassert>
#include <list>
#include <memory>
#include <string>

#include "yapetexception.h"

#include "bdbuffer.h"
#include "key.h"
#include "partdec.h"
#include "structs.h"
#include "yapet10file.hh"
#include "yapetfile.hh"

namespace YAPET {
class File {
   private:
    uint64_t _fileModificationTime;
    std::unique_ptr<yapet::YapetFile> _yapetFile;

    void initializeEmptyFile(const Key& key);

   public:
    //! Constructor
    File(const std::string& filename, const Key& key, bool create = false,
         bool secure = true);

    File(File&& f);
    File& operator=(File&& f);

    File(const File&) = delete;
    File& operator=(const File&) = delete;
    ~File();

    //! Saves a password record list.
    void save(const std::list<PartDec>& records, bool forcewrite = false);
    //! Reads the stored password records from the file.
    std::list<PartDec> read(const Key& key) const;
    //! Returns the file name of the current file.
    std::string getFilename() const { return _yapetFile->filename(); }
    //! Sets a new encryption key for the current file.
    void setNewKey(const Key& oldkey, const Key& newkey,
                   bool forcewrite = false);
    int64_t getMasterPWSet(const Key& key) const;
    //! Return the file version
    FILE_VERSION getFileVersion(const Key& key) const;

    //! Returns whether or not file security is enabled
    bool filesecurityEnabled() const { return _yapetFile->isSecure(); }
};
}  // namespace YAPET
#endif  // _FILE_H
