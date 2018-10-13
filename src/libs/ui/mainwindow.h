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

#ifndef _MAINWINDOW_H
#define _MAINWINDOW_H 1

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_STRINGS_H
#include <strings.h>
#endif

// Forward declaration required for resolving cyclic dependency of info.h and
// this file
class MainWindow;
class InfoDialog;

#include <yacurs.h>

#include <cstring>
#include <limits>
#include <string>

#include "file.h"
#include "help.h"
#include "info.h"
#include "passwordlistitem.hh"
#include "passwordrecord.h"
#include "pwgendialog.h"

namespace INTERNAL {

class Finder;

}  // namespace INTERNAL

class MainWindow : public YACURS::Window {
   private:
    YACURS::ListBox<yapet::PasswordListItem>* recordlist;
    std::string _fileToLoadOnShow;
    HelpDialog* helpdialog;
    InfoDialog* infodialog;
    YACURS::MessageBox* confirmdelete;
    YACURS::MessageBox2* confirmquit;
    PasswordRecord* passwordrecord;
    YACURS::MessageBox2* errormsgdialog;
    YACURS::InputBox* searchdialog;
    PwGenDialog* pwgendialog;
    INTERNAL::Finder* finder;
    // Used when opening an existing record or deleting a record
    // to store the index, so that it can be reselect in case of a
    // window resize.
    YACURS::ListBox<yapet::PasswordListItem>::lsz_t record_index;
    constexpr static auto NO_INDEX{std::numeric_limits<
        YACURS::ListBox<yapet::PasswordListItem>::lsz_t>::max()};
    // Used for search. Keeps the position of the last succesfull
    // match.
    YACURS::ListBox<yapet::PasswordListItem>::lsz_t last_search_index;
    std::unique_ptr<YAPET::File> _yapetFile;
    std::shared_ptr<yapet::AbstractCryptoFactory> _cryptoFactory;

    MainWindow(const MainWindow&) {}

    const MainWindow& operator=(const MainWindow&) { return *this; }

    void apoptosis_handler(YACURS::Event& e);

    void window_show_handler(YACURS::Event& e);

    void window_close_handler(YACURS::Event& e);

    void listbox_enter_handler(YACURS::Event& e);

   public:
    MainWindow(const std::string& fileToLoadOnShow = std::string{});
    virtual ~MainWindow();

    /**
     * Load password file.
     *
     * Unconditionally load a password file, i.e. changes not
     * committed to disk on an already shown file will be lost.
     *
     * @param yapetFile the YAPET::File. MainWindow will take ownership of the
     * unique_pointer
     *
     * @param cryptoFactory the crypt factory
     */
    void load_password_file(
        const std::string& filename,
        std::shared_ptr<yapet::AbstractCryptoFactory>& cryptoFactory,
        bool createFile);

    /**
     * @param selected whether or not to preload the dialog with
     * the currently selected password record.
     */
    void show_password_record(bool selected);

    void delete_selected();

    /**
     * Save records.
     *
     * @return @c false if there were errors, @c true otherwise.
     */
    bool save_records();

    void change_password(
        std::shared_ptr<yapet::AbstractCryptoFactory>& newCryptoFactory);

    void show_help();

    void show_info();

    void show_pwgen();

    void quit();

    /**
     * Get the number of password records.
     */
    YACURS::ListBox<>::lsz_t size() { return recordlist->list().size(); }

    void sort_asc(bool f);
    bool sort_asc() const;

    void search_first();

    void search_next();

    std::string currentFilename() const;
    std::string fileVersion() const;
    std::int64_t passwordLastChanged() const;
    bool matchPasswordWithCurrent(const yapet::SecureArray& password) const;
};

#endif  // _MAINWINDOW_H
