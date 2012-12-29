// -*- mode: c++ -*-

#ifndef CURS_H
#define CURS_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <list>
#include <string>
#include <stdexcept>

#include "mycurses.h"

#include "curex.h"
#include "window.h"

// forward declaration
class Dimension;

class Curses {
    private:
	static std::list<Window*> window_list;
	static std::string title;
	static WINDOW *w;
	static bool initialized;
	static bool hasstatusline;
	static bool hastitle;
	static int nlines, ncols, y, x;

    protected:
	static void putTitle();
    
    
    public:
	static void init();
	static void end();

	static void show();

	static void regwin(Window* W);
	static void unregwin(Window* W);

	static void setTitle(const std::string& t);
	static void setTitle(const char* t);
	static std::string getTitle();
	static void unsetTitle();

	static Dimension getDimension();
};

#endif
