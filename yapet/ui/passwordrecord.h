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
#define _PASSWORDRECORD_H 1

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <string>
#include <yacurs.h>

#include "file.h"
#include "key.h"
#include "partdec.h"
#ifdef ENABLE_PWGEN
# include "pwgendialog.h"
#endif

/**
 * @brief A window that displays all the information associated with a
 * decrypted password record.
 *
 * A window that displays all the information associated with a
 * decrypted password record. The window allows edition of the
 * informations. If the information are edited, a call to \c
 * entryChanged() yields \c true if the record has been edited.
 *
 * To display an existing record, provide a valid pointer to \c
 * PartDec object when constructing the object. If the record has been
 * changed, \c getEncEntry() will return the pointer to the \c PartDec
 * object holding the altered record.
 *
 * To display a window for creating a new password record, pass \c
 * NULL to the \c PartDec pointer argument when constructing. The new
 * record can be obtained by calling \c getEncEntry().
 *
 * In any case, the memory occupied by the pointer returned by \c
 * getEncEntry() has to be freed by the caller. The class does not
 * take care of this.
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
        YACURS::MessageBox* errordialog;
	YACURS::MessageBox* confirmdialog
;	YAPET::PartDec* encentry;
	const YAPET::Key* __key;
	bool __newrecord;
        bool __readonly;

	/**
	 * When @c true, dialog will be closed without asked for
	 * confirmation if cancel has been pressed.
	 */
	bool __force_close;

        const PasswordRecord& operator=(const PasswordRecord&) {
            return *this;
        }

        virtual void on_ok_button();

        void window_close_handler(YACURS::Event& e);

	virtual bool on_close();

    public:
        /**
         * @brief Constructor.
         *
         * Depending on the value passed in \c pe, either an empty
         * record is showed or the decrypted password record including
         * the password stored in the record in plain text is showed
         * except the password record is displaying in read-only mode.
         */
        PasswordRecord( const YAPET::Key* key, const YAPET::PartDec* pe=0);
        ~PasswordRecord();

        /**
         * @brief Returns the password record.
         *
         * Returns the new or altered password record as \c PartDec
         * object. The caller is responsible for freeing the memory
         * associated with the pointer returned.
         *
         * It returns \c NULL if the dialog has been canceled.
         *
         * @return pointer to the new or altered password record, or
         * \c NULL if the dialog has been canceled. The caller is
         * responsible for freeing the memory associated with the
         * pointer returned.
         */
        YAPET::PartDec* getEncEntry() const {
            return encentry;
        }

        /**
         * @brief Indicates whether or not the record has been
         * changed.
         *
         * Indicates whether or not the record has been changed.
         *
         * @return \c true if the record has been changed, \c false
         * otherwise.
         */
        bool changed() const;

	void readonly(bool f);

        bool readonly() const {
            return __readonly;
        }

	bool newrecord() const {
	    return __newrecord;
	}
};

#endif // _PASSWORDRECORD_H
