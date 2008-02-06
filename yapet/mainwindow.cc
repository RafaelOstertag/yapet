// $Id$

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

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

void
MainWindow::printTitle() throw(GPSUI::UIException) {
    GPSUI::Colors::setcolor(stdscr, GPSUI::DEFAULT);
    // Title
    char title[100];
    snprintf(title,100, "..::|| %s ||::..", PACKAGE_STRING);
    int retval = wmove(stdscr, 0, maxX()/2 - strlen(title)/2);
    if (retval == ERR)
	throw GPSUI::UIException("Error moving cursor");
    retval = mywaddstr(stdscr, title);
    if (retval == ERR)
	throw GPSUI::UIException("Error printing title");
}

void
MainWindow::topRightWinContent() throw (GPSUI::UIException) {
    int max_y, max_x;
    getmaxyx (toprightwin, max_y, max_x);

    char win_title[] = "K E Y S";
    int start_title_x = max_x / 2 - strlen (win_title) / 2;

    int retval = mymvwaddstr (toprightwin, 1, start_title_x, win_title);
    if (retval == ERR)
        throw GPSUI::UIException ("mvwaddstr() blew it");
    retval = wmove (toprightwin, 2, 1);
    if (retval == ERR)
        throw GPSUI::UIException ("wmove() blew it");

    retval = whline (toprightwin, 0, max_x - 2);
    if (retval == ERR)
        throw GPSUI::UIException ("whline() blew it");


    KeyDesc* ptr = keys;
    while (ptr->key != NULL
            && ptr->desc != NULL) {

        wattron (toprightwin, A_REVERSE);
        retval = mvwprintw (toprightwin, ptr->y, ptr->x, "  %0-2s  ", ptr->key);
        if (retval == ERR)
            throw GPSUI::UIException ("mvprintw() blew it");
        wattroff (toprightwin, A_REVERSE);
        retval = mymvwaddstr (toprightwin, ptr->y, ptr->x + 8, ptr->desc);
        if (retval == ERR)
            throw GPSUI::UIException ("waddstr() blew it");

        ptr++;
    }

}

void
MainWindow::bottomRightWinContent() throw(GPSUI::UIException) {
    if (key == NULL || recordlist == NULL) return;
    int retval = mymvwaddstr(bottomrightwin, 1, 1, "Cipher: Blowfish");
    if (retval == ERR)
	throw GPSUI::UIException ("waddstr() blew it");
    retval = mvwprintw(bottomrightwin, 2, 1, "Key: %d bytes (%d bits)", key->size(), key->size()*8);
    if (retval == ERR)
	throw GPSUI::UIException ("mvprintw() blew it");
    retval = mvwprintw(bottomrightwin, 3, 1, "%d entries", recordlist->size());
    if (retval == ERR)
	throw GPSUI::UIException ("mvprintw() blew it");
}

void
MainWindow::createWindow() throw(GPSUI::UIException) {
    if (toprightwin != NULL || bottomrightwin != NULL)
	throw GPSUI::UIException("May you consider deleting the window before reallocating");
    int middleX = maxX() / 2;
    int thirdY = maxY() / 3 - 1;

    printTitle();

    //
    // Top right window
    //
    toprightwin = newwin (maxY() - thirdY - 1 , maxX() - middleX, 1, middleX);
    if (toprightwin == NULL)
        throw GPSUI::UIException ("newwin() returned NULL");

    GPSUI::Colors::setcolor(toprightwin, GPSUI::DEFAULT);
    int retval = box (toprightwin, 0, 0);
    if (retval == ERR)
        throw GPSUI::UIException ("box() blew it");

    topRightWinContent();

    //
    // Bottom right window
    //
    bottomrightwin = newwin (thirdY - 1 , maxX() - middleX, maxY()-thirdY, middleX);
    if (bottomrightwin == NULL)
        throw GPSUI::UIException ("newwin() returned NULL");

    GPSUI::Colors::setcolor(bottomrightwin, GPSUI::DEFAULT);
    retval = werase (bottomrightwin);
    if (retval == ERR)
	throw GPSUI::UIException("werase() blew it");
    retval = box (bottomrightwin, 0, 0);
    if (retval == ERR)
        throw GPSUI::UIException ("box() blew it");


    //
    // List widget on the left
    //
    if (recordlist == NULL) {
	std::list<GPSAFE::PartDec> emptylist;
	recordlist = new GPSUI::ListWidget<GPSAFE::PartDec> (emptylist,
							     0,
							     1,
							     maxX() / 2,
							     maxY() - 2);
    }

    bottomRightWinContent();
}

void
MainWindow::resize() throw (GPSUI::UIException) {
    int retval = delwin(toprightwin);
    if (retval == ERR)
	throw GPSUI::UIException("delwin() blew it");

    retval = delwin(bottomrightwin);
    if (retval == ERR)
	throw GPSUI::UIException("delwin() blew it");

    toprightwin = NULL;
    bottomrightwin = NULL;


    createWindow();

    recordlist->resize (0, 1, maxX()/2, maxY() - 2);
}

void
MainWindow::refresh() throw (GPSUI::UIException) {
    printTitle();

    int retval = wrefresh(stdscr);
    if (retval == ERR)
	throw GPSUI::UIException("Error refreshing stdscr");

    topRightWinContent();
    bottomRightWinContent();

    retval = box(toprightwin, 0, 0);
    if (retval == ERR)
        throw GPSUI::UIException ("Error setting border");

    retval = box(bottomrightwin, 0, 0);
    if (retval == ERR)
        throw GPSUI::UIException ("Error setting border");


    retval = wrefresh (toprightwin);
    if (retval == ERR)
        throw GPSUI::UIException ("Error refreshing top right window");

    retval = wrefresh (bottomrightwin);
    if (retval == ERR)
        throw GPSUI::UIException ("Error refreshing bottom right window");

    recordlist->refresh();
    statusbar.refresh();
}

void
MainWindow::createFile(std::string& filename) throw(GPSUI::UIException) {
    closeFile();

    PasswordDialog* pwdia = NULL;
    try {
	pwdia = new PasswordDialog(NEW_PW, filename);
	pwdia->run();
	key = pwdia->getKey();
	delete pwdia;
    } catch(GPSUI::UIException&) {
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
	file = new GPSAFE::File(filename, *key, true);
	records_changed = false;
    } catch(GPSAFE::GPSException& ex) {
	GPSUI::MessageBox* msgbox = NULL;
	try{
	    msgbox = new GPSUI::MessageBox("E R R O R", ex.what());
	    msgbox->run();
	    delete msgbox;
	} catch (GPSUI::UIException&) {
	    if (msgbox != NULL)
		delete msgbox;

	    statusbar.putMsg("Error showing error message");
	}
	closeFile();
    }
} 

void
MainWindow::openFile(std::string filename) throw(GPSUI::UIException) {
    struct stat st;
    int retval = stat(filename.c_str(), &st);
    if (retval == -1 && errno == ENOENT) {
	// Ask user whether or not he wants to create a new file
	GPSUI::DialogBox* question = new GPSUI::DialogBox("Question",
							  "The file does not exist. Do you want to create it?");
	question->run();
	GPSUI::ANSWER a = question->getAnswer();
	delete question;
	if ( a == GPSUI::ANSWER_OK) {
	    createFile(filename);
	    return;
	} else {
	    statusbar.putMsg("File creation canceled");
	    return;
	}
    } else if (retval == -1) {
	// Inform user about other error
	GPSUI::MessageBox* errmsg = NULL;
	try {
	    errmsg = new GPSUI::MessageBox("Error", strerror(errno));
	    errmsg->run();
	    delete errmsg;
	} catch (GPSUI::UIException&) {
	    if (errmsg != NULL)
		delete errmsg;
	}
	refresh();
	return;
    }

    // We were able to stat the file, no make sure it is a file and open it
    // using the password
    if (!S_ISREG(st.st_mode)) {
	GPSUI::MessageBox* errmsg = NULL;
	try {
	    errmsg = new GPSUI::MessageBox("Error", "The specified file is not a regular file");
	    errmsg->run();
	    delete errmsg;
	} catch (GPSUI::UIException&) {
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
    } catch (GPSUI::UIException&) {
	if (pwdia != NULL)
	    delete pwdia;
	statusbar.putMsg("UI error while asking for password");
    }

    // Open file
    if (key != NULL) {
	try {
	    file = new GPSAFE::File(filename, *key, false);
	    std::list<GPSAFE::PartDec> tmp_list = file->read(*key);
	    recordlist->setList(tmp_list);
	    return;
	} catch(GPSAFE::GPSException& e) {
	    if (file != NULL)
		delete file;

	    GPSUI::MessageBox* msgbox = NULL;
	    try {
		msgbox = new GPSUI::MessageBox("Error", e.what());
		msgbox->run();
		delete msgbox;
	    } catch (GPSUI::UIException&) {
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
    } catch (GPSAFE::GPSException& ex) {
	GPSUI::MessageBox* msgbox = NULL;
	try {
	    msgbox = new GPSUI::MessageBox("Error", ex.what());
	    msgbox->run();
	    delete msgbox;
	} catch (GPSUI::UIException) {
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
    } catch (GPSUI::UIException& ex) {
	if (pwentry != NULL) {
	    if (pwentry->getEncEntry() != NULL)
		delete pwentry->getEncEntry();
	    delete pwentry;
	}
	
	GPSUI::MessageBox* msgbox = NULL;
	try {
	    msgbox = new GPSUI::MessageBox("Error", "Error adding password entry");
	    msgbox->run();
	    delete msgbox;
	} catch (GPSUI::UIException&) {
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
    if (key == NULL || file == NULL) return;
    PasswordRecord* pwentry = NULL;
    try {
	GPSAFE::PartDec pd = recordlist->getSelectedItem();
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
    } catch (GPSUI::UIException& ex) {
	if (pwentry != NULL) {
	    if (pwentry->getEncEntry() != NULL)
		delete pwentry->getEncEntry();
	    delete pwentry;
	}
	
	GPSUI::MessageBox* msgbox = NULL;
	try {
	    msgbox = new GPSUI::MessageBox("Error", "Error adding password entry");
	    msgbox->run();
	    delete msgbox;
	} catch (GPSUI::UIException&) {
	    if (msgbox != NULL)
		delete msgbox;
	    
	    statusbar.putMsg("Error showing error message");
	}
    }
    ::refresh();
    refresh();
}

void
MainWindow::deleteSelectedRecord() throw(GPSUI::UIException){
    GPSUI::DialogBox* dialog = NULL;
    try {
	dialog = new GPSUI::DialogBox("Question", "Delete selected record?");
	dialog->run();
	GPSUI::ANSWER a = dialog->getAnswer();
	if (a == GPSUI::ANSWER_OK) {
	    recordlist->deleteSelectedItem();
	    records_changed = true;
	    recordlist->refresh();
	    records_changed = true;
	}
	delete dialog;
    } catch(GPSUI::UIException&) {
	if (dialog != NULL)
	    delete dialog;

	GPSUI::MessageBox* msgbox = NULL;
	try {
	    msgbox = new GPSUI::MessageBox("Error", "Error showing dialog");
	    msgbox->run();
	    delete msgbox;
	} catch (GPSUI::UIException&) {
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

    GPSUI::DialogBox* dialogbox = NULL;
    try {
	dialogbox = new GPSUI::DialogBox("Question", "Save before quitting?");
	dialogbox->run();
	GPSUI::ANSWER a = dialogbox->getAnswer();
	delete dialogbox;
	if (a == GPSUI::ANSWER_OK) {
	    saveFile();
	    return true;
	}

	return true;
    } catch (GPSUI::UIException&) {
	if (dialogbox != NULL)
	    delete dialogbox;
	statusbar.putMsg("Error showing error message");
	refresh();
	return false;
    }
}

void
MainWindow::lockScreen() const throw(GPSUI::UIException){
    if (key == NULL) return;
    int ch;
    while (true) {
	WINDOW* lockwin = newwin(0,0,0,0);
	if (lockwin == NULL)
	    throw GPSUI::UIException("Error creating lock window");
	
	int retval = werase(lockwin);
	if (retval == ERR) {
	    delwin(lockwin);
	    throw GPSUI::UIException("Error erasing window");
	}
	
	
	retval = wrefresh(lockwin);
	if (retval == ERR) {
	    delwin(lockwin);
	    throw GPSUI::UIException("Error refreshing window");
	}
	
	ch = wgetch(lockwin);
#ifdef HAVE_WRESIZE
	if (ch == KEY_RESIZE) {
	    delwin(lockwin);
	    GPSUI::Resizeable::resizeAll();
	    continue;
	}
#endif
	PasswordDialog* pwdia = NULL;
	GPSAFE::Key* testkey = NULL;
	try {
	    pwdia = new PasswordDialog(EXISTING_PW, file->getFilename());
	    pwdia->run();
	    testkey = pwdia->getKey();
	    delete pwdia;
	} catch(GPSUI::UIException&) {
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
	    GPSUI::MessageBox* msgbox = NULL;
	    try {
		msgbox = new GPSUI::MessageBox("Error", "Wrong password");
		msgbox->run();
		delete msgbox;
	    } catch (GPSUI::UIException&) {
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

MainWindow::MainWindow() throw (GPSUI::UIException) : Resizeable(),
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
    wrefresh(toprightwin);
    wrefresh(bottomrightwin);
    delwin (toprightwin);
    delwin (bottomrightwin);
    if (key != NULL)
	delete key;
    if (file != NULL)
	delete file;
}

void
MainWindow::run() throw (GPSUI::UIException) {

    statusbar.putMsg ("No file loaded");
    refresh();

    int ch;
    while(true) {
	try {
	    while ( (ch=recordlist->focus()) ) {
		switch (ch) {
		case '\n':
		    editSelectedRecord();
		    break;
		case 'Q':
		case 'q': 
		    if (quit()) return;
		    break;
#ifdef HAVE_WRESIZE
		case KEY_RESIZE:
		    GPSUI::Resizeable::resizeAll();
		    break;
#endif // HAVE_WRESIZE
		case KEY_REFRESH:
#ifdef HAVE_WRESIZE
		    GPSUI::Resizeable::resizeAll();
#endif // HAVE_WRESIZE
		    GPSUI::Resizeable::refreshAll();
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
			    statusbar.putMsg(tmp->getFilepath());
			    openFile(tmp->getFilepath());
			}
		    } catch (std::exception& ex2) {
			statusbar.putMsg(ex2.what());
		    }
		    delete tmp;
		    ::refresh();
		    GPSUI::Resizeable::refreshAll();
		}
		    break;
		case 'L':
		case 'l':
		    lockScreen();
		    ::refresh();
		    GPSUI::Resizeable::refreshAll();
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
	    }
	} catch(std::exception& ex) {
	    statusbar.putMsg(ex.what());
	}
    }
}
