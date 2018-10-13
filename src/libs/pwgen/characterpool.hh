#ifndef _CHARACTERPOOL_HH
#define _CHARACTERPOOL_HH

#include <string>
#include <vector>

namespace yapet {
namespace pwgen {

/**
 * @brief Available character pools
 *
 * Pools to choose from.
 */
enum POOLS {
    /**
     * Letters: a-z, A-Z
     */
    LETTERS = (1 << 0),
    /**
     * Digits: 0-9
     */
    DIGITS = (1 << 1),
    /**
     * Punctuation characters: comma, semicolon, etc.
     */
    PUNCT = (1 << 2),
    /**
     * Special characters: #,(,), etc.
     */
    SPECIAL = (1 << 3),
    /**
     * Other characters
     */
    OTHER = (1 << 4),
    ALL = LETTERS | DIGITS | PUNCT | SPECIAL | OTHER
};

constexpr bool isLetters(int v) { return (v & LETTERS); }
constexpr bool isDigits(int v) { return (v & DIGITS); }
constexpr bool isPunct(int v) { return (v & PUNCT); }
constexpr bool isSpecial(int v) { return (v & SPECIAL); }
constexpr bool isOther(int v) { return (v & OTHER); }
constexpr bool isAll(int v) { return (v & ALL); }

extern const std::string letters;
extern const std::string digits;
extern const std::string punctuation;
extern const std::string special;
extern const std::string other;

std::string getPools(int pools);

}  // namespace pwgen
}  // namespace yapet

#endif