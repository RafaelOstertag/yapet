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

KeyDesc keys[] = { {3, 2, "S", _("Save File") },
                   {4, 2, "R", _("Load File") },
                   {5, 2, "L", _("Lock Screen") },
                   {6, 2, "A", _("Add Entry") },
                   {7, 2, "D", _("Delete Entry") },
                   {8, 2, "O", _("Sort Order") },
                   {9, 2, "/", _("Search") },
                   {10, 2, "N", _("Search Next") },
                   {11, 2, "C", _("Change Password") },
                   {12, 2, "^L", _("Redraw Screen") },
                   {13, 2, "Q", _("Quit") },
#ifdef ENABLE_PWGEN
                   {14, 2, "G", _("Password Generator") },
#endif
                   {0, 0, NULL, NULL}};



//
// Private
//

//
// Protected
//

//
// Public
//
