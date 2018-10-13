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

#ifndef _BLOWFISHFACTORY_HH
#define _BLOWFISHFACTORY_HH

#include "abstractcryptofactory.hh"
#include "blowfish.hh"
#include "key448.hh"
#include "yapet10file.hh"

namespace yapet {
class BlowfishFactory : public AbstractCryptoFactory {
   private:
    std::shared_ptr<Key> _key448;

   public:
    BlowfishFactory(const SecureArray& password,
                    const MetaData& keyingParameters);
    BlowfishFactory(const BlowfishFactory&) = delete;
    BlowfishFactory(BlowfishFactory&&) = delete;
    BlowfishFactory& operator=(const BlowfishFactory&) = delete;
    BlowfishFactory& operator=(BlowfishFactory&&) = delete;
    ~BlowfishFactory(){};

    virtual std::shared_ptr<AbstractCryptoFactory> newFactory(
        const SecureArray& password, const MetaData& keyingParameters) const;

    virtual std::unique_ptr<Crypto> crypto() const;

    virtual std::shared_ptr<Key> key() const { return _key448; }

    virtual std::unique_ptr<YapetFile> file(const std::string& filename,
                                            bool create, bool secure) const;
};
}  // namespace yapet

#endif