#ifndef _PASSWORDLISTITEM_HH
#define _PASSWORDLISTITEM_HH

#include "passwordrecord.hh"

namespace yapet {
class PasswordListItem {
   private:
    SecureArray _name;
    SecureArray _encryptedRecord;

   public:
    using size_type = SecureArray::size_type;
    PasswordListItem(const char* name, const SecureArray& encryptedRecord);

    PasswordListItem(const PasswordListItem& item);
    PasswordListItem& operator=(const PasswordListItem& item);

    PasswordListItem(PasswordListItem&& item);
    PasswordListItem& operator=(PasswordListItem&& item);

    bool operator==(const PasswordListItem& other) const;
    bool operator!=(const PasswordListItem& other) const {
        return !operator==(other);
    }

    const std::uint8_t* name() const { return *_name; }
    SecureArray::size_type nameSize() const { return _name.size(); }
    const SecureArray& encryptedRecord() const { return _encryptedRecord; }
};

bool operator<(const PasswordListItem& a, const PasswordListItem& b);

}  // namespace yapet
#endif