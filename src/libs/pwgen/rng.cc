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

#include "consts.h"
#include "intl.h"
#include "rng.hh"

using namespace yapet::pwgen;

namespace {
constexpr char DEV_RANDOM[]{"/dev/random"};
constexpr char DEV_URANDOM[]{"/dev/urandom"};
constexpr int BAD_FD{-1};
}  // namespace

void RngFile::closeFd() {
    if (fd != BAD_FD) {
        ::close(fd);
    }
}

RngFile::RngFile(const std::string& filename) : _filename{filename}, fd{-1} {
    fd = ::open(filename.c_str(), O_RDONLY);
    if (fd == BAD_FD) {
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("Cannot open '%s'"), filename.c_str());
        throw std::runtime_error(msg);
    }
}

RngFile::~RngFile() { closeFd(); }

RngFile::RngFile(const RngFile& rng) : _filename{rng._filename} {
    closeFd();

    fd = ::dup(rng.fd);
    if (fd == BAD_FD) {
        throw std::runtime_error(_("Error creating object copy"));
    }
}

RngFile::RngFile(RngFile&& rng)
    : _filename{std::move(rng._filename)}, fd{rng.fd} {
    rng.fd = BAD_FD;
}

RngFile& RngFile::operator=(const RngFile& rng) {
    if (this == &rng) return *this;

    closeFd();

    _filename = rng._filename;
    fd = ::dup(rng.fd);
    if (fd == BAD_FD) {
        throw std::runtime_error(_("Error creating object copy"));
    }

    return *this;
}
RngFile& RngFile::operator=(RngFile&& rng) {
    if (this == &rng) return *this;

    _filename = std::move(rng._filename);
    fd = rng.fd;
    rng.fd = BAD_FD;

    return *this;
}

std::uint8_t RngFile::getNextByte() {
    std::uint8_t c;
    int retval = ::read(fd, &c, sizeof(std::uint8_t));
    if (retval != sizeof(std::uint8_t)) {
        throw std::runtime_error(_("Error reading random byte from file"));
    }
    return c;
}

RngRand::RngRand() { std::srand(time(nullptr)); }

std::uint8_t RngRand::getNextByte() {
    return std::uint8_t((double(std::rand()) / RAND_MAX) * 256);
}

std::unique_ptr<RngInterface> yapet::pwgen::getRng(RNGENGINE rngEngine) {
    switch (rngEngine) {
        case DEVRANDOM:
            return std::unique_ptr<RngInterface>{new RngFile{DEV_RANDOM}};
        case DEVURANDOM:
            return std::unique_ptr<RngInterface>{new RngFile{DEV_URANDOM}};
        case RAND:
            return std::unique_ptr<RngInterface>{new RngRand};
    }

    throw std::invalid_argument(_("Invalid RNGENGINE"));
}