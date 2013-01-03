// -*- mode: c++ -*-

#ifndef MYCURSES_H
#define MYCURSES_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

// In case it is compiled with g++
#ifndef _BOOL
#define _BOOL
#endif

#ifdef HAVE_NCURSES_H
#include <ncurses.h>
#elif HAVE_CURSES_H
#include <curses.h>
#else
#error "Neither ncurses.h nor curses.h available"
#endif

#ifdef HAVE_CURSES_H

#if defined(tab) && defined(_XOPEN_CURSES)
#define __tab tab
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
#ifdef HAVE_CSTDLIB
# include <cstdlib>
#endif
#ifdef HAVE_CSTRING
# include <cstring>
#endif

inline int waddstr_c (WINDOW* win, const char* str) {
    char* tmp_ptr = strdup(str);
    int retval = waddstr (win, tmp_ptr);
    memset (tmp_ptr, 0, strlen(tmp_ptr));
    free (tmp_ptr);
    return retval;
}
#define mywaddstr(a,b) waddstr_c(a,b)
#else // WADDSTR_USE_CHAR
#define mywaddstr(a,b) waddstr(a,b)
#endif // WADDSTR_USE_CHAR

#ifdef MVWADDSTR_USE_CHAR
#ifdef HAVE_CSTDLIB
# include <cstdlib>
#endif
#ifdef HAVE_CSTRING
# include <cstring>
#endif

inline int mvwaddstr_c (WINDOW* win, int y, int x, const char* str) {
    char* tmp_ptr = strdup(str);
    int retval = mvwaddstr (win, y, x, tmp_ptr);
    memset(tmp_ptr, 0, strlen(tmp_ptr));
    free (tmp_ptr);
    return retval;
}
#define mymvwaddstr(a,b,c,d) mvwaddstr_c(a,b,c,d)
#else // MVWADDSTR_USE_CHAR
#define mymvwaddstr(a,b,c,d) mvwaddstr(a,b,c,d)
#endif // MVWADDSTR_USE_CHAR

#ifdef MVWADDNSTR_USE_CHAR
#ifdef HAVE_CSTDLIB
# include <cstdlib>
#endif
#ifdef HAVE_CSTRING
# include <cstring>
#endif

inline int mvwaddnstr_c (WINDOW* win, int y, int x, const char* str, int n) {
    char* tmp_ptr = strdup(str);
    int retval = mvwaddnstr (win, y, x, tmp_ptr, n);
    memset (tmp_ptr, 0, strlen(tmp_ptr));
    free (tmp_ptr);
    return retval;
}
#define mymvwaddnstr(a,b,c,d,e) mvwaddnstr_c(a,b,c,d,e)
#else // MVWADDSTR_USE_CHAR
#define mymvwaddnstr(a,b,c,d,e) mvwaddnstr(a,b,c,d,e)
#endif // MVWADDSTR_USE_CHAR

#endif
