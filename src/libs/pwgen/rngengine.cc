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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <assert.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#ifdef HAVE_FCNTL_H
#include <fcntl.h>
#endif

#include <stdexcept>

#include "consts.h"
#include "intl.h"
#include "rngengine.hh"

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

void closeFd(int fd) {
    if (fd != BAD_FD) {
        ::close(fd);
    }
}
}  // namespace

void RngEngine::fillCache() {
    int retval = ::read(fd, &byteCache, BYTE_CACHE_SIZE);
    if (retval != BYTE_CACHE_SIZE) {
        throw std::runtime_error(_("Error reading random bytes from file"));
    }
    positionInCache = 0;
}

RngEngine::RngEngine() : fd{openDevUrandom()}, positionInCache{EMPTY_CACHE} {}

RngEngine::~RngEngine() { closeFd(fd); }

RngEngine::RngEngine(const RngEngine& other)
    : byteCache{other.byteCache}, positionInCache{other.positionInCache} {
    fd = ::dup(other.fd);
}

RngEngine::RngEngine(RngEngine&& other)
    : fd{other.fd},
      byteCache{other.byteCache},
      positionInCache{other.positionInCache} {
    other.fd = BAD_FD;
    other.positionInCache = EMPTY_CACHE;
}

RngEngine& RngEngine::operator=(const RngEngine& other) {
    if (this == &other) return *this;

    closeFd(fd);

    fd = ::dup(fd);
    byteCache = other.byteCache;
    positionInCache = other.positionInCache;

    return *this;
}

RngEngine& RngEngine::operator=(RngEngine&& other) {
    if (this == &other) return *this;

    closeFd(fd);

    fd = other.fd;
    other.fd = BAD_FD;

    byteCache = std::move(other.byteCache);
    positionInCache = other.positionInCache;
    other.positionInCache = EMPTY_CACHE;

    return *this;
}

std::uint8_t RngEngine::operator()() {
    if (positionInCache == EMPTY_CACHE || positionInCache >= BYTE_CACHE_SIZE) {
        fillCache();
    }
    assert(positionInCache >= 0 && positionInCache < BYTE_CACHE_SIZE);

    return byteCache[positionInCache++];
}
