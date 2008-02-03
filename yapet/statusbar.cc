// $Id$

#include "statusbar.h"

void
StatusBar::createWindow() throw(GPSUI::UIException){
    if (statusbar != NULL)
	throw GPSUI::UIException("May you consider deleting the window before allocating");

    statusbar = newwin (1, maxX(), maxY() - 1, minX());
    if (statusbar == NULL)
        throw GPSUI::UIException ("statusbar could not be initialized");

    int retval = wattron (statusbar, A_REVERSE);
    if (retval == ERR)
        throw GPSUI::UIException ("Error setting attribute");

    retval = wbkgd (statusbar, ' ' | A_REVERSE);
    if (retval == ERR)
        throw GPSUI::UIException ("Error setting the statusbar background");

    refresh();
}

StatusBar::StatusBar() throw (GPSUI::UIException) : Resizeable(),
						    statusbar(NULL) {
    createWindow();
}

StatusBar::~StatusBar() {
    delwin (statusbar);
}

void
StatusBar::putMsg (std::string msg) throw (GPSUI::UIException) {
    message = msg;
    int retval = wclear (statusbar);
    if (retval == ERR)
        throw GPSUI::UIException ("Error erasing status bar");
    retval = mywaddstr (statusbar, message.c_str());
    if (retval == ERR)
        throw GPSUI::UIException ("Error adding status message");
    retval = wrefresh(statusbar);
    if (retval == ERR)
	throw GPSUI::UIException("Error refreshing status bar");
}

void
StatusBar::refresh() {
    // Does a refresh
    putMsg (message);
}

void
StatusBar::resize() {
    int retval = wattroff(statusbar, A_REVERSE);
    if (retval == ERR)
	throw GPSUI::UIException("attribute on status bar could not be set");
    
    retval = wclear(statusbar);
    if (retval == ERR)
        throw GPSUI::UIException ("status bar could not be cleared");
    retval = wrefresh(statusbar);
    if (retval == ERR)
        throw GPSUI::UIException ("status bar could not be refreshed");

    retval = delwin(statusbar);
    if (retval == ERR)
	throw GPSUI::UIException("status bar could not be deleted");

    statusbar = NULL;

    createWindow();
}
