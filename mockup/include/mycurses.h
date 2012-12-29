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

#include <curses.h>

#ifdef refresh
#undef refresh
#endif

#ifdef erase
#undef erase
#endif

#ifdef clear
#undef clear
#endif

#endif
