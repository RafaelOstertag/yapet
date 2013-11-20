// $Id$
//
// Copyright (C) 2009-2010  Rafael Ostertag
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

#include "consts.h"

using namespace YAPET::CONSTS;

const std::string Consts::default_suffix(".pet");
const std::string Consts::default_rcfilename(".yapet");
const size_t Consts::max_pwlen(256);
const int Consts::min_locktimeout(10);

const std::string&
Consts::getDefaultSuffix() {
    return default_suffix;
}

const std::string&
Consts::getDefaultRCFilename() {
    return default_rcfilename;
}

size_t
Consts::getMaxPWLength() {
    return max_pwlen;
}

int
Consts::getMinLockTimeout() {
    return min_locktimeout;
}
