// $Id$
//
// Copyright (C) 2008-2011, 2013  Rafael Ostertag
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
#include "config.h"
#endif

#ifdef HAVE_LIBGEN_H
#include <libgen.h>
#endif

#include <cstdlib>
#include <cstring>

#include "cfg.h"
#include "globals.h"
#include "mainwindow.h"
#include "mainwindowhotkeys.h"
#include "opencmdlinefile.h"
#include "passworddialog.h"

namespace INTERNAL {
class Finder {
   private:
    std::string needle;
#ifdef YACURS_USE_WCHAR
    std::string mbstolower(const std::string& mbs) {
        size_t reqsize = std::mbstowcs(0, mbs.c_str(), 0);
        if (reqsize == (size_t)-1)
            throw std::runtime_error(
                _("Finder(): Unable to convert to wide character string"));

        wchar_t* wstr = new wchar_t[reqsize + 1];
        if (wstr == 0) throw std::runtime_error(_("Finder(): out of memory"));
        if (std::mbstowcs(wstr, mbs.c_str(), reqsize + 1) != reqsize) {
            delete[] wstr;
            throw std::runtime_error(
                _("Finder(): Unable to convert to wide character string"));
        }

        for (size_t i = 0; i < reqsize; i++) wstr[i] = towlower(wstr[i]);

        reqsize = std::wcstombs(0, wstr, 0);
        if (reqsize == (size_t)-1) {
            delete[] wstr;
            throw std::runtime_error(
                _("Finder(): Unable to convert to multibyte string"));
        }

        char* str = new char[reqsize + 1];
        if (str == 0) {
            delete[] wstr;
            throw std::runtime_error(_("Finder(): out of memory"));
        }

        if (std::wcstombs(str, wstr, reqsize + 1) != reqsize) {
            delete[] wstr;
            delete[] str;
            throw std::runtime_error(
                _("Finder(): Unable to convert to multibyte string"));
        }

        std::string retval(str);
        delete[] str;
        delete[] wstr;
        return retval;
    }
#endif

   public:
#ifdef HAVE_STRCASESTR
    Finder(std::string n) : needle(n) {}
#else  // HAVE_STRCASESTR
#if defined(YACURS_USE_WCHAR) && defined(HAVE_TOWLOWER)
    Finder(std::string n) : needle(mbstolower(n)) {}
#else  // defined(YACURS_USE_WCHAR) && defined(HAVE_TOWLOWER)
#ifdef HAVE_TOLOWER
    Finder(std::string n) : needle(n) {
        std::transform(needle.begin(), needle.end(), needle.begin(),
                       std::ptr_fun<int, int>(std::tolower));
    }
#else   // defined(HAVE_TOLOWER)
    Finder(std::string n) : needle(n) {}
#endif  // defined(HAVE_TOLOWER)
#endif  // defined(YACURS_USE_WCHAR) && defined(HAVE_TOWLOWER)
#endif  // HAVE_STRCASESTR

    operator const std::string&() { return needle; }

    bool operator()(const yapet::PasswordListItem& haystack) {
#ifdef HAVE_STRCASESTR
        return strcasestr(reinterpret_cast<const char*>(haystack.name()),
                          needle.c_str()) != 0;
#else  // HAVE_STRCASESTR
#if defined(YACURS_USE_WCHAR) && defined(HAVE_TOWLOWER)
        std::string _hay(
            mbstolower(reinterpret_cast<const char*>(haystack.name())));
        return _hay.find(needle) != std::string::npos;
#else  // defined(YACURS_USE_WCHAR) && defined(HAVE_TOWLOWER)
#if defined(HAVE_TOLOWER)
        std::string lower(reinterpret_cast<const char*>(haystack.name()));
        std::transform(lower.begin(), lower.end(), lower.begin(),
                       std::ptr_fun<int, int>(std::tolower));
        return lower.find(needle) != std::string::npos;
#else   // defined(HAVE_TOLOWER)
        return std::strstr(reinterpret_cast<const char*>(haystack.name()),
                           needle.c_str()) != 0;
#endif  // defined(HAVE_TOLOWER)
#endif  // defined(YACURS_USE_WCHAR) && defined(HAVE_TOWLOWER)
#endif  // HAVE_STRCASESTR
    }
};
}  // namespace INTERNAL

//
// Private
//

void MainWindow::apoptosis_handler(YACURS::Event& e) {
    assert(e == YAPET::EVT_APOPTOSIS);

    if (typeid(e) == typeid(YACURS::EventEx<LoadFile*>)) {
        YACURS::EventEx<LoadFile*>& evt =
            dynamic_cast<YACURS::EventEx<LoadFile*>&>(e);

        delete evt.data();
        evt.stop(true);
        return;
    }

    if (typeid(e) == typeid(YACURS::EventEx<CreateFile*>)) {
        YACURS::EventEx<CreateFile*>& evt =
            dynamic_cast<YACURS::EventEx<CreateFile*>&>(e);

        delete evt.data();
        evt.stop(true);
        return;
    }

    if (typeid(e) == typeid(YACURS::EventEx<ChangePassword*>)) {
        YACURS::EventEx<ChangePassword*>& evt =
            dynamic_cast<YACURS::EventEx<ChangePassword*>&>(e);

        delete evt.data();
        evt.stop(true);
        return;
    }

    if (typeid(e) == typeid(YACURS::EventEx<LoadFileCmdLine*>)) {
        YACURS::EventEx<LoadFileCmdLine*>& evt =
            dynamic_cast<YACURS::EventEx<LoadFileCmdLine*>&>(e);

        delete evt.data();
        evt.stop(true);
        return;
    }
}

void MainWindow::window_show_handler(YACURS::Event& e) {
    assert(e == YACURS::EVT_WINDOW_SHOW);
    YACURS::EventEx<YACURS::WindowBase*>& evt =
        dynamic_cast<YACURS::EventEx<YACURS::WindowBase*>&>(e);

    if (evt.data() == this) {
        if (!_fileToLoadOnShow.empty())
            // does aptoptosis, no need to keep the pointer
            (new LoadFileCmdLine(*this, _fileToLoadOnShow))->run();
    }
}

void MainWindow::window_close_handler(YACURS::Event& e) {
    assert(e == YACURS::EVT_WINDOW_CLOSE);
    YACURS::EventEx<YACURS::WindowBase*>& evt =
        dynamic_cast<YACURS::EventEx<YACURS::WindowBase*>&>(e);

    if (helpdialog != 0 && evt.data() == helpdialog) {
        delete helpdialog;
        helpdialog = 0;
        return;
    }

    if (infodialog != 0 && evt.data() == infodialog) {
        delete infodialog;
        infodialog = 0;
        return;
    }

    if (pwgendialog != 0 && evt.data() == pwgendialog) {
        delete pwgendialog;
        pwgendialog = 0;
        return;
    }

    if (passwordrecord != 0 && evt.data() == passwordrecord) {
        if (passwordrecord->dialog_state() == YACURS::DIALOG_OK) {
            if (passwordrecord->changed()) {
                if (passwordrecord->newrecord()) {
                    recordlist->add(*passwordrecord->getEncEntry());
                    YACURS::Curses::statusbar()->set(_("Added new Record"));
                } else {
                    assert(record_index !=
                           (YACURS::ListBox<yapet::PasswordListItem>::lsz_t) -
                               1);

                    // select the record that has been selected when
                    // the dialog opened. Need in case a screen resize
                    // changed the selection index.
                    recordlist->high_light(record_index);
                    recordlist->selected(*passwordrecord->getEncEntry());

                    YACURS::Curses::statusbar()->set(_("Updated Record"));
                }
                YAPET::Globals::records_changed = true;
            }
        }

        // Reset the record index
        record_index = -1;

        delete passwordrecord;
        passwordrecord = 0;
        return;
    }

    if (confirmdelete != 0 && evt.data() == confirmdelete) {
        if (confirmdelete->dialog_state() == YACURS::DIALOG_YES) {
            assert(record_index !=
                   (YACURS::ListBox<yapet::PasswordListItem>::lsz_t) - 1);

            // select the record that has been selected when
            // the dialog opened. Need in case a screen resize
            // changed the selection index.
            recordlist->high_light(record_index);
            recordlist->delete_selected();
            YACURS::Curses::statusbar()->set(_("Delete selected Record"));
            YAPET::Globals::records_changed = true;
        }

        // Reset the record index
        record_index = -1;

        delete confirmdelete;
        confirmdelete = 0;
        return;
    }

    if (confirmquit != 0 && evt.data() == confirmquit) {
        switch (confirmquit->dialog_state()) {
            case YACURS::DIALOG_YES:
                // do not continue in case there were errors during save
                if (!save_records()) break;
            case YACURS::DIALOG_NO:
                // fall thru
                YACURS::EventQueue::submit(YACURS::EVT_QUIT);
                break;
            case YACURS::DIALOG_CANCEL:
                YACURS::Curses::statusbar()->set(_("Quit aborted"));
                break;
            default:
                throw std::runtime_error(
                    _("Unexpected dialog state for confirmsave dialog"));
                break;
        }

        delete confirmquit;
        confirmquit = 0;
        return;
    }

    if (searchdialog != 0 && evt.data() == searchdialog) {
        if (searchdialog->dialog_state() == YACURS::DIALOG_OK) {
            // Make sure we don't search for an empty string.
            if (searchdialog->input().empty()) {
                YACURS::Curses::statusbar()->set(_("Empty search string"));
            } else {
                if (finder != 0) delete finder;

                finder = new INTERNAL::Finder(searchdialog->input());

                if (!recordlist->search(*finder, 0, &last_search_index)) {
                    YACURS::Curses::statusbar()->set(
                        std::string(_("No match for ")) +
                        searchdialog->input());
                    last_search_index = 0;
                }
            }
        }

        delete searchdialog;
        searchdialog = 0;
        return;
    }

    if (errormsgdialog != 0 && evt.data() == errormsgdialog) {
        delete errormsgdialog;
        errormsgdialog = 0;
        return;
    }
}

void MainWindow::listbox_enter_handler(YACURS::Event& e) {
    assert(e == YACURS::EVT_LISTBOX_ENTER);

    if (typeid(e) ==
        typeid(YACURS::EventEx<YACURS::ListBox<yapet::PasswordListItem>*>)) {
        YACURS::EventEx<YACURS::ListBox<yapet::PasswordListItem>*>& evt =
            dynamic_cast<
                YACURS::EventEx<YACURS::ListBox<yapet::PasswordListItem>*>&>(e);
        if (evt.data() != recordlist) return;

        assert(passwordrecord == 0);
        show_password_record(true);
        return;
    }
}

//
// Protected
//

//
// Public
//
MainWindow::MainWindow(const std::string& fileToLoadOnShow)
    : Window{YACURS::Margin{1, 0, 1, 0}},
      recordlist{new YACURS::ListBox<yapet::PasswordListItem>()},
      _fileToLoadOnShow{fileToLoadOnShow},
      helpdialog{nullptr},
      infodialog{nullptr},
      confirmdelete{nullptr},
      confirmquit{nullptr},
      passwordrecord{nullptr},
      errormsgdialog{nullptr},
      searchdialog{nullptr},
      pwgendialog{nullptr},
      finder{nullptr},
      record_index{NO_INDEX},
      last_search_index{0},
      _yapetFile{nullptr},
      _cryptoFactory{nullptr} {
    Window::widget(recordlist);
    frame(false);

    recordlist->sort_order(YACURS::ASCENDING);

    add_hotkey(HotKeyQ(*this));
    add_hotkey(HotKeyq(*this));

    add_hotkey(HotKeyH(*this));
    add_hotkey(HotKeyh(*this));

    add_hotkey(HotKeyR(*this));
    add_hotkey(HotKeyr(*this));

    add_hotkey(HotKeyS(*this));
    add_hotkey(HotKeys(*this));

    add_hotkey(HotKeyE(*this));
    add_hotkey(HotKeye(*this));

    add_hotkey(HotKeyA(*this));
    add_hotkey(HotKeya(*this));

    add_hotkey(HotKeyO(*this));
    add_hotkey(HotKeyo(*this));

    add_hotkey(HotKeyI(*this));
    add_hotkey(HotKeyi(*this));

    add_hotkey(HotKeyD(*this));
    add_hotkey(HotKeyd(*this));

    add_hotkey(HotKeyC(*this));
    add_hotkey(HotKeyc(*this));

    add_hotkey(HotKeyL());
    add_hotkey(HotKeyl());

    add_hotkey(HotKeySearch(*this));

    add_hotkey(HotKeyN(*this));
    add_hotkey(HotKeyn(*this));

    add_hotkey(HotKeyG(*this));
    add_hotkey(HotKeyg(*this));

    YACURS::EventQueue::connect_event(YACURS::EventConnectorMethod1<MainWindow>(
        YACURS::EVT_WINDOW_CLOSE, this, &MainWindow::window_close_handler));

    YACURS::EventQueue::connect_event(YACURS::EventConnectorMethod1<MainWindow>(
        YACURS::EVT_WINDOW_SHOW, this, &MainWindow::window_show_handler));

    YACURS::EventQueue::connect_event(YACURS::EventConnectorMethod1<MainWindow>(
        YAPET::EVT_APOPTOSIS, this, &MainWindow::apoptosis_handler));

    YACURS::EventQueue::connect_event(YACURS::EventConnectorMethod1<MainWindow>(
        YACURS::EVT_LISTBOX_ENTER, this, &MainWindow::listbox_enter_handler));
}

MainWindow::~MainWindow() {
    assert(recordlist != 0);
    delete recordlist;

    if (helpdialog) delete helpdialog;
    if (infodialog) delete infodialog;
    if (confirmdelete) delete confirmdelete;
    if (confirmquit) delete confirmquit;
    if (passwordrecord) delete passwordrecord;
    if (errormsgdialog) delete errormsgdialog;
    if (searchdialog) delete searchdialog;
    if (pwgendialog) delete pwgendialog;
    if (finder) delete finder;

    YACURS::EventQueue::disconnect_event(
        YACURS::EventConnectorMethod1<MainWindow>(
            YACURS::EVT_WINDOW_CLOSE, this, &MainWindow::window_close_handler));

    YACURS::EventQueue::disconnect_event(
        YACURS::EventConnectorMethod1<MainWindow>(
            YACURS::EVT_WINDOW_SHOW, this, &MainWindow::window_show_handler));

    YACURS::EventQueue::disconnect_event(
        YACURS::EventConnectorMethod1<MainWindow>(
            YAPET::EVT_APOPTOSIS, this, &MainWindow::apoptosis_handler));

    YACURS::EventQueue::disconnect_event(
        YACURS::EventConnectorMethod1<MainWindow>(
            YACURS::EVT_LISTBOX_ENTER, this,
            &MainWindow::listbox_enter_handler));
}

void MainWindow::load_password_file(
    const std::string& filename,
    std::shared_ptr<yapet::AbstractCryptoFactory>& cryptoFactory, bool create) {
    try {
        _cryptoFactory = cryptoFactory;
        _yapetFile = std::unique_ptr<YAPET::File>{
            new YAPET::File{_cryptoFactory, filename, create,
                            YAPET::Globals::config.filesecurity}};
        YAPET::Globals::records_changed = false;

        recordlist->clear();
        recordlist->set(_yapetFile->read());
        std::string msg(_("Opened file: "));
        YACURS::Curses::statusbar()->set(msg + _yapetFile->getFilename());

        std::string ttl("YAPET");
#ifdef HAVE_BASENAME
        ttl += " (";
        // basename may modify the string
        char* tmp = strdup(_yapetFile->getFilename().c_str());
        ttl += basename(tmp);

        ttl += ")";
        std::free(tmp);
#endif
        YACURS::Curses::set_terminal_title(ttl);
    } catch (std::exception& e) {
        recordlist->clear();

        assert(errormsgdialog == 0);

        errormsgdialog =
            new YACURS::MessageBox2(_("Error"), _("Error while reading file:"),
                                    e.what(), YACURS::OK_ONLY);
        errormsgdialog->show();
    }
}

void MainWindow::show_password_record(bool selected) {
    assert(passwordrecord == 0);

    if (!_yapetFile) return;

    if (selected) {
        if (recordlist->empty()) return;

        assert(record_index ==
               (YACURS::ListBox<yapet::PasswordListItem>::lsz_t) - 1);
        record_index = recordlist->selected_index();
        passwordrecord =
            new PasswordRecord(_cryptoFactory, recordlist->selected());
    } else {
        passwordrecord = new PasswordRecord(_cryptoFactory);
    }

    passwordrecord->show();
}

bool MainWindow::save_records() {
    if (!_yapetFile) {
        // Do nothing and return
        return true;
    }

    try {
        if (YAPET::Globals::records_changed) {
            _yapetFile->save(recordlist->list());
            std::string msg(_("Saved file: "));
            YACURS::Curses::statusbar()->set(msg + _yapetFile->getFilename());
            YAPET::Globals::records_changed = false;
        } else {
            YACURS::Curses::statusbar()->set(_("No changes need to be saved"));
        }
        return true;
    } catch (std::exception& e) {
        assert(errormsgdialog == 0);

        errormsgdialog =
            new YACURS::MessageBox2(_("Error"), _("Error while saving file:"),
                                    e.what(), YACURS::OK_ONLY);
        errormsgdialog->show();
        return false;
    }
}

void MainWindow::change_password(
    std::shared_ptr<yapet::AbstractCryptoFactory>& newCryptoFactory) {
    if (!_cryptoFactory) return;

    if (!newCryptoFactory)
        throw std::invalid_argument(_("New key must not be 0"));

    try {
        _cryptoFactory = newCryptoFactory;
        _yapetFile->setNewKey(_cryptoFactory);
        // Reread the records
        recordlist->set(_yapetFile->read());

        YACURS::Curses::statusbar()->set(
            std::string(_("Changed password on ")) + _yapetFile->getFilename());
    } catch (std::exception& e) {
        assert(errormsgdialog == 0);

        errormsgdialog = new YACURS::MessageBox2(
            _("Error"), _("Error while changing password:"), e.what(),
            YACURS::OK_ONLY);
        errormsgdialog->show();
    }
}

void MainWindow::delete_selected() {
    assert(confirmdelete == 0);

    if (recordlist->empty()) return;

    assert(record_index == NO_INDEX);
    record_index = recordlist->selected_index();
    confirmdelete = new YACURS::MessageBox(
        _("Confirm Deletion"), _("Do you want to delete the selected record?"),
        YACURS::YESNO);

    confirmdelete->show();
}

void MainWindow::show_help() {
    assert(helpdialog == 0);

    helpdialog = new HelpDialog;
    helpdialog->show();
}

void MainWindow::show_info() {
    assert(infodialog == 0);

    infodialog = new InfoDialog(*this, recordlist->list().size());
    infodialog->show();
}

void MainWindow::show_pwgen() {
    assert(pwgendialog == 0);

    pwgendialog = new PwGenDialog;
    pwgendialog->show();
}

void MainWindow::quit() {
    if (YAPET::Globals::records_changed) {
        assert(confirmquit == 0);
        confirmquit = new YACURS::MessageBox2(
            _("Confirmation"), _("There are unsaved changes."),
            _("Do you want to save changes, before leaving?"),
            YACURS::YESNOCANCEL);
        confirmquit->show();
    } else {
        YACURS::EventQueue::submit(YACURS::EVT_QUIT);
    }
}

void MainWindow::sort_asc(bool f) {
    if (f) {
        recordlist->sort_order(YACURS::ASCENDING);
        YACURS::Curses::statusbar()->set(_("Sort Order Ascending"));
    } else {
        recordlist->sort_order(YACURS::DESCENDING);
        YACURS::Curses::statusbar()->set(_("Sort Order Descending"));
    }
}

bool MainWindow::sort_asc() const {
    return recordlist->sort_order() == YACURS::ASCENDING;
}

void MainWindow::search_first() {
    assert(searchdialog == 0);

    if (recordlist->empty()) return;  // there is nothing to search

#if defined(HAVE_STRCASESTR) || defined(HAVE_TOLOWER) || defined(HAVE_TOWLOWER)
    searchdialog = new YACURS::InputBox(_("Search"), _("Enter search term"));
#else
    searchdialog = new YACURS::InputBox(
        _("Search"), _("Enter search term (case-sensitive)"));
#endif
    searchdialog->show();
}

void MainWindow::search_next() {
    if (recordlist->empty()) return;  // there is nothing to search

    if (finder == 0) {
        YACURS::Curses::statusbar()->set(
            _("No previous search. Press '/' to search."));
        return;
    }

    if (!recordlist->search(*finder, last_search_index + 1,
                            &last_search_index)) {
        YACURS::Curses::statusbar()->set(
            std::string(_("No more matches for ")) +
            static_cast<const std::string&>(*finder) +
            std::string(_(" found")));
        last_search_index = 0;
        delete finder;
        finder = 0;
    } else {
        YACURS::Curses::statusbar()->set(
            std::string(_("Next match for ")) +
            static_cast<const std::string&>(*finder));
    }
}

std::string MainWindow::currentFilename() const {
    if (!_yapetFile) {
        return std::string{};
    }

    return _yapetFile->getFilename();
}

std::string MainWindow::fileVersion() const {
    if (!_yapetFile) {
        return std::string{};
    }

    auto fileVersion{_yapetFile->getFileVersion()};

    auto stringSize{fileVersion.size() + 1};

    char* buffer{new char[stringSize]};
    std::memcpy(buffer, *fileVersion, fileVersion.size());
    buffer[stringSize - 1] = '\0';

    std::string returnValue{buffer};

    delete[] buffer;
    return returnValue;
}

std::int64_t MainWindow::passwordLastChanged() const {
    if (!_yapetFile) {
        return -1;
    }
    return _yapetFile->getMasterPWSet();
}

bool MainWindow::matchPasswordWithCurrent(
    const yapet::SecureArray& password) const {
    if (!_cryptoFactory) {
        return false;
    }

    auto temporayCryptoFactory{_cryptoFactory->newFactory(password)};

    auto currentKey{_cryptoFactory->key()};
    auto keyFromPassword{temporayCryptoFactory->key()};

    return *currentKey == *keyFromPassword;
}