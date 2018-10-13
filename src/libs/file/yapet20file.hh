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

#ifndef _YAPET20FILE_HH
#define _YAPET20FILE_HH

#include <list>

#include "yapet10file.hh"

namespace yapet {

/**
 * YAPET 1.0 file.
 */
class Yapet20File : public Yapet10File {
   public:
    static constexpr std::uint8_t RECOGNITION_STRING[]{'Y', 'A', 'P', 'E',
                                                       'T', '2', '.', '0'};

    static constexpr int RECOGNITION_STRING_SIZE{8};

    Yapet20File(const std::string& filename, bool create = false,
                bool secure = true);

    Yapet20File(const Yapet20File&) = delete;
    Yapet20File& operator=(const Yapet20File&) = delete;

    Yapet20File(Yapet20File&& other);
    Yapet20File& operator=(Yapet20File&& other);

    virtual ~Yapet20File();

    virtual SecureArray readUnencryptedMetaData();
    virtual void writeUnencryptedMetaData(const SecureArray& metaData);

    virtual int recognitionStringSize() const;
    virtual const uint8_t* recognitionString() const;
};
}  // namespace yapet

#endif /* _YAPET20FILE_HH */
