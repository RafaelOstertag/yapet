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

#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cerrno>
#include <cinttypes>
#include <cstdio>

#include "consts.h"
#include "fileerror.hh"
#include "fileutils.hh"
#include "intl.h"

using namespace yapet;

constexpr auto SECURE_MODE{S_IRUSR | S_IWUSR | S_IFREG};

namespace {

inline uid_t getCurrentUid() { return ::getuid(); }

inline gid_t getCurrentGid() { return ::getgid(); }

inline void getFileStat(const std::string& filename, struct stat* statPtr) {
    auto error = ::stat(filename.c_str(), statPtr);
    if (error) {
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("Unable to get stat for file '%s'"), filename.c_str());
        throw FileError{msg, errno};
    }
}
}  // namespace

void yapet::setSecurePermissionsAndOwner(const std::string& filename) {
    auto uid = getCurrentUid();
    auto gid = getCurrentGid();
    auto error = ::chown(filename.c_str(), uid, gid);
    if (error) {
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("Error setting owner of file '%s'"), filename.c_str());
        throw FileError{msg, errno};
    }

    error = ::chmod(filename.c_str(), S_IRUSR | S_IWUSR);
    if (error) {
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("Error setting file mode of file '%s'"),
                      filename.c_str());
        throw FileError{msg, errno};
    }
}

std::int64_t yapet::getModificationTime(const std::string& filename) {
    struct stat fileStat;
    getFileStat(filename, &fileStat);
    return fileStat.st_mtime;
}

std::uint32_t yapet::getFileSize(const std::string& filename) {
    struct stat fileStat;
    getFileStat(filename, &fileStat);

    return fileStat.st_size;
}

bool yapet::hasSecurePermissions(const std::string& filename) {
    struct stat fileStat;
    getFileStat(filename, &fileStat);

    auto uid = getCurrentUid();
    auto gid = getCurrentGid();

    if (fileStat.st_uid != uid || fileStat.st_gid != gid) {
        return false;
    }

    if (fileStat.st_mode != SECURE_MODE) {
        return false;
    }

    return true;
}

void yapet::renameFile(const std::string& oldName, const std::string& newName) {
    if (oldName == newName) {
        throw FileError{_("Old and new name cannot be the same")};
    }
    auto error = ::rename(oldName.c_str(), newName.c_str());
    if (error) {
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("Cannot rename file '%s'"), oldName.c_str());
        throw FileError{msg, errno};
    }
}