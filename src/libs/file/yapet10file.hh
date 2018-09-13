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
 * @brief Class for storing and retrieving encrypted data to and from disk
 *
 * This class takes care of storing and retrieving encrypted password
 * records to and from disk.
 *
 * Each file created by this class starts with a unencrypted recognition
 * string which currently consists of the 8 bytes "YAPET1.0" as depicted
 * below.
 *
@verbatim
+--------+--------+--------+--------+--------+--------+--------+--------+
|   Y    |   A    |   P    |   E    |   T    |   1    |   .    |   0    |
| 1 byte | 1 byte | 1 byte | 1 byte | 1 byte | 1 byte | 1 byte | 1 byte |
+--------+--------+--------+--------+--------+--------+--------+--------+
@endverbatim
 *
 * After the recognition string a 4 byte unsigned integer which is stored
 * in big-endian order follows. This indicator is read to determine how
 * many bytes to read in order to get the encrypted header.
 *
@verbatim
+--------+--------+--------+--------+
|   Length indicator in big-endian  |
|         order (4 bytes)           |
+--------+--------+--------+--------+--...---+
|  Encrypted header exactly as many bytes    |
|        indicated by the prefix             |
+--------+--------+--------+--------+--...---+
@endverbatim
 *
 * The decrypted header is 25 bytes in size (pre version 0.6). The first
 * byte indicates the version of the file. The next 20 bytes are used as
 * control string. After decryption, the control string is compared to the
 * predefined clear text control string, in order to find out whether or
 * not the key used to decrypt was the same used to encrypt.
 *
@verbatim
+--------+
|Version |
| 1 byte |
+--------+--------+--------+--...---+
|          Control String           |
|             20 bytes              |
+--------+--------+--------+--...---+
|  Time when the Password  |
|    was set (4 bytes)     |
+--------+--------+--------+
@endverbatim
 *
 * As of version 0.6, it File reads headers using a time_t value of 32 or 64
 * bits (see above for 32 bits header). It writes always a header with 64
 * bits as shown below. The header size is 29 bytes.
 *
@verbatim
+--------+
|Version |
| 1 byte |
+--------+--------+--------+--...---+
|          Control String           |
|             20 bytes              |
+--------+--------+--------+--...---+--------+--------+--------+--------+
|             Time when the Password was set (8 bytes)                  |
|                                                                       |
+--------+--------+--------+--------+--------+--------+--------+--------+
@endverbatim
 *
 * Each encrypted password record is prefixed by a 4 byte unsigned integer
 * which is stored in big-endian order. The methods take care returning them
 * in the appropriate order of the host system. That integer is used to
 * indicate the length of the following encrypted data chunk.
 *
@verbatim
+--------+--------+--------+--------+
|   Length indicator in big-endian  |
|         order (4 bytes)           |
+--------+--------+--------+--------+--...---+
|  Encrypted password record of exactly as   |
|   many bytes as indicated by the prefix    |
+--------+--------+--------+--------+--...---+
|   Length indicator in big-endian  |
|         order (4 bytes)           |
+--------+--------+--------+--------+--...---+
|  Encrypted password record of exactly as   |
|   many bytes as indicated by the prefix    |
+--------+--------+--------+--------+--...---+
      [ . . . ]
@endverbatim
 *
 * Putting this together, an encrypted file created by this class looks like
 * this
 *
@verbatim
+--------+--------+--------+--------+--------+--------+--------+--------+
|   Y    |   A    |   P    |   E    |   T    |   1    |   .    |   0    |
| 1 byte | 1 byte | 1 byte | 1 byte | 1 byte | 1 byte | 1 byte | 1 byte |
+--------+--------+--------+--------+--------+--------+--------+--------+
|   Length indicator in big-endian  |
|         order (4 bytes)           |
+--------+--------+--------+--------+--...---+
|  Encrypted header exactly as many bytes    |
|        indicated by the prefix             |
+--------+--------+--------+--------+--...---+
|   Length indicator in big-endian  |
|         order (4 bytes)           |
+--------+--------+--------+--------+--...---+
|  Encrypted password record of exactly as   |
|   many bytes as indicated by the prefix    |
+--------+--------+--------+--------+--...---+
|   Length indicator in big-endian  |
|         order (4 bytes)           |
+--------+--------+--------+--------+--...---+
|  Encrypted password record of exactly as   |
|   many bytes as indicated by the prefix    |
+--------+--------+--------+--------+--...---+
      [ . . . ]
@endverbatim
 *
 * Instances of this class keeps the file open for the lifetime of the
 * instance.
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

    virtual SecureArray readMetaData();

    virtual std::list<SecureArray> readPasswordRecords();

    virtual void writeIdentifier();

    virtual void writeMetaData(const SecureArray& metaData);

    virtual void writePasswordRecords(const std::list<SecureArray>& passwords);

    virtual int recognitionStringSize() const;
    virtual const uint8_t* recognitionString() const;
};
}  // namespace yapet

#endif /* _YAPET10FILE_HH */
