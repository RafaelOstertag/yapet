// -*- mode: c++ -*-

#ifndef CURS_H
#define CURS_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <string>

#include "mycurses.h"

#include "curex.h"
#include "statusline.h"

class Curses {
    private:
	static std::string title;
	static WINDOW* w;
	static StatusLine* statusline;
	static bool initialized;
	static bool hasstatusline;
	static bool hastitle;
	static int nlines, ncols, y, x;

    protected:
	static void putTitle();
    
    
    public:
	static void init(bool _statusline);
	static void end();

	static void show();

	static void setTitle(const std::string& t);
	static void setTitle(const char* t);
	static std::string getTitle();
	static void unsetTitle();

	static void pushStatus(const std::string& m);
	static void pushStatus(const char* m);
	static void popStatus();

	static Dimension getDimension();
	static Dimension getStatusDimension();
};

#endif
