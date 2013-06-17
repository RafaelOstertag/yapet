// -*- c++ -*-
//
// $Id$
//
// Copyright (C) 2009-2010  Rafael Ostertag
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

#ifndef _PWGENDIALOG_H
#define _PWGENDIALOG_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifndef ENABLE_PWGEN
# error \
    "This file must only be included with ENABLE_PWGEN defined in config.h"
#endif

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif

#include <vector>
#include <string>

#include "secstring.h"
#include "basewindow.h"
#include "button.h"
#include "checkboxgroup.h"
#include "intinwidget.h"
#include "pwgen/pwgen.h"

/**
 * @brief Password Generator Dialog
 *
 * Display the Password generator dialog.
 *
 */
class PWGenDialog : protected YAPET::UI::BaseWindow {
    private:
        WINDOW* window;
        YAPET::UI::CheckBoxGroup* ckbxgroup;
        YAPET::UI::InputWidget* pwdisplay;
        YAPET::UI::IntInWidget* pwleninput;
        YAPET::UI::Button* regenbutton;
        YAPET::UI::Button* okbutton;
        YAPET::UI::Button* cancelbutton;
        YAPET::UI::secstring password;
        YAPET::PWGEN::PWGen pwgen;
        size_t pwlen;
        int ckbox_options;
        bool canceled;

        std::vector<std::string> ckbxitems;

        inline PWGenDialog(const PWGenDialog& bla) : pwgen(bla.ckbox_options)
        {
            assert(0);
        }

        inline const PWGenDialog& operator=(const PWGenDialog&) {
            return *this;
        }

        inline int windowWidth() {
            return maxX() - 8;
        }

        inline int windowHeight() {
            // This window is static. Since we do not let the mainwindow be
            // smaller than 80x24, we can use a hardcoded value
            return 14;
            // return maxY() - 4;
        }

        inline int startX() {
            return minX() + 4;
        }

        inline int startY() {
            return (maxY() - windowHeight() ) / 2;
        }

        std::string getNameOfRNG() const;

        int CheckBoxOptions2Charpools(uint16_t o) const;

        void initCheckBoxItems();

        void createWindow() throw(YAPET::UI::UIException);

        void printTitles() throw(YAPET::UI::UIException);

    public:
        /**
         * The default password length and character pools are taken from
         * YAPET::GLOBALS::Globals
         */
        PWGenDialog() throw(YAPET::UI::UIException);
        virtual ~PWGenDialog();

        void run() throw(YAPET::UI::UIException);

        void refresh() throw(YAPET::UI::UIException);

        void resize() throw(YAPET::UI::UIException);

        inline const YAPET::UI::secstring& getPassword() {
            return password;
        }

        inline bool isCanceled() const {
            return canceled;
        }
};

#endif // _PWGENDIALOG_H
