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

#include <iostream>

#include "colors.h"
#include "mainwindow.h"

int main (int argc, char** argv) {
    MainWindow* mainwin = NULL;

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
    endwin();

    return 0;
}
