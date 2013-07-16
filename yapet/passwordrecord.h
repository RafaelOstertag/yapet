// -*- c++ -*-
//
// $Id$
//
// Copyright (C) 2008-2013  Rafael Ostertag
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
# include "config.h"
#endif

# include <string>

#include "file.h"
#include "key.h"
#include "partdec.h"
#ifdef ENABLE_PWGEN
# include "pwgendialog.h"
#endif

#include <yacurs.h>

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
class PasswordRecord : public YACURS::Dialog {
    private:
	YACURS::VPack* vpack;
	YACURS::Label* lname;
	YACURS::Label* lhost;
	YACURS::Label* lusername;
	YACURS::Label* lpassword;
	YACURS::Label* lcomment;
        YACURS::Input<std::string>* name;
        YACURS::Input<std::string>* host;
        YACURS::Input<std::string>* username;
        YACURS::Input<std::string>* password;
        YACURS::Input<std::string>* comment;
#ifdef ENABLE_PWGEN
        YACURS::Button* pwgenbutton;
#endif
        YAPET::PartDec* encentry;
	YACURS::MessageBox* errordialog;
        bool __readonly;

        const PasswordRecord& operator=(const PasswordRecord&) {
            assert(0);
            return *this;
        }

	virtual void button_press_handler(YACURS::Event& e);
	void window_close_handler(YACURS::Event& e);

    public:
        /**
         * @brief Constructor.
         *
         * Depending on the value passed in \c pe, either an empty record is
         * showed or the decrypted password record including the password
         * stored in the record in plain text is showed except the password
         * record is displaying in read-only mode.
         *
         * @param k the key used to decrypt/encrypt the password record.
         *
         * @param f the password file. This is only used for locking the screen.
         *
         * @param pe pointer to a \c PartDec which will be displayed, or \c
         * NULL in order to obtain a new password record.
         *
         * @param ro specify whether or not the dialog is readonly.
         */
        PasswordRecord(YAPET::PartDec* pe=NULL);
        ~PasswordRecord();

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

        void readonly(bool ro);

        inline bool readonly() const {
            return __readonly;
        }
};

#endif // _PASSWORDRECORD_H
