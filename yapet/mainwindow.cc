// $Id$
//
// Copyright (C) 2008, 2009  Rafael Ostertag
//
// This file is part of YAPET.
//
// YAPET is free software: you can redistribute it and/or modify it under the
// terms of the GNU General Public License as published by the Free Software
// Foundation, either version 3 of the License, or (at your option) any later
// version.
//
// YAPET is distributed in the hope that it will be useful, but WITHOUT ANY
// WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
// FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
// details.
//
// You should have received a copy of the GNU General Public License along with
// YAPET.  If not, see <http://www.gnu.org/licenses/>.
//

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_FCNTL_H
# include <fcntl.h>
#endif

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif

#ifdef HAVE_SYS_STAT_H
# include <sys/stat.h>
#endif

#ifdef HAVE_SIGNAL_H
# include <signal.h>
#endif

#ifdef HAVE_ERRNO_H
# include <errno.h>
#endif

#ifdef HAVE_STRING_H
# include <string.h>
#endif

#ifdef TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# ifdef HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif // TIME_WITH_SYS_TIME

#include <button.h>
#include <dialogbox.h>
#include <colors.h>
#include <misc.h>

#include "../intl.h"
#include "mainwindow.h"
#include "fileopen.h"
#include "passworddialog.h"
#include "passwordrecord.h"
#include "searchdialog.h"

/**
 * @brief Structure defining a key for the \c MainWindow.
 *
 * This struct defines a key that will be displayed in the main menu of \c
 * MainWindow.
 */
struct KeyDesc {
	/**
	 * @brief The y-position of the key.
	 *
	 * The y-position of the key.
	 */
	int y;
	/**
	 * @brief The x-position of the key.
	 *
	 * The x-position of the key.
	 */
	int x;
	/**
	 * @brief The key.
	 *
	 * The key to be pressed to call the menu item.
	 */
	const char* key;
	/**
	 * @brief The description of the key.
	 *
	 * Describes the function of the menu item.
	 */
	const char* desc;
};

/*
 * The _ macro is undefined and redefined to a noop, so we can trick the
 * xgettext program to read strings to translate. After that, _ is restored to
 * its previous state (taken from intl.h).
 *
 * The actual translation using gettext is done in
 * MainWindow::topRightWinContent()
 *
 * Better approaches welcome.
 */
#ifdef _
#undef _
#endif
#define _(String) String
/**
 * @brief The menu of \c MainWindow.
 *
 * Those are the keys used for the main menu of the \c MainWindow class.
 */
KeyDesc keys[] = { {3, 2, "S", _("Save File")},
		   {4, 2, "R", _("Load File")},
		   {5, 2, "L", _("Lock Screen")},
		   {6, 2, "A", _("Add Entry")},
		   {7, 2, "D", _("Delete Entry")},
		   {8, 2, "O", _("Sort Order")},
		   {9, 2, "/", _("Search")},
		   {10, 2, "N", _("Search Next")},
		   {11, 2, "C", _("Change Password")},
		   {12, 2, "^L", _("Redraw Screen")},
		   {13, 2, "Q", _("Quit")},
		   {0, 0, NULL, NULL}
};
#undef _
#if ! defined(_) && ENABLE_NLS==0
#define _(String) (String)
#endif

#if ! defined(_) && ENABLE_NLS==1
#define _(String) gettext(String)
#endif


#if defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
/**
 * @brief Class for calling the signal handler of \c MainWindow.
 *
 * This class is passed to \c BaseWindow::setTimeout() as class for calling the
 * signal handler of \c MainWindow.
 */
class Alarm : public YAPETUI::BaseWindow::AlarmFunction {
    private:
	MainWindow& ref;
    public:
	inline Alarm(MainWindow& r) : ref(r) {}
	inline void process(int signo) {
	    ref.handle_signal(signo);
	}
};
#endif // defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)

void
MainWindow::printTitle() throw(YAPETUI::UIException) {
    YAPETUI::Colors::setcolor(stdscr, YAPETUI::DEFAULT);

    // Clear the previous title
    int retval = wmove(stdscr, 0,0);
    if (retval == ERR)
	throw YAPETUI::UIException(_("Error moving cursor"));
    retval = wclrtoeol(stdscr);
    if (retval == ERR)
	throw YAPETUI::UIException(_("Error clearing line"));

    // Title
    char title[100];
    snprintf(title,100, "..::|| %s ||::..", PACKAGE_STRING);

    // Position the title
    retval = wmove(stdscr, 0, maxX()/2 - strlen(title)/2);
    if (retval == ERR)
	throw YAPETUI::UIException(_("Error moving cursor"));
    retval = mywaddstr(stdscr, title);
    if (retval == ERR)
	throw YAPETUI::UIException(_("Error printing title"));

    std::string terminal_title;
    if ( file != NULL )
	terminal_title = "YAPET (" + file->getFilename() + ")";
    else
	terminal_title = "YAPET";

    setTerminalTitle(terminal_title);
}

void
MainWindow::topRightWinContent() throw (YAPETUI::UIException) {
    int max_y, max_x;
    getmaxyx (toprightwin, max_y, max_x);

    int start_title_x = max_x / 2 - strlen (_("K E Y S")) / 2;

    int retval = mymvwaddstr (toprightwin, 1, start_title_x, _("K E Y S"));
    if (retval == ERR)
	throw YAPETUI::UIException (_("mvwaddstr() blew it"));
    retval = wmove (toprightwin, 2, 1);
    if (retval == ERR)
	throw YAPETUI::UIException (_("wmove() blew it"));

#if defined(_XOPEN_CURSES) && !defined(__NCURSES_H)
    retval = whline (toprightwin, '-', max_x - 2);
#else
    retval = whline (toprightwin, 0, max_x - 2);
#endif
    if (retval == ERR)
	throw YAPETUI::UIException (_("whline() blew it"));


    KeyDesc* ptr = keys;
    while (ptr->key != NULL
	    && ptr->desc != NULL) {
	wattron (toprightwin, A_REVERSE);
	retval = mvwprintw (toprightwin, ptr->y, ptr->x, "  %0-2s  ", ptr->key);
	if (retval == ERR)
	    throw YAPETUI::UIException (_("mvprintw() blew it"));
	wattroff (toprightwin, A_REVERSE);
	// The translation of the keys is done here
	retval = mymvwaddstr (toprightwin, ptr->y, ptr->x + 8, _(ptr->desc));
	if (retval == ERR)
	    throw YAPETUI::UIException (_("waddstr() blew it"));

	ptr++;
    }

}

void
MainWindow::bottomRightWinContent() throw(YAPETUI::UIException) {
    if (key == NULL || recordlist == NULL) return;

    int retval = 0;
    if (usefsecurity)
	retval = mymvwaddstr(bottomrightwin, 1, 2, _("File permission check: enabled"));
    else
	retval = mymvwaddstr(bottomrightwin, 1, 2, _("File permission check: disabled"));
    if (retval == ERR)
	throw YAPETUI::UIException (_("waddstr() blew it"));
    retval = mvwprintw(bottomrightwin, 2, 2, _("Screen lock time-out: %u sec"), locktimeout);
    if (retval == ERR)
	throw YAPETUI::UIException (_("mvprintw() blew it"));
    retval = mvwprintw(bottomrightwin, 3, 2, _("%d entries "), recordlist->size());
    if (retval == ERR)
	throw YAPETUI::UIException (_("mvprintw() blew it"));

#if defined(HAVE_ASCTIME) && defined(HAVE_LOCALTIME)
    if (file != NULL) {
	try {
	    time_t t = file->getMasterPWSet(*key);
	    retval = mvwprintw(bottomrightwin, 4, 2, _("PW set: %s"),
			       asctime(localtime(&t)) );
	    if (retval == ERR)
		throw YAPETUI::UIException (_("mvprintw() blew it"));
	} catch (YAPET::YAPETException& ex) {
	    statusbar.putMsg(ex.what());
	}
    }
#endif

}

void
MainWindow::createWindow() throw(YAPETUI::UIException) {
    if (toprightwin != NULL || bottomrightwin != NULL)
	throw YAPETUI::UIException(_("May you consider deleting the window before reallocating"));
    int middleX = maxX() / 2;
    int thirdY = maxY() / 3 - 1;

    printTitle();

    //
    // Top right window
    //
    toprightwin = newwin (maxY() - thirdY - 1 , maxX() - middleX, 1, middleX);
    if (toprightwin == NULL)
	throw YAPETUI::UIException (_("newwin() returned NULL"));

    YAPETUI::Colors::setcolor(toprightwin, YAPETUI::DEFAULT);
    int retval = box (toprightwin, 0, 0);
    if (retval == ERR)
	throw YAPETUI::UIException (_("box() blew it"));

    topRightWinContent();

    //
    // Bottom right window
    //
    bottomrightwin = newwin (thirdY - 1 , maxX() - middleX, maxY()-thirdY, middleX);
    if (bottomrightwin == NULL)
	throw YAPETUI::UIException (_("newwin() returned NULL"));

    YAPETUI::Colors::setcolor(bottomrightwin, YAPETUI::DEFAULT);
    retval = werase (bottomrightwin);
    if (retval == ERR)
	throw YAPETUI::UIException(_("werase() blew it"));
    retval = box (bottomrightwin, 0, 0);
    if (retval == ERR)
	throw YAPETUI::UIException (_("box() blew it"));


    //
    // List widget on the left
    //
    if (recordlist == NULL) {
	std::list<YAPET::PartDec> emptylist;
	recordlist = new YAPETUI::ListWidget<YAPET::PartDec> (emptylist,
							     0,
							     1,
							     maxX() / 2,
							     maxY() - 2);
    }

    bottomRightWinContent();
}

void
MainWindow::resize() throw (YAPETUI::UIException) {
    int retval = delwin(toprightwin);
    if (retval == ERR)
	throw YAPETUI::UIException(_("delwin() blew it"));

    retval = delwin(bottomrightwin);
    if (retval == ERR)
	throw YAPETUI::UIException(_("delwin() blew it"));

    toprightwin = NULL;
    bottomrightwin = NULL;


    createWindow();

    recordlist->resize (0, 1, maxX()/2, maxY() - 2);
}

void
MainWindow::refresh() throw (YAPETUI::UIException) {
    printTitle();

    int retval = wrefresh(stdscr);
    if (retval == ERR)
	throw YAPETUI::UIException(_("Error refreshing stdscr"));

    topRightWinContent();
    bottomRightWinContent();

    retval = box(toprightwin, 0, 0);
    if (retval == ERR)
	throw YAPETUI::UIException (_("Error setting border"));

    retval = box(bottomrightwin, 0, 0);
    if (retval == ERR)
	throw YAPETUI::UIException (_("Error setting border"));


    retval = wrefresh (toprightwin);
    if (retval == ERR)
	throw YAPETUI::UIException (_("Error refreshing top right window"));

    retval = wrefresh (bottomrightwin);
    if (retval == ERR)
	throw YAPETUI::UIException (_("Error refreshing bottom right window"));

    recordlist->refresh();
    statusbar.refresh();
}

void
MainWindow::createFile(std::string& filename) throw(YAPETUI::UIException) {
    closeFile();

    PasswordDialog* pwdia = NULL;
    try {
	pwdia = new PasswordDialog(NEW_PW, filename);
	pwdia->run();
	key = pwdia->getKey();
	delete pwdia;
    } catch(YAPETUI::UIException&) {
	if (pwdia != NULL)
	    delete pwdia;

	statusbar.putMsg(_("Error while asking for password"));
	return;
    }

    if (key == NULL) {
	statusbar.putMsg(_("Creation of file canceled"));
	return;
    }

    try {
	file = new YAPET::File(filename, *key, true, usefsecurity);
	statusbar.putMsg(filename + _(" created"));
	records_changed = false;
    } catch(YAPET::YAPETException& ex) {
	YAPETUI::MessageBox* msgbox = NULL;
	try{
	    msgbox = new YAPETUI::MessageBox(_("E R R O R"), ex.what());
	    msgbox->run();
	    delete msgbox;
	} catch (YAPETUI::UIException&) {
	    if (msgbox != NULL)
		delete msgbox;

	    statusbar.putMsg(_("Error showing error message"));
	}
	closeFile();
    }
}

void
MainWindow::openFile(std::string filename) throw(YAPETUI::UIException) {
    struct stat st;
    int retval = stat(filename.c_str(), &st);
    if (retval == -1 && errno == ENOENT) {
	// Ask user whether or not he wants to create a new file
	YAPETUI::DialogBox* question =
	    new YAPETUI::DialogBox(_("Q U E S T I O N"),
				   _("The file does not exist. Do you want to create it?"));
	question->run();
	YAPETUI::ANSWER a = question->getAnswer();
	delete question;
	if ( a == YAPETUI::ANSWER_OK) {
	    createFile(filename);
	    return;
	} else {
	    statusbar.putMsg(_("File creation canceled"));
	    return;
	}
    } else if (retval == -1) {
	// Inform user about other error
	YAPETUI::MessageBox* errmsg = NULL;
	try {
	    errmsg = new YAPETUI::MessageBox(_("E R R O R"), strerror(errno));
	    errmsg->run();
	    delete errmsg;
	} catch (YAPETUI::UIException&) {
	    if (errmsg != NULL)
		delete errmsg;
	}
	refresh();
	return;
    }

    // We were able to stat the file, no make sure it is a file and open it
    // using the password
    if (!S_ISREG(st.st_mode)) {
	YAPETUI::MessageBox* errmsg = NULL;
	try {
	    errmsg = new YAPETUI::MessageBox(_("E R R O R"),
					     _("The specified file is not a regular file"));
	    errmsg->run();
	    delete errmsg;
	} catch (YAPETUI::UIException&) {
	    if (errmsg != NULL)
		delete errmsg;
	}
	return;
    }

    closeFile();

    // Ask for password
    PasswordDialog* pwdia = NULL;
    try {
	pwdia = new PasswordDialog(EXISTING_PW, filename);
	pwdia->run();
	key = pwdia->getKey();
	delete pwdia;
    } catch (YAPETUI::UIException&) {
	if (pwdia != NULL)
	    delete pwdia;
	statusbar.putMsg(_("UI error while asking for password"));
    }

    // Open file
    if (key != NULL) {
	try {
	    file = new YAPET::File(filename, *key, false, usefsecurity);
	    std::list<YAPET::PartDec> tmp_list = file->read(*key);
	    recordlist->setList(tmp_list);
	    statusbar.putMsg(filename + _(" opened"));
	    return;
	} catch(YAPET::YAPETException& e) {
	    if (file != NULL)
		delete file;

	    YAPETUI::MessageBox* msgbox = NULL;
	    try {
		msgbox = new YAPETUI::MessageBox(_("E R R O R"), e.what());
		msgbox->run();
		delete msgbox;
	    } catch (YAPETUI::UIException&) {
		if (msgbox != NULL)
		    delete msgbox;
		statusbar.putMsg(_("Error while trying to show error"));
	    }
	    delete key;
	    key = NULL;
	    file = NULL;
	    statusbar.putMsg(_("Error opening file"));
	    return;
	}
    } else {
	statusbar.putMsg(_("Opening of ") + filename + _(" canceled"));
    }
}

void
MainWindow::saveFile() {
    if (key == NULL || file == NULL) return;

    try {
	file->save(recordlist->getList());
	records_changed = false;
	statusbar.putMsg(file->getFilename() + _(" saved"));
    } catch (YAPET::YAPETException& ex) {
	YAPETUI::MessageBox* msgbox = NULL;
	try {
	    msgbox = new YAPETUI::MessageBox(_("E R R O R"), ex.what());
	    msgbox->run();
	    delete msgbox;
	} catch (YAPETUI::UIException) {
	    if (msgbox != NULL)
		delete msgbox;
	    statusbar.putMsg(_("Error showing error message"));
	}
    }
}

void
MainWindow::closeFile() {
    // Remove key and close file
    if (key != NULL) {
	delete key;
	key = NULL;
    }
    if (file != NULL) {
	delete file;
	file = NULL;
    }

    // Clear list
    recordlist->getList().clear();
    records_changed = false;
}

void
MainWindow::addNewRecord() {
    if (key == NULL || file == NULL) return;
    PasswordRecord* pwentry = NULL;
    try {
	pwentry = new PasswordRecord(*key, NULL);
	pwentry->run();
	if (pwentry->entryChanged() &&
	    pwentry->getEncEntry() != NULL) {
	    recordlist->getList().push_back(*pwentry->getEncEntry());
	    recordlist->setSortOrder();
	    delete pwentry->getEncEntry();
	    records_changed = true;
	    statusbar.putMsg(_("New record added"));
	} else {
	    statusbar.putMsg(_("Record addition canceled"));
	}
	delete pwentry;
    } catch (YAPETUI::UIException& ex) {
	if (pwentry != NULL) {
	    if (pwentry->getEncEntry() != NULL)
		delete pwentry->getEncEntry();
	    delete pwentry;
	}

	YAPETUI::MessageBox* msgbox = NULL;
	try {
	    msgbox = new YAPETUI::MessageBox(_("E R R O R"), _("Error adding password entry"));
	    msgbox->run();
	    delete msgbox;
	} catch (YAPETUI::UIException&) {
	    if (msgbox != NULL)
		delete msgbox;

	    statusbar.putMsg(_("Error showing error message"));
	}

    }
    ::refresh();
    refresh();
}

void
MainWindow::editSelectedRecord() {
    if (key == NULL ||
	file == NULL ||
	recordlist->size() == 0) return;
    PasswordRecord* pwentry = NULL;
    try {
	YAPET::PartDec pd = recordlist->getSelectedItem();
	pwentry = new PasswordRecord(*key, &pd);
	pwentry->run();
	if (pwentry->entryChanged() &&
	    pwentry->getEncEntry() != NULL) {
	    recordlist->replaceCurrentItem(*pwentry->getEncEntry());
	    recordlist->setSortOrder();
	    records_changed = true;
	    statusbar.putMsg(_("Record edited"));
	    delete pwentry->getEncEntry();
	} else {
	    statusbar.putMsg(_("Record edition canceled"));
	}
	delete pwentry;
    } catch (YAPETUI::UIException& ex) {
	if (pwentry != NULL) {
	    if (pwentry->getEncEntry() != NULL)
		delete pwentry->getEncEntry();
	    delete pwentry;
	}

	YAPETUI::MessageBox* msgbox = NULL;
	try {
	    msgbox = new YAPETUI::MessageBox(_("E R R O R"), _("Error adding password entry"));
	    msgbox->run();
	    delete msgbox;
	} catch (YAPETUI::UIException&) {
	    if (msgbox != NULL)
		delete msgbox;

	    statusbar.putMsg(_("Error showing error message"));
	}
    }
    ::refresh();
    refresh();
}

void
MainWindow::deleteSelectedRecord() throw(YAPETUI::UIException){
    if (recordlist->size() < 1) return;

    YAPETUI::DialogBox* dialog = NULL;
    try {
	dialog = new YAPETUI::DialogBox(_("Q U E S T I O N"), _("Delete selected record?"));
	dialog->run();
	YAPETUI::ANSWER a = dialog->getAnswer();
	if (a == YAPETUI::ANSWER_OK) {
	    recordlist->deleteSelectedItem();
	    records_changed = true;
	    recordlist->refresh();
	    statusbar.putMsg(_("Record deleted"));
	    records_changed = true;
	} else {
	    statusbar.putMsg("");
	}
	delete dialog;
    } catch(YAPETUI::UIException&) {
	if (dialog != NULL)
	    delete dialog;

	YAPETUI::MessageBox* msgbox = NULL;
	try {
	    msgbox = new YAPETUI::MessageBox(_("E R R O R"), _("Error showing dialog"));
	    msgbox->run();
	    delete msgbox;
	} catch (YAPETUI::UIException&) {
	    if (msgbox != NULL)
		delete msgbox;

	    statusbar.putMsg(_("Error showing error message"));
	}
    }
    refresh();
}

void
MainWindow::setSortOrder() {
    try {
	switch (recordlist->getSortOrder()) {
	case(YAPETUI::ListWidget<YAPET::PartDec>::ASCENDING):
	    recordlist->setSortOrder(YAPETUI::ListWidget<YAPET::PartDec>::DESCENDING);
	    statusbar.putMsg(_("Set sort order descending"));
	    break;
	case(YAPETUI::ListWidget<YAPET::PartDec>::DESCENDING):
	    recordlist->setSortOrder(YAPETUI::ListWidget<YAPET::PartDec>::ASCENDING);
	    statusbar.putMsg(_("Set sort order ascending"));
	    break;
	};
	recordlist->refresh();
    } catch(std::exception& ex) {
	YAPETUI::MessageBox* msgbox = NULL;
	try {
	    msgbox = new YAPETUI::MessageBox(_("E R R O R"), ex.what());
	    msgbox->run();
	    delete msgbox;
	} catch (YAPETUI::UIException&) {
	    if (msgbox != NULL)
		delete msgbox;
	    statusbar.putMsg(_("Error showing error message"));
	}
    }
}

void
MainWindow::searchTerm() {
    if (key == NULL ||
	file == NULL ||
	recordlist->size() == 0) return;
    SearchDialog* searchdialog = NULL;
    try {
	searchdialog = new SearchDialog();
	searchdialog->run();
	if (!searchdialog->isCanceled()) {
	    if (recordlist->searchTerm(searchdialog->getSearchTerm()) ) {
		// simply clear the status bar
		statusbar.putMsg("");
	    } else {
		statusbar.putMsg(_("Search term not found"));
	    }
	} else {
	    statusbar.putMsg(_("Search canceled"));
	}
	delete searchdialog;
    } catch (YAPETUI::UIException& ex) {
	if (searchdialog != NULL) {
	    delete searchdialog;
	}

	YAPETUI::MessageBox* msgbox = NULL;
	try {
	    msgbox = new YAPETUI::MessageBox(_("E R R O R"), ex.what());
	    msgbox->run();
	    delete msgbox;
	} catch (YAPETUI::UIException&) {
	    if (msgbox != NULL)
		delete msgbox;

	    statusbar.putMsg(_("Error showing error message"));
	}
    }
    ::refresh();
    refresh();
}

void
MainWindow::searchNext() {
    if (key == NULL ||
	file == NULL ||
	recordlist->size() == 0) return;

    if (recordlist->searchNext() ) {
	    // simply clear the status bar
	statusbar.putMsg("");
    } else {
	statusbar.putMsg(_("Search term not found"));
    }
}

bool
MainWindow::quit() {
    if (!records_changed) return true;

    YAPETUI::DialogBox* dialogbox = NULL;
    try {
	dialogbox = new YAPETUI::DialogBox(_("Q U E S T I O N"), _("Save before quitting?"));
	dialogbox->run();
	YAPETUI::ANSWER a = dialogbox->getAnswer();
	delete dialogbox;
	if (a == YAPETUI::ANSWER_OK) {
	    saveFile();
	    return true;
	}

	return true;
    } catch (YAPETUI::UIException&) {
	if (dialogbox != NULL)
	    delete dialogbox;
	statusbar.putMsg(_("Error showing error message"));
	refresh();
	return false;
    }
}

void
MainWindow::lockScreen() const throw(YAPETUI::UIException){
    if (key == NULL) return;
    int ch;
    while (true) {
	WINDOW* lockwin = newwin(0,0,0,0);
	if (lockwin == NULL)
	    throw YAPETUI::UIException(_("Error creating lock window"));

	int retval = werase(lockwin);
	if (retval == ERR) {
	    delwin(lockwin);
	    throw YAPETUI::UIException(_("Error erasing window"));
	}

	retval = wrefresh(lockwin);
	if (retval == ERR) {
	    delwin(lockwin);
	    throw YAPETUI::UIException(_("Error refreshing window"));
	}

	std::string locked_title(_("YAPET -- Locked --"));
	setTerminalTitle(locked_title);

	ch = wgetch(lockwin);
#ifdef HAVE_WRESIZE
	if (ch == KEY_RESIZE) {
	    delwin(lockwin);
	    YAPETUI::BaseWindow::resizeAll();
	    continue;
	}
#endif
	PasswordDialog* pwdia = NULL;
	YAPET::Key* testkey = NULL;
	try {
	    pwdia = new PasswordDialog(EXISTING_PW, file->getFilename());
	    pwdia->run();
	    testkey = pwdia->getKey();
	    delete pwdia;
	} catch(YAPETUI::UIException&) {
	    if (pwdia != NULL)
		delete pwdia;
	    if (testkey != NULL)
		delete testkey;
	    delwin(lockwin);
	    continue;
	}

	if (testkey == NULL) {
	    delwin(lockwin);
	    continue;
	}

	if (*testkey != *key) {
	    YAPETUI::MessageBox* msgbox = NULL;
	    try {
		msgbox = new YAPETUI::MessageBox(_("E R R O R"), _("Wrong password"));
		msgbox->run();
		delete msgbox;
	    } catch (YAPETUI::UIException&) {
		if (msgbox != NULL)
		    delete msgbox;
	    }
	} else {
	    delete testkey;
	    delwin(lockwin);
	    return;
	}

	delete testkey;
	delwin(lockwin);
    }
}

void
MainWindow::changePassword() throw(YAPETUI::UIException) {
    if (file == NULL || key == NULL) return;

    // Make sure there are no unsaved entries
    if (records_changed) {
	YAPETUI::DialogBox* dialogbox = NULL;
	try {
	    dialogbox = new YAPETUI::DialogBox(_("Q U E S T I O N"), _("Save before changing password?"));
	    dialogbox->run();
	    YAPETUI::ANSWER a = dialogbox->getAnswer();
	    delete dialogbox;
	    if (a == YAPETUI::ANSWER_OK) {
		saveFile();
	    } else {
		statusbar.putMsg(_("Password change aborted"));
		return;
	    }
	} catch (YAPETUI::UIException&) {
	    if (dialogbox != NULL)
		delete dialogbox;
	    statusbar.putMsg(_("Error showing error message"));
	    refresh();
	    return;
	}
    }

    // Prompt for the new password
    PasswordDialog* pwdia = NULL;
    YAPET::Key* newkey;
    try {
	pwdia = new PasswordDialog(NEW_PW, file->getFilename());
	pwdia->run();
	newkey = pwdia->getKey();
	delete pwdia;
    } catch(YAPETUI::UIException&) {
	if (pwdia != NULL)
	    delete pwdia;

	statusbar.putMsg(_("Error while asking for password"));
	return;
    }

    // Make sure the key has been generated
    if (newkey == NULL) {
	statusbar.putMsg(_("Password change canceled"));
	return;
    }

    // Change the password
    try {
	file->setNewKey(*key, *newkey);
    } catch (std::exception& ex) {
	delete newkey;
	YAPETUI::MessageBox* msgbox = NULL;
	try {
	    msgbox = new YAPETUI::MessageBox(_("E R R O R"), ex.what());
	    msgbox->run();
	    delete msgbox;
	} catch (YAPETUI::UIException&) {
	    if (msgbox != NULL)
		delete msgbox;
	}
	return;
    }

    delete key;
    key = newkey;

    // Read records from file
    try {
	std::list<YAPET::PartDec> tmp_list = file->read(*key);
	recordlist->setList(tmp_list);
    } catch(YAPET::YAPETException& e) {
	    if (file != NULL)
		delete file;

	    YAPETUI::MessageBox* msgbox = NULL;
	    try {
		msgbox = new YAPETUI::MessageBox(_("E R R O R"), e.what());
		msgbox->run();
		delete msgbox;
	    } catch (YAPETUI::UIException&) {
		if (msgbox != NULL)
		    delete msgbox;
		statusbar.putMsg(_("Error while trying to show error"));
	    }
	    delete key;
	    key = NULL;
	    file = NULL;
	    statusbar.putMsg(_("Error reading from file"));
	    return;
    }

    statusbar.putMsg(_("Password successfully changed"));
}

MainWindow::MainWindow(unsigned int timeout,
		       bool fsecurity) throw (YAPETUI::UIException) : BaseWindow(),
								      toprightwin (NULL),
								      bottomrightwin (NULL),
								      recordlist (NULL),
								      statusbar(),
								      records_changed(false),
								      key (NULL),
								      file (NULL),
								      locktimeout(timeout),
								      usefsecurity(fsecurity) {
    locktimeout = locktimeout < 10 ? 10 : locktimeout;
    createWindow();
}

MainWindow::~MainWindow() {
    delete recordlist;
    wclear(toprightwin);
    wclear(bottomrightwin);
    delwin (toprightwin);
    delwin (bottomrightwin);
    if (key != NULL)
	delete key;
    if (file != NULL)
	delete file;


}

void
MainWindow::run() throw (YAPETUI::UIException) {

    if (file == NULL || key == NULL)
	statusbar.putMsg (_("No file loaded"));

    if (file != NULL && key != NULL)
	statusbar.putMsg(file->getFilename() + _(" loaded"));

    refresh();

    Alarm alrm(*this);
    int ch;
    while(true) {
	try {
#if defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
	    BaseWindow::setTimeout(&alrm,locktimeout);
#endif // defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
	    while ( (ch=recordlist->focus()) ) {
#if defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
		YAPETUI::BaseWindow::suspendTimeout();
#endif // defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
		switch (ch) {
		case '\n':
		    editSelectedRecord();
		    break;
		case 3: // ^C
		case 'Q':
		case 'q':
		    if (quit()) return;
		    break;
#ifdef HAVE_WRESIZE
		case KEY_RESIZE:
		    YAPETUI::BaseWindow::resizeAll();
		    break;
#endif // HAVE_WRESIZE
		case KEY_REFRESH:
#ifdef HAVE_WRESIZE
		    YAPETUI::BaseWindow::resizeAll();
#endif // HAVE_WRESIZE
		    YAPETUI::BaseWindow::refreshAll();
		    break;
		case 'S':
		case 's':
		    saveFile();
		    break;
		case 'R':
		case 'r': {
		    FileOpen* tmp = NULL;
		    try {
			tmp = new FileOpen(_("O P E N  F I L E"));
			tmp->run();
			if (!tmp->isCanceled()) {
			    openFile(tmp->getFilepath());
			}
			delete tmp;
		    } catch (std::exception& ex2) {
			statusbar.putMsg(ex2.what());
			if (file != NULL)
			    delete file;
			if (key != NULL)
			    delete key;
			if (tmp != NULL)
			  delete tmp;
			file = NULL;
			key = NULL;
		    }
		    ::refresh();
		    YAPETUI::BaseWindow::refreshAll();
		}
		    break;
		case 'L':
		case 'l':
		    lockScreen();
		    ::refresh();
		    YAPETUI::BaseWindow::refreshAll();
		    break;

		case 'A':
		case 'a':
		    addNewRecord();
		    break;

		case 'D':
		case 'd':
		    deleteSelectedRecord();
		    break;

		case 'O':
		case 'o':
		    setSortOrder();
		    break;

		case '/':
		    searchTerm();
		    break;

		case 'N':
		case 'n':
		    searchNext();
		    break;

		case 'c':
		case 'C':
		    changePassword();
		    ::refresh();
		    YAPETUI::BaseWindow::refreshAll();
		    break;
		}
#if defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
	    YAPETUI::BaseWindow::setTimeout(&alrm,600);
#endif // defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
	    }
	} catch(std::exception& ex) {
	    statusbar.putMsg(ex.what());
	}
    }
}

void
MainWindow::run(std::string fn) {
    if (fn.empty()) {
	run();
	return;
    }

    refresh();

    try {
	openFile(fn);
    } catch (std::exception& ex2) {
	statusbar.putMsg(ex2.what());
	if (file != NULL)
	    delete file;
	if (key != NULL)
	    delete key;
	file = NULL;
	key = NULL;
    }
    ::refresh();

    run();
}

#if defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
void
MainWindow::handle_signal(int signo) {
    if (signo == SIGALRM) {
	lockScreen();
	::refresh();
	YAPETUI::BaseWindow::refreshAll();
    }
}
#endif // defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
