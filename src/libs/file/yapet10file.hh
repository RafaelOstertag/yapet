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

#ifndef _YAPET10FILE_HH
#define _YAPET10FILE_HH

#include <list>

#include "rawfile.hh"
#include "yapetfile.hh"

namespace yapet {

/**
 * YAPET 1.0 file.
 */
class Yapet10File : public YapetFile {
   private:
    std::string recognitionStringAsString() const;

   public:
    static constexpr std::uint8_t RECOGNITION_STRING[]{'Y', 'A', 'P', 'E',
                                                       'T', '1', '.', '0'};
    static constexpr int RECOGNITION_STRING_SIZE{8};

    Yapet10File(const std::string& filename, bool create = false,
                bool secure = true);

    Yapet10File(const Yapet10File&) = delete;
    Yapet10File& operator=(const Yapet10File&) = delete;

    Yapet10File(Yapet10File&& other);
    Yapet10File& operator=(Yapet10File&& other);

    virtual ~Yapet10File();

    virtual void open();

    virtual bool hasValidFormat();

    virtual SecureArray readIdentifier();

    /**
     * This is a noop on YAPET 1.0 files
     */
    virtual SecureArray readUnencryptedMetaData();

    virtual SecureArray readHeader();

    virtual std::list<SecureArray> readPasswordRecords();

    virtual void writeIdentifier();

    /**
     * This is a noop on YAPET 1.0 files
     */
    virtual void writeUnencryptedMetaData(const SecureArray&);

    virtual void writeHeader(const SecureArray& header);

    virtual void writePasswordRecords(const std::list<SecureArray>& passwords);

    virtual int recognitionStringSize() const;
    virtual const uint8_t* recognitionString() const;
};
}  // namespace yapet

#endif /* _YAPET10FILE_HH */
