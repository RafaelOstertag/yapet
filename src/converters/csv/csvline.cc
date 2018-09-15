#include <cstdio>
#include <stdexcept>

#include "consts.h"
#include "csvline.hh"

using namespace yapet;

void CSVLine::validIndexOrThrow(field_index_type index) const {
    if (index > _numberOfFields) {
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("Cannot access field %d. Maximum field index %d"),
                      index, _numberOfFields - 1);
        throw std::out_of_range(msg);
    }
}

CSVLine::CSVLine(field_index_type numberOfFields, char separator)
    : _numberOfFields{numberOfFields},
      _line{_numberOfFields},
      _separator{separator} {}

CSVLine::CSVLine(const CSVLine& other)
    : _numberOfFields{other._numberOfFields},
      _line{other._line},
      _separator{other._separator} {}
CSVLine::CSVLine(CSVLine&& other)
    : _numberOfFields{std::move(other._numberOfFields)},
      _line{std::move(other._line)},
      _separator{other._separator} {}

CSVLine& CSVLine::operator=(const CSVLine& other) {
    if (this == &other) {
        return *this;
    }

    _numberOfFields = other._numberOfFields;
    _line = other._line;
    _separator = other._separator;

    return *this;
}

CSVLine& CSVLine::operator=(CSVLine&& other) {
    if (this == &other) {
        return *this;
    }

    _numberOfFields = std::move(other._numberOfFields);
    _line = std::move(other._line);
    _separator = other._separator;

    return *this;
}

void CSVLine::addField(field_index_type index, const std::string& value) {
    validIndexOrThrow(index);

    CSVStringField field{value, _separator};
    _line[index] = field;
}

std::string CSVLine::operator[](field_index_type index) {
    validIndexOrThrow(index);

    return _line[index].unescape();
}

void CSVLine::addStringAsCsvFieldToLine(const std::string& field,
                                        field_index_type atIndex) {
    if (atIndex == _numberOfFields) {
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("Expected %d fields in line, read %d"), _numberOfFields,
                      atIndex);
        throw std::invalid_argument(msg);
    }

    CSVStringField csvField{field, _separator};
    _line[atIndex] = csvField;
}

void CSVLine::parseLine(const std::string& line) {
    bool inEscapedField{false};
    field_index_type currentIndex{0};

    std::string fieldBuffer;
    for (std::string::size_type i = 0; i < line.size(); i++) {
        char currentChar{line[i]};
        // Skip newline
        if (currentChar == '\n') {
            continue;
        }

        // Flip the 'in escape field' state
        if (currentChar == CSVLine::ESCAPE_CHAR) {
            inEscapedField = !inEscapedField;
        }

        if (currentChar == _separator && !inEscapedField) {
            addStringAsCsvFieldToLine(fieldBuffer, currentIndex);
            currentIndex++;
            fieldBuffer = std::string{};
            continue;
        }

        fieldBuffer += currentChar;
    }

    if (currentIndex < (_numberOfFields - 1)) {
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("Expected %d fields in line, read only %d"),
                      _numberOfFields, currentIndex);
        throw std::invalid_argument(msg);
    }

    // Process the remaining string in stringstream
    addStringAsCsvFieldToLine(fieldBuffer, currentIndex);
}

std::string CSVLine::getLine() {
    std::string lineAsString;
    for (field_index_type i = 0; i < _numberOfFields; i++) {
        lineAsString += _line[i].escape();
        if (i < (_numberOfFields - 1)) {
            lineAsString += _separator;
        }
    }

    return lineAsString;
}
