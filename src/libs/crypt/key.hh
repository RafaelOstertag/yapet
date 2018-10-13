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

#ifndef _KEY_HH
#define _KEY_HH

#include "metadata.hh"
#include "securearray.hh"

namespace yapet {
/**
 * Interface for encryption/decryption keys
 */
class Key {
   public:
    virtual ~Key() {}

    /**
     * Set parameters for key generator
     */
    virtual void keyingParameters(const MetaData& parameters) = 0;

    virtual const MetaData& keyingParameters() const = 0;

    /**
     * The password from which the key is derived.
     *
     * The password provided in \c password is expected to be terminated by
     * \c \0.
     *
     * If the key computation requires key parameters, they must be set
     * before calling this function.
     *
     * @param password \c SecureArray containing password terminated by \c
     * \0.
     */
    virtual void password(const SecureArray& password) = 0;

    virtual SecureArray key() const = 0;
    virtual SecureArray::size_type keySize() const = 0;

    virtual SecureArray ivec() const = 0;
    virtual SecureArray::size_type ivecSize() const = 0;

    virtual bool operator==(const Key& k) const = 0;
    //! Compares two keys for inequality
    virtual bool operator!=(const Key& k) const = 0;
};
}  // namespace yapet
#endif