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

#ifndef _YAPETUNLOCKDIALOG_H
#define _YAPETUNLOCKDIALOG_H 1

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <yacurs.h>
#include "mainwindow.h"

class YapetUnlockDialog : public YACURS::UnlockDialog {
   private:
    const MainWindow& _mainWindow;

    YACURS::VPack* _vpack;
    YACURS::DynLabel* _text1;
    YACURS::DynLabel* _text2;
    YACURS::DynLabel* _text3;
    YACURS::Input<>* _secret_input;
    YACURS::Button* _quit;
    YACURS::Spacer* _quit_spacer;

   protected:
    void window_show_handler(YACURS::Event& _e);

    // From dialog
    void button_press_handler(YACURS::Event& _e);

   public:
    YapetUnlockDialog(const YACURS::Window& mainWindow);
    ~YapetUnlockDialog();

    YapetUnlockDialog(const YapetUnlockDialog&) = delete;
    YapetUnlockDialog& operator=(const YapetUnlockDialog&) = delete;
    YapetUnlockDialog(YapetUnlockDialog&&) = delete;
    YapetUnlockDialog& operator=(YapetUnlockDialog&&) = delete;

    bool unlock();

    void clear();
};

#endif  // _YAPETUNLOCKDIALOG_H
