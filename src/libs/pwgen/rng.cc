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

#include "rng.hh"

using namespace yapet::pwgen;

Rng::Rng(std::uint8_t hi) : _rngEngine{}, _distribution{0, hi} {}

Rng::Rng(const Rng& rng) : _rngEngine{}, _distribution{rng._distribution} {}

Rng::Rng(Rng&& rng)
    : _rngEngine{std::move(rng._rngEngine)},
      _distribution{std::move(rng._distribution)} {}

Rng& Rng::operator=(const Rng& rng) {
    if (this == &rng) return *this;

    _rngEngine = rng._rngEngine;
    _distribution = rng._distribution;

    return *this;
}

Rng& Rng::operator=(Rng&& rng) {
    if (this == &rng) return *this;

    _rngEngine = std::move(rng._rngEngine);
    _distribution = std::move(rng._distribution);

    return *this;
}

std::uint8_t Rng::getNextInt() { return _distribution(_rngEngine); }