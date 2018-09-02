#ifndef _PASSWORDLISTITEM_HH
#define _PASSWORDLISTITEM_HH

#include "passwordrecord.hh"

namespace yapet {
class PasswordListItem {
   private:
    SecureArray _name;
    SecureArray _encryptedRecord;

   public:
    PasswordListItem(const char* name, const SecureArray& encryptedRecord);

    PasswordListItem(const PasswordListItem& item);
    PasswordListItem& operator=(const PasswordListItem& item);

    PasswordListItem(PasswordListItem&& item);
    PasswordListItem& operator=(PasswordListItem&& item);

    const std::uint8_t* name() const { return *_name; }
    const SecureArray& encryptedRecord() const { return _encryptedRecord; }
};
}  // namespace yapet
#endif