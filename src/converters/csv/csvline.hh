#ifndef _CSVLINE_HH
#define _CSVLINE_HH

#include <vector>

#include "csvstringfield.hh"

namespace yapet {
class CSVLine {
   public:
    using field_index_type = unsigned int;

   private:
    static constexpr char ESCAPE_CHAR{'"'};
    static constexpr char DEFAULT_SEPARATOR{','};

    field_index_type _numberOfFields;
    std::vector<CSVStringField> _line;
    char _separator;

    void addStringAsCsvFieldToLine(const std::string& field,
                                   field_index_type atIndex);
    void validIndexOrThrow(field_index_type index) const;

   public:
    CSVLine(field_index_type numberOfFields,
            char separator = CSVLine::DEFAULT_SEPARATOR);
    CSVLine(const CSVLine& other);
    CSVLine(CSVLine&& other);
    CSVLine& operator=(const CSVLine& other);
    CSVLine& operator=(CSVLine&& other);

    /**
     * Add a unescaped field to the line.
     *
     * @param value unescaped field value
     */
    void addField(field_index_type index, const std::string& value);
    /**
     * Get field unescaped from line.
     *
     * @param index position of field
     */
    std::string operator[](field_index_type index);

    /**
     * Parse CSV line.
     *
     * Parse a CSV line and populate fields. This field is inteded to be used
     * with a line read from a CSV file.
     *
     * @param line line to parse
     */
    void parseLine(const std::string& line);

    /**
     * Get line with all fields escaped.
     *
     * This function is intended to be used when writing a line to a CSV file.
     *
     * @return string suitable to be saved to a file.
     */
    std::string getLine();
};
}  // namespace yapet

#endif