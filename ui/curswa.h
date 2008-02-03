// -*- c++ -*-
//
// $Id$

/** Holds workarounds for curses.h
 * 
 * Holds workarounds for curses.h when compiling under C++. It specifically
 * removes the macros:
 *
 * - box
 * - clear
 * - erase
 * - move
 * - refresh
 *
 * and replaces them by inline functions.
 *
 * If 
 */

#ifndef _CURSWA_H
#define _CURSWA_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_CURSES_H

#ifdef box
#undef box
inline int box(WINDOW* win, int verch, int horch) {
    return wborder(win, verch, verch, horch, horch, 0, 0, 0, 0);
}
#endif

#ifdef clear
#undef clear
inline int clear() {
    return wclear(stdscr);
}
#endif

#ifdef erase
#undef erase
inline int erase() {
    return werase(stdscr);
}
#endif

#ifdef move
#undef move
inline int move(int y, int x) {
    return wmove(stdscr, y, x);
}
#endif

#ifdef refresh
#undef refresh
inline int refresh() {
    return wrefresh(stdscr);
}
#endif

#endif // HAVE_CURSES_H

#ifdef WADDSTR_USE_CHAR
#include <stdlib.h>
#include <string.h>

inline int waddstr_c(WINDOW* win, const char* str) {
    char* tmp_ptr = (char*)malloc(strlen(str)+1);
    memcpy(tmp_ptr, str, strlen(str)+1);
    int retval = waddstr(win, tmp_ptr);
    free(tmp_ptr);
    return retval;
}
#define mywaddstr(a,b) waddstr_c(a,b)
#else // WADDSTR_USE_CHAR
#define mywaddstr(a,b) waddstr(a,b)
#endif // WADDSTR_USE_CHAR

#ifdef MVWADDSTR_USE_CHAR
#include <stdlib.h>
#include <string.h>

inline int mvwaddstr_c(WINDOW* win, int y, int x, const char* str) {
    char* tmp_ptr = (char*)malloc(strlen(str)+1);
    memcpy(tmp_ptr, str, strlen(str)+1);
    int retval = mvwaddstr(win, y, x, tmp_ptr);
    free(tmp_ptr);
    return retval;
}
#define mymvwaddstr(a,b,c,d) mvwaddstr_c(a,b,c,d)
#else // MVWADDSTR_USE_CHAR
#define mymvwaddstr(a,b,c,d) mvwaddstr(a,b,c,d)
#endif // MVWADDSTR_USE_CHAR

#ifdef MVWADDNSTR_USE_CHAR
#include <stdlib.h>
#include <string.h>

inline int mvwaddnstr_c(WINDOW* win, int y, int x, const char* str, int n) {
    char* tmp_ptr = (char*)malloc(strlen(str)+1);
    memcpy(tmp_ptr, str, strlen(str)+1);
    int retval = mvwaddnstr(win, y, x, tmp_ptr, n);
    free(tmp_ptr);
    return retval;
}
#define mymvwaddnstr(a,b,c,d,e) mvwaddnstr_c(a,b,c,d,e)
#else // MVWADDSTR_USE_CHAR
#define mymvwaddnstr(a,b,c,d,e) mvwaddnstr(a,b,c,d,e)
#endif // MVWADDSTR_USE_CHAR

#ifndef HAVE_MVWCHGAT

#ifdef HAVE_ALLOCA_H
# include <alloca.h>
#elif defined __GNUC__
# define alloca __builtin_alloca
#elif defined _AIX
# define alloca __alloca
#elif defined _MSC_VER
# include <malloc.h>
# define alloca _alloca
#else
# include <stddef.h>
# ifdef  __cplusplus
extern "C"
# endif
void *alloca (size_t);
#endif
     

inline int mvwchgat(WINDOW* w, int y, int x, int n, int attr, short color, const void*) {
    char* buff = (char*)alloca(n);
    if (buff == NULL)
	return ERR;
    int retval = mvwinnstr(w, y, x, buff, n);
    if (retval == ERR)
	return retval;
    retval = wattron(w, attr | COLOR_PAIR(color));
    if (retval == ERR)
	return retval;
    retval = mymvwaddnstr(w, y, x, buff, n);
    if (retval == ERR)
	return retval;
    retval = wattroff(w, attr | COLOR_PAIR(color));
    if (retval == ERR)
	return retval;

    return OK;
}

#endif // HAVE_MVWCHGAT

#endif // _CURSWA_H
