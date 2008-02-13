// -*- c++ -*-
//
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

#ifndef _PASSWORDDIALOG_H
#define _PASSWORDDIALOG_H

#ifdef HAVE_CONFIG_H
# include <config.h>
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

#ifdef HAVE_STRING
# include <string>
#endif

#include <key.h>

#include <basewindow.h>
#include <button.h>
#include <passwordwidget.h>

/**
 * @brief Determines the type of the password dialog.
 *
 * Determines the type of the password dialog: Either a password dialog for a
 * new password or a dialog for an existing password.
 */
enum PWTYPE {
    /**
     * Makes the \c PasswordDialog show two input widget for the password. One
     * for the password and the other to confirm the password.
     */
    NEW_PW,
    /**
     * Makes the \c PasswordDialog show only one input widget for the password.
     */
    EXISTING_PW
};

/**
 * @brief Shows a dialog for entering the password.
 *
 * Depending on the \c PWTYPE, it shows either one or two password input
 * widgets. If \c PWTYPE is \c NEW_PW, it shows two input widgets, one for the
 * password and the other to confirm the password. If the passwords matches and
 * the user doesn't cancel the dialog, a \c Key is generated and put on the
 * heap. The pointer to the key can be obtained by calling \c getKey(). The
 * memory occupied by this key is NOT freed by this class.
 *
 * If \c PWTYPE is \c EXISTING_PW, only one widget for entering the password is
 * displayed. The same rules as for \c NEW_PW apply in regard to the key.
 */
class PasswordDialog : protected YAPETUI::BaseWindow {
    private:
	enum {
	    HEIGHT_NEW = 9,
	    HEIGHT_EX = 7
	};

	WINDOW* window;
	YAPETUI::PasswordWidget* pwidget1;
	YAPETUI::PasswordWidget* pwidget2;
	YAPETUI::Button* okbutton;
	YAPETUI::Button* cancelbutton;
	PWTYPE pwtype;
	YAPET::Key* key;

	std::string filename;

	inline PasswordDialog(const PasswordDialog&) {}
	inline const PasswordDialog& operator=(const PasswordDialog&) { return *this; }

	inline int getWidth() const {
	    return maxX() - 8;
	}

	inline int getHeight() const {
	    if (pwtype == NEW_PW)
		return HEIGHT_NEW;
	    else
		return HEIGHT_EX;
	}

	inline int getStartX() const {
	    return maxX()/2 - getWidth()/2;
	}

	inline int getStartY() const {
	    return maxY()/2 - getHeight()/2;
	}

	void createWindow() throw(YAPETUI::UIException);

    public:
	/**
	 * @brief Constructor.
	 *
	 * Sets up the dialog, but does not show it. Use \c run() to display
	 * the dialog to the user.
	 *
	 * @param pt the type of the dialog. \c NEW_PW for asking for a new
	 * password with confirmation, or \c EXISTING_PW for asking for an
	 * existing password.
	 *
	 * @param fn the filename of the file for which the password is asked.
	 */
	PasswordDialog(PWTYPE pt, std::string fn) throw(YAPETUI::UIException);
	~PasswordDialog();

	/**
	 * @brief Displays the password dialog.
	 *
	 * Displays the password dialog. Use \c getKey() for obtaining the key
	 * generated from the password.
	 */
	void run() throw(YAPETUI::UIException);
	/**
	 * @brief Returns the key generated from the password.
	 *
	 * Use this function to get the key generated from the password
	 * entered. If the dialog has been canceled or the passwords do not
	 * match in case of entering a fresh password, \c NULL is returned.
	 *
	 * The caller is responsible for freeing the memory occupied by the
	 * key.
	 *
	 * @return the pointer to the key or \c NULL if no key was/could be
	 * generated. The memory occupied by the key has to be freed by the
	 * caller.
	 */
	YAPET::Key* getKey() const { return key; }
	void resize() throw(YAPETUI::UIException);
	void refresh() throw(YAPETUI::UIException);
};

#endif // _PASSWORDDIALOG_H
