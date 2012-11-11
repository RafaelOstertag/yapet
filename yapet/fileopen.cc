// $Id$
//
// Copyright (C) 2008-2010  Rafael Ostertag
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
#ifdef HAVE_ERRNO_H
# include <errno.h>
#endif
#ifdef HAVE_STRING_H
# include <string.h>
#endif
#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif
#ifdef HAVE_DIRENT_H
# include <dirent.h>
#endif

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

#include "../intl.h"
#include <messagebox.h>
#include <colors.h>
#include "consts.h"
#include "fileopen.h"

void
FileOpen::createWindows() throw (YAPET::UI::UIException) {
    if (window != NULL)
        throw YAPET::UI::UIException (_ ("May you consider deleting the window before reallocating") );

    // return;
    window = newwin (windowHeight(), windowWidth(), startY(), startX() );

    if (window == NULL)
        throw YAPET::UI::UIException (_ ("Error creating file open window") );

    std::list<YAPET::UI::secstring> dir_list;
    std::list<YAPET::UI::secstring> file_list;
    getEntries (dir_list, file_list);
    dir = new YAPET::UI::ListWidget<YAPET::UI::secstring> (dir_list,
            startX() + 1,
            startY() + 2,
            windowWidth() / 2 - 1,
            windowHeight() - 6);
    files = new YAPET::UI::ListWidget<YAPET::UI::secstring> (file_list,
            startX() + windowWidth() / 2 ,
            startY() + 2,
            windowWidth() / 2 - 1,
            windowHeight() - 6);
    input = new YAPET::UI::InputWidget (startX() + 1,
                                        startY() + windowHeight() - 3,
                                        windowWidth() - 2);
    okbutton = new YAPET::UI::Button (_ ("OK"),
                                      startX() + 1,
                                      startY() + windowHeight() - 2);
    cancelbutton = new YAPET::UI::Button (_ ("Cancel"),
                                          startX() + 8,
                                          startY() + windowHeight() - 2);
}

void
FileOpen::getEntries (std::list<YAPET::UI::secstring>& d,
                      std::list<YAPET::UI::secstring>& f)
throw (YAPET::UI::UIException) {
    DIR* dir_ptr = opendir (directory.c_str() );

    if (dir_ptr == NULL)
        throw YAPET::UI::UIException (strerror (errno) );

    struct dirent* de;
    struct stat st;

    while ( (de = readdir (dir_ptr) ) != NULL) {
        int retval = stat ( YAPET::UI::secstring (directory + "/" + de->d_name).c_str(), &st);

        if (retval != 0) {
            continue;
        }

        if (S_ISDIR (st.st_mode) ) {
            d.push_back (de->d_name);
        } else if (S_ISREG (st.st_mode) ) {
            std::string tmp (de->d_name);

            if (endswith (tmp, YAPET::CONSTS::Consts::getDefaultSuffix() ) )
                f.push_back (de->d_name);
        }
    }

    closedir (dir_ptr);
    d.sort();
    f.sort();
}

void
FileOpen::printTitle() throw (YAPET::UI::UIException) {
    int retval = mymvwaddstr (window, 0, 2, title.c_str() );

    if (retval == ERR)
        throw YAPET::UI::UIException (_ ("Error printing title") );
}

void
FileOpen::printCWD() throw (YAPET::UI::UIException) {
    char format[50];
    snprintf (format, 50, "%%-%ds", (windowWidth() - 2) );
    int retval = mvwprintw (window, 1, 1, format, " ");

    if (retval == ERR)
        throw YAPET::UI::UIException (_ ("Error clearing line") );

    retval = mymvwaddstr (window, 1, 1, directory.c_str() );

    if (retval == ERR)
        throw YAPET::UI::UIException (_ ("Error printing cwd") );

    retval = wrefresh (window);

    if (retval == ERR)
        throw YAPET::UI::UIException (_ ("Error refreshing cwd") );
}

void
FileOpen::getcwd() throw (YAPET::UI::UIException) {
    long size = 0;
#ifdef HAVE_PATHCONF
    size = pathconf (".", _PC_PATH_MAX);
    size = size < 1 ? FALLBACK_PATH_MAX : size;
#else
    size = MAX_PATH;
#endif
    char* buf = (char *) malloc ( (size_t) size);

    if (buf == NULL)
        throw YAPET::UI::UIException (_ ("Error allocating memory") );

    char* ptr = ::getcwd (buf, (size_t) size);

    if (ptr == NULL) {
        free (buf);
        throw YAPET::UI::UIException (strerror (errno) );
    }

    directory = ptr;
    free (ptr);
}

void
FileOpen::cd (const YAPET::UI::secstring d) throw (YAPET::UI::UIException) {
    int retval = chdir (d.c_str() );

    if (retval != 0)
        throw YAPET::UI::UIException (strerror (errno) );

    getcwd();
}

FileOpen::FileOpen (std::string t) throw (YAPET::UI::UIException) : BaseWindow(),
        title (t),
        window (NULL),
        dir (NULL),
        files (NULL),
        input (NULL),
        okbutton (NULL),
        cancelbutton (NULL),
        canceled (true) {
    getcwd();
    createWindows();
}

FileOpen::~FileOpen() {
#ifdef PARANOID
    wclear (window);
#endif
    delwin (window);
    delete dir;
    delete files;
    delete input;
    delete okbutton;
    delete cancelbutton;
}

void
FileOpen::run() throw (YAPET::UI::UIException) {
    std::list<YAPET::UI::secstring> file_list;
    std::list<YAPET::UI::secstring> dir_list;
    refresh();
    int ch;

    // The big loop
    while (true) {
        // The event handler for the directory list
        while ( (ch = dir->focus() ) != '\t') {
            switch (ch) {
#ifdef HAVE_WRESIZE
                case KEY_RESIZE:
                    YAPET::UI::BaseWindow::resizeAll();
                    break;
#endif // HAVE_WRESIZE
                case KEY_ESC:
                    canceled = true;
                    return;
                case KEY_ENTER:
                case '\n': {
                    try {
                        file_list.clear();
                        dir_list.clear();
                        cd (dir->getSelectedItem() );
                        getEntries (dir_list, file_list);
                        files->setList (file_list);
                        dir->setList (dir_list);
                        printCWD();
                    } catch (YAPET::UI::UIException& ex) {
                        YAPET::UI::MessageBox* tmp =
                            new YAPET::UI::MessageBox (_ ("E R R O R"), ex.what() );
                        tmp->run();
                        delete tmp;
                        this->refresh();
                    }
                }
                break;
            }
        }

        // The event handler for the file list
        while ( (ch = files->focus() ) != '\t') {
            switch (ch) {
#ifdef HAVE_WRESIZE
                case KEY_RESIZE:
                    YAPET::UI::BaseWindow::resizeAll();
                    break;
#endif // HAVE_WRESIZE
                case KEY_ESC:
                    canceled = true;
                    return;
                case KEY_ENTER:
                case '\n':
                    filename = files->getSelectedItem();
                    input->setText (filename);
                    break;
            }
        }

#ifdef HAVE_WRESIZE

        while ( (ch = input->focus() ) == KEY_RESIZE)
            YAPET::UI::BaseWindow::resizeAll();

#else // HAVE_WRESIZE
        ch = input->focus();
#endif // HAVE_WRESIZE

        if (ch == KEY_ESC) {
            canceled = true;
            return;
        }

        filename = input->getText();
        // The ok button
#ifdef HAVE_WRESIZE

        while ( (ch = okbutton->focus() ) == KEY_RESIZE)
            YAPET::UI::BaseWindow::resizeAll();

#else // HAVE_WRESIZE
        ch = okbutton->focus();
#endif // HAVE_WRESIZE

        switch (ch) {
            case KEY_ESC:
                canceled = true;
                return;
            case '\n':
            case KEY_ENTER:
                canceled = false;
                return;
        }

        // The cancel button
#ifdef HAVE_WRESIZE

        while ( (ch = cancelbutton->focus() ) == KEY_RESIZE)
            YAPET::UI::BaseWindow::resizeAll();

#else // HAVE_WRESIZE
        ch = cancelbutton->focus();
#endif // HAVE_WRESIZE

        if (ch == '\n' || ch == KEY_ENTER || ch == KEY_ESC) {
            canceled = true;
            return;
        }
    }
}

void
FileOpen::refresh() throw (YAPET::UI::UIException) {
    YAPET::UI::Colors::setcolor (window, YAPET::UI::MESSAGEBOX);
    int retval = box (window, 0, 0);

    if (retval == ERR)
        throw YAPET::UI::UIException (_ ("Error drawing box") );

    printTitle();
    printCWD();
    retval = wrefresh (window);

    if (retval == ERR)
        throw YAPET::UI::UIException (_ ("Error refreshing window") );

    dir->refresh();
    files->refresh();
    input->refresh();
    okbutton->refresh();
    cancelbutton->refresh();
}

void
FileOpen::resize() throw (YAPET::UI::UIException) {
    delete dir;
    delete files;
    delete input;
    delete okbutton;
    delete cancelbutton;
    int retval = delwin (window);

    if (retval == ERR)
        throw YAPET::UI::UIException (_ ("Error deleting window") );

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
    if (!endswith (filename, YAPET::CONSTS::Consts::getDefaultSuffix().c_str() ) )
        filename += YAPET::CONSTS::Consts::getDefaultSuffix().c_str();

    std::string tmp_filename (filename.c_str() );
    std::string tmp_directory (directory.c_str() );

    if (tmp_directory == "/")
        return tmp_directory + tmp_filename;

    return tmp_directory + "/" + tmp_filename;
}
