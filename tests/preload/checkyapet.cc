// $Id$
//
// Create an empty YAPET file, press Enter (nothing must happen),
// press 'd' (nothing must happen). Quit YAPET.
//
//
// call yapet like this
//
//  LD_PRELOAD=libpwrecord.so yapet -i /tmp/testfile
#include <libyacurscfg.h>

#if defined(HAVE_CURSES_ENHANCED) && defined(HAVE_LOCALE_H) && \
    defined(HAVE_CWCHAR) && !defined(DISABLE_WCHAR)
#define YACURS_USE_WCHAR 1
#endif

#include <unistd.h>
#include <cstdlib>

#ifdef YACURS_USE_WCHAR
#include <clocale>
#include <cwchar>
#endif

#ifdef __cplusplus
extern "C" {
#endif

#ifdef YACURS_USE_WCHAR
std::wint_t
#else
int
#endif
    __test_data[50][100] = {
        {'\n', 0},
        // Password for new file
        {'p', 'l', 'e', 'a', 's', 'e', 'c', 'h', 'a', 'n', 'g', 'e', '\n', 0},
        // Enter password again
        {'p', 'l', 'e', 'a', 's', 'e', 'c', 'h', 'a', 'n', 'g', 'e', '\n', 0},
        {'\n', 0},  // Press ok button
        {'\n', 0},  // must not do anything
        {'d', 0},   // must not do anything
        {'q', 0},   // quit
        {0}};

#ifdef YACURS_USE_WCHAR
int wget_wch(void* wdc, std::wint_t* i) {
    static int row = 0;
    static int col = 0;

    if (__test_data[row][col] == 0) {
        col = 0;
        row++;

        if (__test_data[row][col] == 0) std::abort();
    }

    *i = __test_data[row][col++];

    return 0;
}

#else
int wgetch(void* wdc) {
    static int row = 0;
    static int col = 0;

    if (__test_data[row][col] == 0) {
        col = 0;
        row++;

        if (__test_data[row] == 0) std::abort();
    }

    return __test_data[row][col++];
}

#endif

#ifdef __cplusplus
}
#endif
