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

#ifndef _METADATA_HH
#define _METADATA_HH

#include <unordered_map>

#include "serializable.hh"

namespace yapet {
class MetaData : public Serializable {
   private:
    std::unordered_map<std::string, int> _dataItems;

   public:
    MetaData();
    MetaData(const SecureArray& serialized);
    MetaData(const MetaData& other);
    MetaData(MetaData&& other);
    MetaData& operator=(const MetaData& other);
    MetaData& operator=(MetaData&& other);

    int getValue(const std::string& key) const;
    void setValue(const std::string& key, int value);

    std::unordered_map<std::string, int>::size_type size() const;

    SecureArray serialize() const;
};
}  // namespace yapet

#endif