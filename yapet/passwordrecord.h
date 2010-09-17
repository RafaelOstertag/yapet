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

#include "key.h"
#include "partdec.h"

#include "basewindow.h"
#include "button.h"
#include "passwordwidget.h"
#ifdef ENABLE_PWGEN
# include "pwgendialog.h"
#endif

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
class PasswordRecord : protected YAPET::UI::BaseWindow {
    private:
        enum {
            HEIGHT = 14
        };

        WINDOW* window;
        YAPET::UI::InputWidget* name;
        YAPET::UI::InputWidget* host;
        YAPET::UI::InputWidget* username;
        YAPET::UI::InputWidget* password;
        YAPET::UI::InputWidget* comment;
        YAPET::UI::Button* okbutton;
        YAPET::UI::Button* cancelbutton;
#ifdef ENABLE_PWGEN
        YAPET::UI::Button* pwgenbutton;
#endif
        YAPET::Key* key;
        YAPET::PartDec* encentry;
        YAPET::UI::secstring s_name;
        YAPET::UI::secstring s_host;
        YAPET::UI::secstring s_username;
        YAPET::UI::secstring s_password;
        YAPET::UI::secstring s_comment;
        bool namechanged;
        bool hostchanged;
        bool usernamechanged;
        bool passwordchanged;
        bool commentchanged;
	bool readonly;

        inline PasswordRecord (const PasswordRecord&) {}
        inline const PasswordRecord& operator= (const PasswordRecord&) {
            return *this;
        }

    protected:

        inline int getWidth() const {
            return maxX() - 8;
        }

        inline int getHeight() const {
            return HEIGHT;
        }

        inline int getStartX() const {
            return maxX() / 2 - getWidth() / 2;
        }

        inline int getStartY() const {
            return maxY() / 2 - getHeight() / 2;
        }

        void createWindow() throw (YAPET::UI::UIException);

        //! Asks the user whether or not he want to cancel
        bool sureToCancel() throw (YAPET::UI::UIException);

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
        PasswordRecord (YAPET::Key& k, YAPET::PartDec* pe, bool ro = false) throw (YAPET::UI::UIException);
        ~PasswordRecord();

        /**
         * @brief Shows the dialog and handles user input.
         *
         * Shows the dialog and handles user input.
         *
         * Call \c getEncEntry() for obtaining the encrypted password record.
         */
        void run() throw (YAPET::UI::UIException);
        void resize() throw (YAPET::UI::UIException);
        void refresh() throw (YAPET::UI::UIException);
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
        inline YAPET::PartDec* getEncEntry() const {
            return encentry;
        }

        /**
         * @brief Indicates whether or not the record has been changed.
         *
         * Indicates whether or not the record has been changed.
         *
         * @return \c true if the record has been changed, \c false otherwise.
         */
        bool entryChanged() const;

	void setReadonly(bool ro);

	inline bool getReadonly() const { return readonly; }
};

#endif // _PASSWORDRECORD_H
