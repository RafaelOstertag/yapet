// -*- mode: c++ -*-

#ifndef CURS_H
#define CURS_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_STRING
#include <string>
#endif // HAVE_STRING

#include "mycurses.h"
#include "rectangle.h"
#include "lineobject.h"
#include "statusline.h"
#include "window.h"
#include "event.h"


class Curses {
    private:
	static WINDOW* w;
	static StatusLine* statusline;
	static LineObject* title;
	static Window* mainwindow;
	static bool initialized;
	static Rectangle<> scrdim;

    public:
	static void init();
	static void end();

	static void show();
	static int resize(EventBase& e);

	static void setTitle(LineObject* _title);
	static LineObject* getTitle();
	static void unsetTitle();

	static void setStatusLine(StatusLine* _sl);
	static StatusLine* getStatusLine();
	static void unsetStatusLine();

	static void setWindow(Window* _w);
	static Window* getWindow();
	static void unsetWindow();

	static Rectangle<> inquiryScreenSize();
};

#endif
