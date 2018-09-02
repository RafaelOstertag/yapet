#include <cstring>

#include "passwordlistitem.hh"

using namespace yapet;

PasswordListItem::PasswordListItem(const char* host,
                                   const SecureArray& encryptedRecord)
    : _name{PasswordRecord::NAME_SIZE}, _encryptedRecord{encryptedRecord} {
    auto stringLengthIncludingZero = std::strlen(host) + 1;
    auto len = stringLengthIncludingZero > PasswordRecord::NAME_SIZE
                   ? PasswordRecord::NAME_SIZE
                   : stringLengthIncludingZero;

    std::memcpy(*_name, host, len);
    (*_name)[PasswordRecord::NAME_SIZE - 1] = '\0';
}

PasswordListItem::PasswordListItem(const PasswordListItem& item)
    : _name{item._name}, _encryptedRecord{item._encryptedRecord} {}
PasswordListItem& PasswordListItem::operator=(const PasswordListItem& item) {
    if (&item == this) {
        return *this;
    }

    _name = item._name;
    _encryptedRecord = item._encryptedRecord;

    return *this;
}

PasswordListItem::PasswordListItem(PasswordListItem&& item)
    : _name{std::move(item._name)},
      _encryptedRecord{std::move(item._encryptedRecord)} {}
PasswordListItem& PasswordListItem::operator=(PasswordListItem&& item) {
    if (&item == this) {
        return *this;
    }

    _name = std::move(item._name);
    _encryptedRecord = std::move(item._encryptedRecord);

    return *this;
}