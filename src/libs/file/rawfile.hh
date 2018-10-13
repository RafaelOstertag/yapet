/*
 * Copyright (C) 2018 Rafael Ostertag
 *
 * This file is part of YAPET.
 *
 * YAPET is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * YAPET is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * YAPET.  If not, see <http://www.gnu.org/licenses/>.
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

#ifndef _RAWFILE_HH
#define _RAWFILE_HH

#include <cstdint>
#include <cstdio>
#include <fstream>
#include <string>
#include <utility>

#include "securearray.hh"

namespace yapet {

/**
 * Provide basic file I/O using \c SecureArray
 */
class RawFile {
   private:
    std::string _filename;
    FILE* _file;
    bool _openFlag;

   public:
    using seek_type = long;

    RawFile(const std::string& filename) noexcept;
    ~RawFile();

    RawFile(const RawFile&) = delete;
    RawFile& operator=(const RawFile&) = delete;

    RawFile(RawFile&&);
    RawFile& operator=(RawFile&&);

    void openExisting();
    void openNew();

    bool isOpen() const { return _openFlag; }

    /**
     * Read \c size bytes.
     *
     * The bytes read are returned in a \c SecureArray.
     */
    std::pair<SecureArray, bool> read(std::uint32_t size);
    /**
     * Read the next record.
     *
     * Read the length indicator and return a \c SecureArray holding the data.
     */
    std::pair<SecureArray, bool> read();

    /**
     * Write \c SecureArray to file.
     *
     * Write the content of the \c SecureArray preceded by the size of the
     * record.
     */
    void write(const SecureArray& secureArray);
    /**
     * Write to file. The buffer is written without any preceding size
     * information.
     */
    void write(const std::uint8_t* buffer, std::uint32_t size);

    std::string filename() const { return _filename; }

    void rewind();

    /**
     * Go to position in file measured in bytes. \c position must be positive
     */
    void seekAbsolute(seek_type position);

    /**
     * Close the file
     */
    void close();

    /**
     * Close and open the file
     */
    void reopen();

    /**
     * Get current stream position measured in bytes
     */
    seek_type getPosition();

    /**
     * Write remaining buffered data to disk.
     */
    void flush();
};
}  // namespace yapet

#endif