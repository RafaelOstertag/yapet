// $Id$
//
// YAPET -- Yet Another Password Encryption Tool
// Copyright (C) 2008  Rafael Ostertag
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
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

#include <button.h>
#include <dialogbox.h>
#include <colors.h>

#include "mainwindow.h"
#include "fileopen.h"
#include "passworddialog.h"
#include "passwordrecord.h"


struct KeyDesc {
    int y;
    int x;
    const char* key;
    const char* desc;
};

KeyDesc keys[] = { {4, 2, "S", "Save File"},
		   {5, 2, "R", "Load File"},
		   {6, 2, "L", "Lock Screen"},
		   {7, 2, "A", "Add Entry"},
		   {8, 2, "D", "Delete Entry"},
		   {9, 2, "^L", "Redraw Screen"},
		   {10, 2, "Q", "Quit"},
		   {0, 0, NULL, NULL}
};

#if defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
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
    // Title
    char title[100];
    snprintf(title,100, "..::|| %s ||::..", PACKAGE_STRING);
    int retval = wmove(stdscr, 0, maxX()/2 - strlen(title)/2);
    if (retval == ERR)
	throw YAPETUI::UIException("Error moving cursor");
    retval = mywaddstr(stdscr, title);
    if (retval == ERR)
	throw YAPETUI::UIException("Error printing title");
}

void
MainWindow::topRightWinContent() throw (YAPETUI::UIException) {
    int max_y, max_x;
    getmaxyx (toprightwin, max_y, max_x);

    char win_title[] = "K E Y S";
    int start_title_x = max_x / 2 - strlen (win_title) / 2;

    int retval = mymvwaddstr (toprightwin, 1, start_title_x, win_title);
    if (retval == ERR)
	throw YAPETUI::UIException ("mvwaddstr() blew it");
    retval = wmove (toprightwin, 2, 1);
    if (retval == ERR)
	throw YAPETUI::UIException ("wmove() blew it");

    retval = whline (toprightwin, 0, max_x - 2);
    if (retval == ERR)
	throw YAPETUI::UIException ("whline() blew it");


    KeyDesc* ptr = keys;
    while (ptr->key != NULL
	    && ptr->desc != NULL) {

	wattron (toprightwin, A_REVERSE);
	retval = mvwprintw (toprightwin, ptr->y, ptr->x, "  %0-2s  ", ptr->key);
	if (retval == ERR)
	    throw YAPETUI::UIException ("mvprintw() blew it");
	wattroff (toprightwin, A_REVERSE);
	retval = mymvwaddstr (toprightwin, ptr->y, ptr->x + 8, ptr->desc);
	if (retval == ERR)
	    throw YAPETUI::UIException ("waddstr() blew it");

	ptr++;
    }

}

void
MainWindow::bottomRightWinContent() throw(YAPETUI::UIException) {
    if (key == NULL || recordlist == NULL) return;

    int retval = mymvwaddstr(bottomrightwin, 1, 2, "Cipher: Blowfish");
    if (retval == ERR)
	throw YAPETUI::UIException ("waddstr() blew it");
    retval = mvwprintw(bottomrightwin, 2, 2, "Key: %d bytes (%d bits)", key->size(), key->size()*8);
    if (retval == ERR)
	throw YAPETUI::UIException ("mvprintw() blew it");
    retval = mvwprintw(bottomrightwin, 3, 2, "%d entries ", recordlist->size());
    if (retval == ERR)
	throw YAPETUI::UIException ("mvprintw() blew it");

}

void
MainWindow::createWindow() throw(YAPETUI::UIException) {
    if (toprightwin != NULL || bottomrightwin != NULL)
	throw YAPETUI::UIException("May you consider deleting the window before reallocating");
    int middleX = maxX() / 2;
    int thirdY = maxY() / 3 - 1;

    printTitle();

    //
    // Top right window
    //
    toprightwin = newwin (maxY() - thirdY - 1 , maxX() - middleX, 1, middleX);
    if (toprightwin == NULL)
	throw YAPETUI::UIException ("newwin() returned NULL");

    YAPETUI::Colors::setcolor(toprightwin, YAPETUI::DEFAULT);
    int retval = box (toprightwin, 0, 0);
    if (retval == ERR)
	throw YAPETUI::UIException ("box() blew it");

    topRightWinContent();

    //
    // Bottom right window
    //
    bottomrightwin = newwin (thirdY - 1 , maxX() - middleX, maxY()-thirdY, middleX);
    if (bottomrightwin == NULL)
	throw YAPETUI::UIException ("newwin() returned NULL");

    YAPETUI::Colors::setcolor(bottomrightwin, YAPETUI::DEFAULT);
    retval = werase (bottomrightwin);
    if (retval == ERR)
	throw YAPETUI::UIException("werase() blew it");
    retval = box (bottomrightwin, 0, 0);
    if (retval == ERR)
	throw YAPETUI::UIException ("box() blew it");


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
	throw YAPETUI::UIException("delwin() blew it");

    retval = delwin(bottomrightwin);
    if (retval == ERR)
	throw YAPETUI::UIException("delwin() blew it");

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
	throw YAPETUI::UIException("Error refreshing stdscr");

    topRightWinContent();
    bottomRightWinContent();

    retval = box(toprightwin, 0, 0);
    if (retval == ERR)
	throw YAPETUI::UIException ("Error setting border");

    retval = box(bottomrightwin, 0, 0);
    if (retval == ERR)
	throw YAPETUI::UIException ("Error setting border");


    retval = wrefresh (toprightwin);
    if (retval == ERR)
	throw YAPETUI::UIException ("Error refreshing top right window");

    retval = wrefresh (bottomrightwin);
    if (retval == ERR)
	throw YAPETUI::UIException ("Error refreshing bottom right window");

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

	statusbar.putMsg("Error while asking for password");
	return;
    }

    if (key == NULL) {
	statusbar.putMsg("Creation of file canceled");
	return;
    }

    try {
	file = new YAPET::File(filename, *key, true);
	statusbar.putMsg(filename + " created");
	records_changed = false;
    } catch(YAPET::YAPETException& ex) {
	YAPETUI::MessageBox* msgbox = NULL;
	try{
	    msgbox = new YAPETUI::MessageBox("E R R O R", ex.what());
	    msgbox->run();
	    delete msgbox;
	} catch (YAPETUI::UIException&) {
	    if (msgbox != NULL)
		delete msgbox;

	    statusbar.putMsg("Error showing error message");
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
	    new YAPETUI::DialogBox("Question",
				   "The file does not exist. Do you want to create it?");
	question->run();
	YAPETUI::ANSWER a = question->getAnswer();
	delete question;
	if ( a == YAPETUI::ANSWER_OK) {
	    createFile(filename);
	    return;
	} else {
	    statusbar.putMsg("File creation canceled");
	    return;
	}
    } else if (retval == -1) {
	// Inform user about other error
	YAPETUI::MessageBox* errmsg = NULL;
	try {
	    errmsg = new YAPETUI::MessageBox("Error", strerror(errno));
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
	    errmsg = new YAPETUI::MessageBox("Error",
					     "The specified file is not a regular file");
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
	statusbar.putMsg("UI error while asking for password");
    }

    // Open file
    if (key != NULL) {
	try {
	    file = new YAPET::File(filename, *key, false);
	    std::list<YAPET::PartDec> tmp_list = file->read(*key);
	    recordlist->setList(tmp_list);
	    statusbar.putMsg(filename + " opened");
	    return;
	} catch(YAPET::YAPETException& e) {
	    if (file != NULL)
		delete file;

	    YAPETUI::MessageBox* msgbox = NULL;
	    try {
		msgbox = new YAPETUI::MessageBox("Error", e.what());
		msgbox->run();
		delete msgbox;
	    } catch (YAPETUI::UIException&) {
		if (msgbox != NULL)
		    delete msgbox;
		statusbar.putMsg("Error while trying to show error");
	    }
	    delete key;
	    key = NULL;
	    file = NULL;
	    statusbar.putMsg("Error opening file");
	    return;
	}
    } else {
	statusbar.putMsg("Opening of " + filename + " canceled");
    }
}

void
MainWindow::saveFile() {
    if (key == NULL || file == NULL) return;
    try {
	file->save(recordlist->getList());
	records_changed = false;
	statusbar.putMsg(file->getFilename() + " saved");
    } catch (YAPET::YAPETException& ex) {
	YAPETUI::MessageBox* msgbox = NULL;
	try {
	    msgbox = new YAPETUI::MessageBox("Error", ex.what());
	    msgbox->run();
	    delete msgbox;
	} catch (YAPETUI::UIException) {
	    if (msgbox != NULL)
		delete msgbox;
	    statusbar.putMsg("Error showing error message");
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
	    delete pwentry->getEncEntry();
	    records_changed = true;
	} else {
	    statusbar.putMsg("Record addition canceled");
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
	    msgbox = new YAPETUI::MessageBox("Error", "Error adding password entry");
	    msgbox->run();
	    delete msgbox;
	} catch (YAPETUI::UIException&) {
	    if (msgbox != NULL)
		delete msgbox;

	    statusbar.putMsg("Error showing error message");
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
	    records_changed = true;
	    delete pwentry->getEncEntry();
	} else {
	    statusbar.putMsg("Record edition canceled");
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
	    msgbox = new YAPETUI::MessageBox("Error", "Error adding password entry");
	    msgbox->run();
	    delete msgbox;
	} catch (YAPETUI::UIException&) {
	    if (msgbox != NULL)
		delete msgbox;

	    statusbar.putMsg("Error showing error message");
	}
    }
    ::refresh();
    refresh();
}

void
MainWindow::deleteSelectedRecord() throw(YAPETUI::UIException){
    YAPETUI::DialogBox* dialog = NULL;
    try {
	dialog = new YAPETUI::DialogBox("Question", "Delete selected record?");
	dialog->run();
	YAPETUI::ANSWER a = dialog->getAnswer();
	if (a == YAPETUI::ANSWER_OK) {
	    recordlist->deleteSelectedItem();
	    records_changed = true;
	    recordlist->refresh();
	    records_changed = true;
	}
	delete dialog;
    } catch(YAPETUI::UIException&) {
	if (dialog != NULL)
	    delete dialog;

	YAPETUI::MessageBox* msgbox = NULL;
	try {
	    msgbox = new YAPETUI::MessageBox("Error", "Error showing dialog");
	    msgbox->run();
	    delete msgbox;
	} catch (YAPETUI::UIException&) {
	    if (msgbox != NULL)
		delete msgbox;

	    statusbar.putMsg("Error showing error message");
	}
    }
    refresh();
}

bool
MainWindow::quit() {
    if (!records_changed) return true;

    YAPETUI::DialogBox* dialogbox = NULL;
    try {
	dialogbox = new YAPETUI::DialogBox("Question", "Save before quitting?");
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
	statusbar.putMsg("Error showing error message");
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
	    throw YAPETUI::UIException("Error creating lock window");

	int retval = werase(lockwin);
	if (retval == ERR) {
	    delwin(lockwin);
	    throw YAPETUI::UIException("Error erasing window");
	}


	retval = wrefresh(lockwin);
	if (retval == ERR) {
	    delwin(lockwin);
	    throw YAPETUI::UIException("Error refreshing window");
	}

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
		msgbox = new YAPETUI::MessageBox("Error", "Wrong password");
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

MainWindow::MainWindow() throw (YAPETUI::UIException) : BaseWindow(),
							toprightwin (NULL),
							bottomrightwin (NULL),
							recordlist (NULL),
							statusbar(),
							records_changed(false),
							key (NULL),
							file (NULL) {
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
	statusbar.putMsg ("No file loaded");

    if (file != NULL && key != NULL)
	statusbar.putMsg(file->getFilename() + " loaded");

    refresh();

    Alarm alrm(*this);
    int ch;
    while(true) {
	try {
#if defined(HAVE_SIGACTION) && defined(HAVE_SIGNAL_H)
	    BaseWindow::setTimeout(&alrm,600);
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
			tmp = new FileOpen("O P E N  F I L E");
			tmp->run();
			if (!tmp->isCanceled()) {
			    openFile(tmp->getFilepath());
			}
		    } catch (std::exception& ex2) {
			statusbar.putMsg(ex2.what());
			if (file != NULL)
			    delete file;
			if (key != NULL)
			    delete key;
			file = NULL;
			key = NULL;
		    }
		    delete tmp;
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
