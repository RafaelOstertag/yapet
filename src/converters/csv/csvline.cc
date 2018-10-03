#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <cstdio>
#include <stdexcept>

#include "consts.h"
#include "csvline.hh"
#include "intl.h"

using namespace yapet;

constexpr std::string::size_type START_OF_LINE{0};

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
                      atIndex + 1);
        throw std::invalid_argument(msg);
    }

    CSVStringField csvField{field, _separator};
    _line[atIndex] = csvField;
}

void CSVLine::parseLine(const std::string& line) {
    bool inEscapedField{false};
    field_index_type currentIndex{0};

    std::string fieldBuffer;
    for (std::string::size_type column = 0; column < line.size(); column++) {
        char currentChar{line[column]};
        char lookAhead{column + 1 < line.size() ? line[column + 1]
                                                : _separator};

        if (currentChar == CSVLine::ESCAPE_CHAR && column == START_OF_LINE) {
            fieldBuffer = CSVLine::ESCAPE_CHAR;
            inEscapedField = true;
            continue;
        }

        if (currentChar == CSVLine::ESCAPE_CHAR &&
            lookAhead == CSVLine::ESCAPE_CHAR && inEscapedField) {
            fieldBuffer += CSVLine::ESCAPE_CHAR;
            fieldBuffer += CSVLine::ESCAPE_CHAR;
            column++;
            continue;
        }

        if (currentChar == CSVLine::ESCAPE_CHAR && lookAhead == _separator) {
            fieldBuffer += CSVLine::ESCAPE_CHAR;
            inEscapedField = false;
            continue;
        }

        if (currentChar == _separator && !inEscapedField) {
            addStringAsCsvFieldToLine(fieldBuffer, currentIndex);
            currentIndex++;
            fieldBuffer = std::string{};

            if (lookAhead == CSVLine::ESCAPE_CHAR) {
                fieldBuffer = CSVLine::ESCAPE_CHAR;
                inEscapedField = true;
                column++;
            }

            continue;
        }

        fieldBuffer += currentChar;
    }

    if (inEscapedField) {
        throw std::invalid_argument(_("'\"' mismatch"));
    }

    if (currentIndex < (_numberOfFields - 1)) {
        char msg[YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE];
        std::snprintf(msg, YAPET::Consts::EXCEPTION_MESSAGE_BUFFER_SIZE,
                      _("Expected %d fields in line, read only %d"),
                      _numberOfFields, currentIndex + 1);
        throw std::invalid_argument(msg);
    }

    // Process the remaining string in stringstream
    addStringAsCsvFieldToLine(fieldBuffer, currentIndex);
}

std::string CSVLine::getLine() {
    std::string lineAsString;
    for (field_index_type column = 0; column < _numberOfFields; column++) {
        lineAsString += _line[column].escape();
        if (column < (_numberOfFields - 1)) {
            lineAsString += _separator;
        }
    }

    return lineAsString;
}
