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

#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>

#ifndef HAVE_PATHCONF
#if HAVE_LIMITS_H
# include <limits.h>
#elif HAVE_SYS_PARAM_H
# include <sys/param.h>
#endif
#ifndef PATH_MAX
# if defined(_POSIX_PATH_MAX)
#  define PATH_MAX _POSIX_PATH_MAX
# elif defined(MAXPATHLEN)
#  define PATH_MAX MAXPATHLEN
# else
#  define PATH_MAX 255/* the Posix minimum path-size */
# endif
#endif
#endif // HAVE_PATHCONF

#ifdef HAVE_ALGORITHM
# include <algorithm>
#endif

#include <messagebox.h>
#include <colors.h>
#include "fileopen.h"

inline 
bool endswith(YAPETUI::secstring h, YAPETUI::secstring n) {
    if (n.length() > h.length())
	return false;

    if ( h.substr(h.length()-n.length(),n.length()) == n)
	return true;

    return false;
}

inline
bool endswith(std::string h, std::string n) {
    if (n.length() > h.length())
	return false;

    if ( h.substr(h.length()-n.length(),n.length()) == n)
	return true;

    return false;
}
    


void
FileOpen::createWindows() throw (YAPETUI::UIException) {
    if (window != NULL) return;

    window = newwin (windowHeight(), windowWidth(), startY(), startX());
    if (window == NULL)
        throw YAPETUI::UIException ("Error creating file open window");

    std::list<YAPETUI::secstring> dir_list;
    std::list<YAPETUI::secstring> file_list;
    getEntries(dir_list, file_list);
    dir = new YAPETUI::ListWidget<YAPETUI::secstring>(dir_list,
						  startX()+1,
						  startY()+2,
						  windowWidth()/2-1,
						  windowHeight()-6);

    files = new YAPETUI::ListWidget<YAPETUI::secstring>(file_list,
						    startX() + windowWidth()/2 ,
						    startY() + 2,
						    windowWidth()/2-1,
						    windowHeight()-6);

    input = new YAPETUI::InputWidget(startX()+1,
				   startY()+windowHeight()-3,
				   windowWidth()-2);

    okbutton = new YAPETUI::Button("OK",
				 startX()+1,
				 startY()+windowHeight()-2);

    cancelbutton = new YAPETUI::Button("Cancel",
				     startX()+8,
				     startY()+windowHeight()-2);

}

void
FileOpen::getEntries(std::list<YAPETUI::secstring>& d, 
		     std::list<YAPETUI::secstring>& f)
    throw(YAPETUI::UIException) {

    DIR* dir_ptr = opendir(directory.c_str());
    if (dir_ptr == NULL)
	throw YAPETUI::UIException(strerror(errno));

    struct dirent* de;
    struct stat st;

    while ( (de = readdir(dir_ptr)) != NULL) {
	int retval = stat( YAPETUI::secstring(directory +"/"+de->d_name).c_str(), &st);
	if (retval != 0) {
	    continue;
	}

	if (S_ISDIR(st.st_mode)) {
	    d.push_back(de->d_name);
	} else if (S_ISREG(st.st_mode)) {
	    std::string tmp(de->d_name);
	    if (endswith(tmp, ".pet") )
		f.push_back(de->d_name);
	}
    }
    closedir(dir_ptr);

    d.sort();
    f.sort();
}

void
FileOpen::printTitle() throw(YAPETUI::UIException) {
    int retval = mymvwaddstr(window, 0, 2, title.c_str());
    if (retval == ERR)
	throw YAPETUI::UIException("Error printing title");
}

void
FileOpen::printCWD() throw(YAPETUI::UIException) {
    char format[50];
    snprintf(format, 50, "%%-%ds", (windowWidth()-2));

    int retval = mvwprintw(window, 1, 1, format, " ");
    if (retval == ERR)
	throw YAPETUI::UIException("Error touching line");

    retval = mymvwaddstr(window, 1, 1, directory.c_str());
    if (retval == ERR)
	throw YAPETUI::UIException("Error printing cwd");
}

void
FileOpen::getcwd() throw (YAPETUI::UIException) {
    long size = 0;
#ifdef HAVE_PATHCONF
	size = pathconf (".", _PC_PATH_MAX);
    size = size < 1 ? FALLBACK_PATH_MAX : size;
#else
	size = MAX_PATH;
#endif
    char* buf = (char *) malloc ( (size_t) size);
    if (buf == NULL)
        throw YAPETUI::UIException ("Error allocating memory");

    char* ptr = ::getcwd (buf, (size_t) size);
    if (ptr == NULL) {
        free (buf);
        throw YAPETUI::UIException (strerror (errno));
    }

    directory = ptr;
    free (ptr);
}

void
FileOpen::cd (const YAPETUI::secstring d) throw (YAPETUI::UIException) {
    int retval = chdir (d.c_str());
    if (retval != 0)
        throw YAPETUI::UIException (strerror (errno));

    getcwd();
}

FileOpen::FileOpen(std::string t) throw(YAPETUI::UIException) : Resizeable(),
						       title(t),
						       window (NULL),
						       dir (NULL),
						       files (NULL),
						       input(NULL),
						       okbutton(NULL),
						       cancelbutton(NULL),
						       canceled(true) {
    getcwd();
    createWindows();
    
}

FileOpen::~FileOpen() {
	wclear(window);
	delwin (window);
	delete dir;
	delete files;
	delete input;
	delete okbutton;
	delete cancelbutton;
}

void
FileOpen::run() throw (YAPETUI::UIException) {
    std::list<YAPETUI::secstring> file_list;
    std::list<YAPETUI::secstring> dir_list;

    refresh();

    int ch;
    // The big loop
    while(true){
	
	// The event handler for the directory list
	while ( (ch= dir->focus()) != '\t') {
	    switch (ch) {
#ifdef HAVE_WRESIZE
	    case KEY_RESIZE:
		YAPETUI::Resizeable::resizeAll();
		break;
#endif // HAVE_WRESIZE
	    case KEY_ENTER:
	    case '\n': {
		try {
		    file_list.clear();
		    dir_list.clear();
		    cd(dir->getSelectedItem());
		    getEntries(dir_list, file_list);
		    files->setList(file_list);
		    dir->setList(dir_list);
		} catch (YAPETUI::UIException& ex) {
		    YAPETUI::MessageBox* tmp = 
			new YAPETUI::MessageBox("Error", ex.what());
		    tmp->run();
		    delete tmp;
		}
	    }
		break;	    
	    }
	}

	// The event handler for the file list
	while ( (ch = files->focus()) != '\t') {
	    switch (ch) {
#ifdef HAVE_WRESIZE
	    case KEY_RESIZE:
		YAPETUI::Resizeable::resizeAll();
		break;
#endif // HAVE_WRESIZE
	    case KEY_ENTER:
	    case '\n':
		filename = files->getSelectedItem();
		input->setText(filename);
		break;
	    }
	}

#ifdef HAVE_WRESIZE
	while ( (ch = input->focus()) == KEY_RESIZE)
	    YAPETUI::Resizeable::resizeAll();
#else // HAVE_WRESIZE
	ch = input->focus();
#endif // HAVE_WRESIZE
	filename = input->getText();


	// The ok button
#ifdef HAVE_WRESIZE
	while ( (ch = okbutton->focus()) == KEY_RESIZE)
	    YAPETUI::Resizeable::resizeAll();
#else // HAVE_WRESIZE
	ch = okbutton->focus();
#endif // HAVE_WRESIZE
	if (ch == '\n' || ch == KEY_ENTER) {
	    canceled = false;
	    return;
	}

	// The cancel button
#ifdef HAVE_WRESIZE
	while ( (ch = cancelbutton->focus()) == KEY_RESIZE)
	    YAPETUI::Resizeable::resizeAll();
#else // HAVE_WRESIZE
	ch = cancelbutton->focus();
#endif // HAVE_WRESIZE
	if (ch == '\n' || ch == KEY_ENTER) {
	    canceled = true;
	    return;
	}
    }
}

void
FileOpen::refresh() throw (YAPETUI::UIException) {
    YAPETUI::Colors::setcolor(window, YAPETUI::MESSAGEBOX);



    int retval = box(window, 0, 0);
    if (retval == ERR)
	throw YAPETUI::UIException("Error drawing box");

    printTitle();
    printCWD();

    retval = wrefresh (window);
    if (retval == ERR)
        throw YAPETUI::UIException ("Error refreshing window");

    dir->refresh();
    files->refresh();
    input->refresh();
    okbutton->refresh();
    cancelbutton->refresh();
}

void
FileOpen::resize() throw (YAPETUI::UIException) {
    delete dir;
    delete files;
    delete input;
    delete okbutton;
    delete cancelbutton;
    
    int retval = delwin(window);
    if (retval == ERR)
	throw YAPETUI::UIException("Error deleting window");

    window = NULL;
    dir = NULL;
    files = NULL;
    input = NULL;
    okbutton = NULL;
    cancelbutton = NULL;

    createWindows();
}

std::string
FileOpen::getFilepath() {
    if (!endswith(filename, ".pet"))
	filename+=".pet";

    std::string tmp_filename(filename.c_str());
    std::string tmp_directory(directory.c_str());
    if (tmp_directory == "/")
	return tmp_directory + tmp_filename;

    return tmp_directory + "/" + tmp_filename;
}
