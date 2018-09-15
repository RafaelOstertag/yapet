#ifndef _CSVSTRINGFIELD_HH
#define _CSVSTRINGFIELD_HH

#include <string>

namespace yapet {

class CSVStringField {
   private:
    static constexpr char DEFAULT_SEPARATOR{','};

    std::string _escapedString;
    std::string _unescapedString;
    std::string _string;
    char _fieldSeparator;

    bool isEscaped();
    bool requireEscaping();
    std::string performEscapeing();
    std::string performUnescapeing();

   public:
    CSVStringField();
    /**
     * Constructor.
     *
     * @param str CSV field string encoded as per
     * .
     */
    CSVStringField(const std::string& str,
                   char fieldSeparator = CSVStringField::DEFAULT_SEPARATOR);
    CSVStringField(const CSVStringField& other);
    CSVStringField(CSVStringField&& other);
    CSVStringField& operator=(const CSVStringField& other);
    CSVStringField& operator=(CSVStringField&& other);

    /**
     * Unescape string.
     *
     * Unescape string assuming it is escaped as per
     * https://www.ietf.org/rfc/rfc4180.txt
     */
    std::string unescape();

    /**
     * Escape string if necessary.
     *
     * Escape string as per https://www.ietf.org/rfc/rfc4180.txt
     */
    std::string escape();
};

}  // namespace yapet

#endif