// -*- mode: c++ -*-

#ifndef CURS_H
#define CURS_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_SIGNAL_H
#include <signal.h>
#endif // HAVE_SIGNAL_H

#ifdef HAVE_STRING
#include <string>
#endif // HAVE_STRING

#include "mycurses.h"
#include "lineobject.h"
#include "statusline.h"
#include "window.h"


class Curses {
    private:
	static sigset_t sigoldmask;
	static struct sigaction sigoldaction;

	static WINDOW* w;
	static StatusLine* statusline;
	static LineObject* title;
	static Window* mainwindow;
	static bool initialized;
	static Rectangle<> scrdim;

	static void setupSignal();
	static void restoreSignal();

	static void signal_handler(int, siginfo_t*, void*);

    protected:
	static void inquiryScreenSize();

    public:
	static void init();
	static void end();

	static void show();
	static void resize();

	static void setTitle(LineObject* _title);
	static LineObject* getTitle();
	static void unsetTitle();

	static void setStatusLine(StatusLine* _sl);
	static StatusLine* getStatusLine();
	static void unsetStatusLine();

	static void setWindow(Window* _w);
	static Window* getWindow();
	static void unsetWindow();
	    
};

#endif
