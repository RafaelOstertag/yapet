// -*- c++ -*-
//
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

#ifndef _PASSWORDRECORD_H
#define _PASSWORDRECORD_H

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
#include <partdec.h>

#include <basewindow.h>
#include <button.h>
#include <passwordwidget.h>

/**
 * @brief A window that displays all the information associated with a
 * decrypted password record.
 *
 * A window that displays all the information associated with a decrypted
 * password record. The window allows edition of the informations. If the
 * information are edited, a call to \c entryChanged() yields \c true if the
 * record has been edited.
 *
 * To display an existing record, provide a valid pointer to \c PartDec object
 * when constructing the object. If the record has been changed, \c
 * getEncEntry() will return the pointer to the \c PartDec object holding the
 * altered record.
 *
 * To display a window for creating a new password record, pass \c NULL to the
 * \c PartDec pointer argument when constructing. The new record can be
 * obtained by calling \c getEncEntry().
 *
 * In any case, the memory occupied by the pointer returned by \c getEncEntry()
 * has to be freed by the caller. The class does not take care of this.
 */
class PasswordRecord : protected YAPETUI::BaseWindow {
    private:
	enum {
	    HEIGHT = 14
	};

	WINDOW* window;
	YAPETUI::InputWidget* name;
	YAPETUI::InputWidget* host;
	YAPETUI::InputWidget* username;
	YAPETUI::InputWidget* password;
	YAPETUI::InputWidget* comment;
	YAPETUI::Button* okbutton;
	YAPETUI::Button* cancelbutton;
	YAPET::Key* key;
	YAPET::PartDec* encentry;

	inline PasswordRecord(const PasswordRecord&) {}
	inline const PasswordRecord& operator=(const PasswordRecord&) { return *this; }

	inline int getWidth() const {
	    return maxX() - 8;
	}

	inline int getHeight() const {
	    return HEIGHT;
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
	 * Depending on the value passed in \c pe, either an empty record is
	 * showed or the decrypted password record including the password
	 * stored in the record in plain text is showed.
	 *
	 * @param k the key used to decrypt/encrypt the password record.
	 *
	 * @param pe pointer to a \c PartDec which will be displayed, or \c
	 * NULL in order to obtain a new password record.
	 */
	PasswordRecord(YAPET::Key& k, YAPET::PartDec* pe) throw(YAPETUI::UIException);
	~PasswordRecord();

	/**
	 * @brief Shows the dialog and handles user input.
	 *
	 * Shows the dialog and handles user input.
	 *
	 * Call \c getEncEntry() for obtaining the encrypted password record.
	 */
	void run() throw(YAPETUI::UIException);
	void resize() throw(YAPETUI::UIException);
	void refresh() throw(YAPETUI::UIException);
	/**
	 * @brief Returns the password record.
	 *
	 * Returns the new or altered password record as \c PartDec object. The
	 * caller is responsible for freeing the memory associated with the pointer returned.
	 *
	 * It returns \c NULL if the dialog has been canceled.
	 *
	 * @return pointer to the new or altered password record, or \c NULL if
	 * the dialog has been canceled. The caller is responsible for freeing
	 * the memory associated with the pointer returned.
	 */
	inline YAPET::PartDec* getEncEntry() const { return encentry; }

	/**
	 * @brief Indicates whether or not the record has been changed.
	 *
	 * Indicates whether or not the record has been changed.
	 *
	 * @return \c true if the record has been changed, \c false otherwise.
	 */
	inline bool entryChanged() const {
	    return name->isTextChanged() ||
		host->isTextChanged() ||
		username->isTextChanged() ||
		password->isTextChanged() ||
		comment->isTextChanged();
	}

};

#endif // _PASSWORDRECORD_H
