// -*- c++ -*-
//
// $Id$
//
// Copyright (C) 2008-2010  Rafael Ostertag
//
// This file is part of YAPET.
//
// YAPET is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// YAPET is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// YAPET.  If not, see <http://www.gnu.org/licenses/>.
//

/**
 * @file
 * @brief Holds workarounds for curses.h
 *
 * Holds workarounds for curses.h when compiling under C++. It specifically
 * removes the macros:
 *
 * - \c box
 * - \c clear
 * - \c erase
 * - \c move
 * - \c refresh
 *
 * and replaces them by inline functions.
 *
 * It also defines certain inline functions for dealing with \c const
 * \c char* arguments. Some curses functions simply use \c char* which
 * doesn't go along well with \c std::string::c_str(). The defined
 * functions do have the same syntax as their cousins.
 *
 * The configure script figures out which one to use.
 *
 * The following functions are defined to handle \c const \c char*
 * arguments
 *
 * - \c mywaddstr()
 * - \c mymvwaddstr()
 * - \c mymvwaddnstr()
 *
 * \c mvwchgat is also defined as inline function in case the curses
 * library does not provide it. The configure script takes care of
 * figuring whether or not the function is provided by the curses
 * library.
 *
 * Please note that this file has to be included after the (n)curses
 * header file.
 */

#ifndef _CURSWA_H
#define _CURSWA_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif


enum {
#ifdef KEY_REFRESH
#undef KEY_REFRESH
    /**
     * The value returned by \c [w]getch() when the user presses \c
     * ^L.
     */
    KEY_REFRESH = 12,
#endif // KEY_REFRESH
#ifndef KEY_ESC
    /**
     * The value returned by \c [w]getch() upon pressing the escape key
     */
    KEY_ESC = 27,
#endif // KEY_ESC
#ifndef KEY_TAB
    /**
     * The value returned by \c [w]getch() upon pressing the tab key
     */
    KEY_TAB = '\t',
#endif // KEY_TAB
#ifndef KEY_SPACE
    /**
     * The value returned by \c [w]getch() upon pressing the space key
     */
    KEY_SPACE = ' ',
#endif // KEY_SPACE
    /* Used by InputWidget to indicate that the read-only mode should be
       left */
    KEY_CTRL_E = 5
};

/**
 * @brief Shows or hides the cursor
 *
 * Shows or hides the cursor.
 */
inline void
visibleCursor (bool v) {
    if (v) {
        int err = curs_set (2);

        if (err == ERR) curs_set (1);
    } else {
        curs_set (0);
    }
}


#ifdef HAVE_CURSES_H

#if defined(tab) && defined(_XOPEN_CURSES)
#undef tab
#endif

#ifdef box
#undef box
inline int box (WINDOW* win, int verch, int horch) {
    return wborder (win, verch, verch, horch, horch, 0, 0, 0, 0);
}
#endif

#ifdef clear
#undef clear
inline int clear() {
    return wclear (stdscr);
}
#endif

#ifdef erase
#undef erase
inline int erase() {
    return werase (stdscr);
}
#endif

#ifdef move
#undef move
inline int move (int y, int x) {
    return wmove (stdscr, y, x);
}
#endif

#ifdef refresh
#undef refresh
inline int refresh() {
    return wrefresh (stdscr);
}
#endif

#endif // HAVE_CURSES_H

#ifdef WADDSTR_USE_CHAR
#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
# include <string.h>
#endif

inline int waddstr_c (WINDOW* win, const char* str) {
    char* tmp_ptr = (char*) malloc (strlen (str) + 1);
    memcpy (tmp_ptr, str, strlen (str) + 1);
    int retval = waddstr (win, tmp_ptr);
    free (tmp_ptr);
    return retval;
}
#define mywaddstr(a,b) waddstr_c(a,b)
#else // WADDSTR_USE_CHAR
#define mywaddstr(a,b) waddstr(a,b)
#endif // WADDSTR_USE_CHAR

#ifdef MVWADDSTR_USE_CHAR
#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
# include <string.h>
#endif

inline int mvwaddstr_c (WINDOW* win, int y, int x, const char* str) {
    char* tmp_ptr = (char*) malloc (strlen (str) + 1);
    memcpy (tmp_ptr, str, strlen (str) + 1);
    int retval = mvwaddstr (win, y, x, tmp_ptr);
    free (tmp_ptr);
    return retval;
}
#define mymvwaddstr(a,b,c,d) mvwaddstr_c(a,b,c,d)
#else // MVWADDSTR_USE_CHAR
#define mymvwaddstr(a,b,c,d) mvwaddstr(a,b,c,d)
#endif // MVWADDSTR_USE_CHAR

#ifdef MVWADDNSTR_USE_CHAR
#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif
#ifdef HAVE_STRING_H
# include <string.h>
#endif

inline int mvwaddnstr_c (WINDOW* win, int y, int x, const char* str, int n) {
    char* tmp_ptr = (char*) malloc (strlen (str) + 1);
    memcpy (tmp_ptr, str, strlen (str) + 1);
    int retval = mvwaddnstr (win, y, x, tmp_ptr, n);
    free (tmp_ptr);
    return retval;
}
#define mymvwaddnstr(a,b,c,d,e) mvwaddnstr_c(a,b,c,d,e)
#else // MVWADDSTR_USE_CHAR
#define mymvwaddnstr(a,b,c,d,e) mvwaddnstr(a,b,c,d,e)
#endif // MVWADDSTR_USE_CHAR

#if !defined(HAVE_MVWCHGAT) || ( defined(_XOPEN_CURSES) && !defined(__NCURSES_H) )

#ifdef HAVE_ALLOCA_H
# include <alloca.h>
#elif defined __GNUC__
# define alloca __builtin_alloca
#elif defined _AIX
# define alloca __alloca
#elif defined _MSC_VER
# include <malloc.h>
# define alloca _alloca
#else // HAVE_ALLOCA_H
# ifdef HAVE_ALLOCA
#  include <stdlib.h>
# else // HAVE_ALLOCA
#  include <stddef.h>
#  ifdef  __cplusplus
extern "C"
#  endif // __cplusplus
void *alloca (size_t);
# endif // HAVE_ALLOCA
#endif // HAVE_ALLOCA_H

inline int _mvwchgat_ (WINDOW* w, int y, int x, int n, int attr, short color, const void*) {
    char* buff = (char*) alloca (n);

    if (buff == NULL)
        return ERR;

    int retval = mvwinnstr (w, y, x, buff, n);

    if (retval == ERR)
        return retval;

    retval = wattron (w, attr | COLOR_PAIR (color) );

    if (retval == ERR)
        return retval;

    retval = mymvwaddnstr (w, y, x, buff, n);

    if (retval == ERR)
        return retval;

    retval = wattroff (w, attr | COLOR_PAIR (color) );

    if (retval == ERR)
        return retval;

    return OK;
}

#define mymvwchgat(a,b,c,d,e,f,g) _mvwchgat_(a,b,c,d,e,f,g)
#else
#define mymvwchgat(a,b,c,d,e,f,g) mvwchgat(a,b,c,d,e,f,g)
#endif // HAVE_MVWCHGAT

#endif // _CURSWA_H
