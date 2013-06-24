// $Id$
//
// Copyright (C) 2008-2010  Rafael Ostertag
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
# include "config.h"
#endif

#ifdef HAVE_FCNTL_H
# include <fcntl.h>
#endif

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif

#ifdef HAVE_SYS_STAT_H
# include <sys/stat.h>
#endif

#ifdef HAVE_STRING_H
# include <string.h>
#endif

#ifdef TIME_WITH_SYS_TIME
# include <sys/time.h>
# include <time.h>
#else
# ifdef HAVE_SYS_TIME_H
#  include <sys/time.h>
# else
#  include <time.h>
# endif
#endif // TIME_WITH_SYS_TIME

#include <cerrno>

#ifdef ENABLE_PWGEN
# include "pwgendialog.h"
#endif
#include "cfg.h"
#include "mainwindow.h"

//
// Private
//
class HotKeyQ : public YACURS::HotKey {
 public:
    HotKeyQ() : HotKey('Q') {}
    HotKeyQ(const HotKeyQ& hkq): HotKey(hkq) {}
    void action() {
	YACURS::EventQueue::submit(YACURS::EVT_QUIT);
    }

    HotKey* clone() const {
	return new HotKeyQ(*this);
    }
};

//
// Protected
//

//
// Public
//
MainWindow::MainWindow(): Window(),
			  recordlist(new YACURS::ListBox<YAPET::PartDec>()) {
    Window::widget(recordlist);
    frame(true);
    add_hotkey(HotKeyQ() );
}

MainWindow::~MainWindow() {
    assert(recordlist!=0);
    delete recordlist;
}

