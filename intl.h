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
// this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef _INTL_H
#define _INTL_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "gettext.h"
#ifdef HAVE_LOCALE_H
#include <locale.h>
#endif

#if ! defined(_) && ENABLE_NLS==0
#define _(String) (String)
#endif

#if ! defined(N_) && ENABLE_NLS==0
#define N_(String) (String)
#endif

#if ! defined(_) && ENABLE_NLS==1
#define _(String) gettext(String)
#endif

#if ! defined(N_) && ENABLE_NLS==1
#define N_(String) (String)
#endif

#endif
