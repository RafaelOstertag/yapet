#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cerrno>
#include <cinttypes>
#include <stdio.h>

#include "intl.h"
#include "fileutils.hh"
#include "fileerror.hh"

using namespace yapet;

constexpr auto SECURE_MODE{S_IRUSR | S_IWUSR | S_IFREG};

namespace {

    inline uid_t getCurrentUid() {
        return ::getuid();
    }

    inline gid_t getCurrentGid() {
        return ::getgid();
    }

    inline void getFileStat(const std::string& filename, struct stat* statPtr) {
        auto error = ::stat(filename.c_str(), statPtr);
        if (error) {
            throw FileError{_("Unable to get stat for file"), errno};
        }
    }
}; // namespace

void yapet::setSecurePermissionsAndOwner(const std::string& filename) {
    auto uid = getCurrentUid();
    auto gid = getCurrentGid();
    auto error = ::chown(filename.c_str(), uid, gid);
    if (error) {
        throw FileError{_("Error setting owner of file"), errno};
    }

    error = ::chmod(filename.c_str(), S_IRUSR | S_IWUSR);
    if (error) {
        throw FileError{_("Error setting file mode"), errno};
    }

}

std::uint64_t yapet::getModificationTime(const std::string& filename) {
    struct stat fileStat;
    getFileStat(filename, &fileStat);
    return fileStat.st_mtime;
}

bool yapet::hasSecurePermissions(const std::string& filename) {
    struct stat fileStat;
    getFileStat(filename, &fileStat);

    auto uid = getCurrentUid();
    auto gid = getCurrentGid();

    if (fileStat.st_uid != uid ||
            fileStat.st_gid != gid) {
        return false;
    }

    if (fileStat.st_mode != SECURE_MODE) {
        return false;
    }

    return true;
}

void yapet::renameFile(const std::string& oldName, const std::string& newName) {
    if (oldName == newName) {
        throw FileError(_("Old and new name cannot be the same"));
    }
    auto error = ::rename(oldName.c_str(), newName.c_str());
    if (error) {
        throw FileError{_("Cannot rename file"), errno};
    }
}