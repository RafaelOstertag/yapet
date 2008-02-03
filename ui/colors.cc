// $Id$

#include "colors.h"

using namespace GPSUI;

bool Colors::initialized = false;

color Colors::colors[] = { 
    // Normal
    {1, COLOR_WHITE, COLOR_BLUE, A_NORMAL},
    // Message box title
    {2, COLOR_CYAN, COLOR_BLACK, A_NORMAL},
    // Message box
    {3, COLOR_BLACK, COLOR_CYAN, A_NORMAL},
    // Input widget no focus
    {4, COLOR_BLACK, COLOR_WHITE, A_REVERSE},
    // Input widget focus
    {5, COLOR_BLACK, COLOR_YELLOW, A_REVERSE},
    // Button no focus
    {6, COLOR_WHITE, COLOR_BLACK, A_NORMAL},
    // Button focus    
    {7, COLOR_BLACK, COLOR_GREEN, A_REVERSE},
    // List widget
    {8, COLOR_WHITE, COLOR_BLUE, A_NORMAL} 
};



void
Colors::initColors() {
    if (initialized) return;

    if (has_colors() == FALSE) {
	initialized = true;
	return;
    }

    start_color();
    init_pair(colors[DEFAULT].no,
	      colors[DEFAULT].fg,
	      colors[DEFAULT].bg);

    init_pair(colors[MESSAGEBOX_TITLE].no,
	      colors[MESSAGEBOX_TITLE].fg,
	      colors[MESSAGEBOX_TITLE].bg);

    init_pair(colors[MESSAGEBOX].no,
	      colors[MESSAGEBOX].fg,
	      colors[MESSAGEBOX].bg);

    init_pair(colors[INPUTWIDGET_NOFOCUS].no,
	      colors[INPUTWIDGET_NOFOCUS].fg,
	      colors[INPUTWIDGET_NOFOCUS].bg);

    init_pair(colors[INPUTWIDGET_FOCUS].no,
	      colors[INPUTWIDGET_FOCUS].fg,
	      colors[INPUTWIDGET_FOCUS].bg);

    init_pair(colors[BUTTON_NOFOCUS].no,
	      colors[BUTTON_NOFOCUS].fg,
	      colors[BUTTON_NOFOCUS].bg);

    init_pair(colors[BUTTON_FOCUS].no,
	      colors[BUTTON_FOCUS].fg,
	      colors[BUTTON_FOCUS].bg);

    init_pair(colors[LISTWIDGET].no,
	      colors[LISTWIDGET].fg,
	      colors[LISTWIDGET].bg);
    

    initialized = true;
}

void
Colors::setcolor(WINDOW* w, COLORS c) {
    if (has_colors() == TRUE) {
	wattron(w, COLOR_PAIR(colors[c].no));
	wbkgd(w, ' ' | COLOR_PAIR(colors[c].no));
    } else {
	wattron(w, colors[c].attr);
	wbkgd(w, ' ' | colors[c].attr);
    }
}

short
Colors::getcolor(COLORS c) {
    if (has_colors() == TRUE) {
	return colors[c].no;
    }
    return 0;
}

void
Colors::unsetcolor(WINDOW* w, COLORS c) {
    if (has_colors() == TRUE)
	wattroff(w, COLOR_PAIR(colors[c].no));
    else
	wattroff(w, colors[c].attr);

    wbkgd(w, ' ');
}
