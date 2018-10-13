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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif

#include <cstdlib>
#include <functional>
#include <limits>
#include <stdexcept>

#include "consts.h"
#include "intl.h"
#include "rng.hh"

using namespace yapet::pwgen;

namespace {
constexpr char DEV_URANDOM[]{"/dev/urandom"};
constexpr int BAD_FD{-1};

int openDevUrandom() {
    auto fd{::open(DEV_URANDOM, O_RDONLY)};
    if (fd == BAD_FD) {
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("Cannot open '%s'"), DEV_URANDOM);
        throw std::runtime_error(msg);
    }
    return fd;
}

std::uint8_t readRandomInt(int fd) {
    std::uint8_t i;
    std::uint8_t retval = ::read(fd, &i, sizeof(i));
    if (retval != sizeof(i)) {
        throw std::runtime_error(_("Error reading random bytes from file"));
    }
    return i;
}

void closeFd(int fd) {
    if (fd != BAD_FD) {
        ::close(fd);
    }
}
}  // namespace

Rng::Rng(std::uint8_t hi) : fd{openDevUrandom()}, max{hi} {}

Rng::~Rng() { closeFd(fd); }

Rng::Rng(const Rng& rng) : max{rng.max} { fd = ::dup(rng.fd); }

Rng::Rng(Rng&& rng) : fd{rng.fd}, max{rng.max} { rng.fd = BAD_FD; }

Rng& Rng::operator=(const Rng& rng) {
    if (this == &rng) return *this;

    closeFd(fd);

    fd = ::dup(fd);
    max = rng.max;

    return *this;
}

Rng& Rng::operator=(Rng&& rng) {
    if (this == &rng) return *this;

    closeFd(fd);

    fd = rng.fd;
    rng.fd = BAD_FD;

    max = rng.max;

    return *this;
}

std::uint8_t Rng::getNextInt() {
    std::uint8_t randomNumber = readRandomInt(fd);
    while (randomNumber > max) {
        randomNumber = readRandomInt(fd);
    }
    return randomNumber;
}