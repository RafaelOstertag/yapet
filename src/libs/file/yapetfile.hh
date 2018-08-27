/*
 * Copyright (C) 2018 rafi
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Additional permission under GNU GPL version 3 section 7
 *
 * If you modify this program, or any covered work, by linking or combining it
 * with the OpenSSL project's OpenSSL library (or a modified version of that
 * library), containing parts covered by the terms of the OpenSSL or SSLeay
 * licenses, Rafael Ostertag grants you additional permission to convey the
 * resulting work.  Corresponding Source for a non-source form of such a
 * combination shall include the source code for the parts of OpenSSL used as
 * well as that of the covered work.
 */

/*
 * File:   YapetFile.hh
 * Author: rafi
 *
 * Created on August 19, 2018, 6:27 PM
 */

#ifndef _YAPETFILE_HH
#define _YAPETFILE_HH

#include <list>

#include "rawfile.hh"

namespace yapet {

class YapetFile {
   private:
    RawFile _rawFile;
    bool _create;
    bool _secure;

   protected:
    inline RawFile& getRawFile() { return _rawFile; }

    void openRawFile();

   public:
    /**
     *
     * @param filename file to open
     * @param create if \c true create file if it does not exist. If \c false
     * open existing file or fail
     * @param secure if \c true, fail if permissions and owner are not secure.
     */
    YapetFile(const std::string& filename, bool create = false,
              bool secure = true);

    YapetFile(const YapetFile&) = delete;
    YapetFile& operator=(const YapetFile&) = delete;

    YapetFile(YapetFile&& other);
    YapetFile& operator=(YapetFile&& other);

    virtual ~YapetFile();

    /**
     * Indicate whether the file has valid format.
     *
     * @return \c true if the file has valid format, \c false otherwise.
     */
    virtual bool hasValidFormat() = 0;

    /**
     * Get plain text identifier.
     *
     * @return plain text identifier
     */
    virtual SecureArray readIdentifier() = 0;

    /**
     * Read the meta data.
     * @return meta data
     */
    virtual SecureArray readMetaData() = 0;

    virtual std::list<SecureArray> readPasswordRecords() = 0;

    /**
     * Write the identifier
     */
    virtual void writeIdentifier() = 0;

    /**
     * Write meta data
     */
    virtual void writeMetaData(const SecureArray& metaData) = 0;

    /**
     * Write password records
     */
    virtual void writePasswordRecords(
        const std::list<SecureArray>& passwords) = 0;

    bool isSecure() const { return _secure; }

    bool isCreate() const { return _create; }

    std::string filename() const { return _rawFile.filename(); }
};
}  // namespace yapet

#endif /* _YAPETFILE_HH */
