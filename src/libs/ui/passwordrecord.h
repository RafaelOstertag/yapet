/*
 * Copyright (C) 2018 Rafael Ostertag
 *
 * This file is part of YAPET.
 *
 * YAPET is free software: you can redistribute it and/or modify it under the
 * terms of the GNU General Public License as published by the Free Software
 * Foundation, either version 3 of the License, or (at your option) any later
 * version.
 *
 * YAPET is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
 * FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more
 * details.
 *
 * You should have received a copy of the GNU General Public License along with
 * YAPET.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Additional permission under GNU GPL version 3 section 7
 *
 * If you modify this program, or any covered work, by linking or combining it
 * with the OpenSSL project's OpenSSL library (or a modified version of that
 * library), containing parts covered by the terms of the OpenSSL or SSLeay
 * licenses, Rafael Ostertag grants you additional permission to convey the
 * resulting work.  Corresponding Source for a non-source form of such a
 * combination shall include the source code for the parts of OpenSSL used as
 * well as that of the covered work.
 */

#ifndef _PASSWORDRECORD_H
#define _PASSWORDRECORD_H 1

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <yacurs.h>

#include <string>

#include "file.hh"
#include "pwgendialog.h"

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
 * 0 to the \c PartDec pointer argument when constructing. The new
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
    YACURS::Button* pwgenbutton;
    YACURS::Spacer* pwgenbutton_spacer;

    YACURS::MessageBox* errordialog;
    YACURS::MessageBox* confirmdialog;
    PwGenDialog* pwgendialog;

    std::shared_ptr<yapet::AbstractCryptoFactory> _cryptoFactory;
    std::shared_ptr<yapet::PasswordListItem> _passwordListItem;
    bool _newrecord;
    bool _readonly;
    bool _password_hidden;

    /**
     * When @c true, dialog will be closed without asked for
     * confirmation if cancel has been pressed.
     */
    bool _force_close;
    bool _modified_by_pwgen;

    virtual void on_ok_button();

    void button_press_handler(YACURS::Event& e);

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
    PasswordRecord(std::shared_ptr<yapet::AbstractCryptoFactory>& cryptoFactory,
                   const yapet::PasswordListItem& passwordListItem);
    PasswordRecord(
        std::shared_ptr<yapet::AbstractCryptoFactory>& cryptoFactory);
    ~PasswordRecord();

    PasswordRecord(const PasswordRecord&) = delete;
    PasswordRecord(PasswordRecord&&) = delete;
    PasswordRecord& operator=(const PasswordRecord&) = delete;
    PasswordRecord& operator=(PasswordRecord&&) = delete;

    /**
     * @brief Returns the password record.
     *
     * Returns the new or altered password record as \c PasswordListItem
     * object. The caller is responsible for freeing the memory
     * associated with the pointer returned.
     *
     * It returns \c 0 if the dialog has been canceled.
     *
     * @return pointer to the new or altered password record, or
     * \c 0 if the dialog has been canceled. The caller is
     * responsible for freeing the memory associated with the
     * pointer returned.
     */
    std::shared_ptr<yapet::PasswordListItem> getEncEntry() const {
        return _passwordListItem;
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

    bool readonly() const { return _readonly; }

    void password_hidden(bool f);

    bool password_hidden() const { return _password_hidden; }

    bool newrecord() const { return _newrecord; }
};

#endif  // _PASSWORDRECORD_H
