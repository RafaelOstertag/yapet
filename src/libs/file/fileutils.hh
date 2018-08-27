#ifndef _FILEUTILS_HH
#define _FILEUTILS_HH

#include <cinttypes>
#include <string>

namespace yapet {
void setSecurePermissionsAndOwner(const std::string& filename);
std::uint64_t getModificationTime(const std::string& filename);
bool hasSecurePermissions(const std::string& filename);
void renameFile(const std::string& oldName, const std::string& newName);
}  // namespace yapet

#endif