// $Id$

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

#ifdef HAVE_NCURSES_H
# include <ncurses.h>
#else // HAVE_NCURSES_H
# ifdef HAVE_CURSES_H
#  include <curses.h>
# else
#  error "Neither curses.h nor ncurses.h available"
# endif // HAVE_CURSES_H
#endif // HAVE_NCURSES_H
#include "curswa.h" // Leave this here. It depends on the above includes.

#ifdef TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# ifdef HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif

#ifdef HAVE_SYS_RESOURCE_H
# include <sys/resource.h>
#endif

#include <string.h>
#include <errno.h>

#include <stdio.h>
#include <iostream>

#include "colors.h"
#include "mainwindow.h"

void print_rlimit_err() {
    
}

void set_rlimit() {
#if defined(HAVE_SETRLIMIT) && defined(RLIMIT_CORE)
    rlimit rl;

    rl.rlim_cur = 0;
    rl.rlim_max = 0;
    int retval = setrlimit(RLIMIT_CORE, &rl);
    if (retval != 0) {
	std::cerr << "Failed to suppress the creation of core file." << std::endl 
		  << "The error message is: " << strerror(errno) << std::endl
		  << "In case a core file is created, it may contain clear text passwords." << std::endl
		  << std::endl 
		  << "Press <ENTER> to continue" << std::endl;
	getchar();
    }
#else
	std::cerr << "Cannot suppress the creation of core file." << std::endl 
		  << "In case a core file is created, it may contain clear text passwords." << std::endl
		  << std::endl 
		  << "Press <ENTER> to continue" << std::endl;
	getchar();
#endif
}

int main (int argc, char** argv) {
    MainWindow* mainwin = NULL;

    set_rlimit();

    initscr();
    raw();
    noecho();
    refresh();
    curs_set(0);
    keypad (stdscr, TRUE);

    GPSUI::Colors::initColors();

    try {
	mainwin = new MainWindow();
	mainwin->run();
	delete mainwin;
    } catch (std::exception& ex) {
	if (mainwin != NULL)
	    delete mainwin;
	clear();
	endwin();
	std::cerr << ex.what() << std::endl << std::endl;
	return 1;
    }

    clear();
    refresh();
    endwin();

    return 0;
}
