#include <cstdio>
#include <stdexcept>

#include "consts.h"
#include "csvstringfield.hh"

using namespace yapet;

constexpr char DOUBLE_QUOTE{'"'};

bool CSVStringField::isEscaped() {
    return _string[0] == DOUBLE_QUOTE &&
           _string[_string.length() - 1] == DOUBLE_QUOTE;
}

bool CSVStringField::requireEscaping() {
    return _string.find(_fieldSeparator) != std::string::npos;
}

std::string CSVStringField::performEscapeing() {
    if (!requireEscaping()) {
        return _string;
    }

    std::string buffer;

    buffer += DOUBLE_QUOTE;

    for (std::string::size_type i = 0; i < _string.size(); i++) {
        char currentCharacter = _string[i];

        // Escape a double quote by adding another double quote in front
        if (currentCharacter == DOUBLE_QUOTE) {
            buffer += DOUBLE_QUOTE;
        }

        buffer += currentCharacter;
    }

    buffer += DOUBLE_QUOTE;
    return buffer;
}

std::string CSVStringField::performUnescapeing() {
    if (!isEscaped()) {
        return _string;
    }

    std::string buffer;

    bool nextCharacterDoubleQuote{false};
    for (std::string::size_type i = 1; i < _string.size() - 1; i++) {
        char currentCharacter{_string[i]};
        if (nextCharacterDoubleQuote) {
            if (currentCharacter != DOUBLE_QUOTE) {
                char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
                std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                              _("Expected '\"' but got %c"), currentCharacter);
                throw std::invalid_argument(msg);
            }

            buffer += DOUBLE_QUOTE;
            nextCharacterDoubleQuote = false;
            continue;
        }

        if (currentCharacter == DOUBLE_QUOTE) {
            nextCharacterDoubleQuote = true;
            continue;
        }

        buffer += currentCharacter;
    }

    return buffer;
}

CSVStringField::CSVStringField()
    : _escapedString{},
      _unescapedString{},
      _string{},
      _fieldSeparator{CSVStringField::DEFAULT_SEPARATOR} {}

CSVStringField::CSVStringField(const std::string& str, char fieldSeparator)
    : _escapedString{},
      _unescapedString{},
      _string{str},
      _fieldSeparator{fieldSeparator} {}

CSVStringField::CSVStringField(const CSVStringField& other)
    : _escapedString{other._escapedString},
      _unescapedString{other._unescapedString},
      _string{other._string},
      _fieldSeparator{other._fieldSeparator} {}

CSVStringField::CSVStringField(CSVStringField&& other)
    : _escapedString{std::move(other._escapedString)},
      _unescapedString{std::move(other._unescapedString)},
      _string{std::move(other._string)},
      _fieldSeparator{std::move(other._fieldSeparator)} {}

CSVStringField& CSVStringField::operator=(const CSVStringField& other) {
    if (this == &other) return *this;

    _escapedString = other._escapedString;
    _unescapedString = other._unescapedString;
    _string = other._string;
    _fieldSeparator = other._fieldSeparator;

    return *this;
}

CSVStringField& CSVStringField::operator=(CSVStringField&& other) {
    if (this == &other) return *this;

    _escapedString = std::move(other._escapedString);
    _unescapedString = std::move(other._unescapedString);
    _string = std::move(other._string);
    _fieldSeparator = std::move(other._fieldSeparator);

    return *this;
}

std::string CSVStringField::unescape() {
    if (!_unescapedString.empty()) {
        return _unescapedString;
    }

    if (_string.empty()) {
        return _string;
    }

    _unescapedString = performUnescapeing();

    return _unescapedString;
}

std::string CSVStringField::escape() {
    if (!_escapedString.empty()) {
        return _escapedString;
    }

    if (_string.empty()) {
        return _string;
    }

    if (!isEscaped()) {
        _escapedString = performEscapeing();
    } else {
        _escapedString = _string;
    }

    return _escapedString;
}