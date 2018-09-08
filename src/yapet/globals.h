// -*- c++ -*-
//
// $Id: mainwindow.h 5207 2013-06-16 16:33:45Z rafisol $
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

#ifndef _GLOBALS_H
#define _GLOBALS_H 1

// Crypt
#include <file.h>

#include "cfg.h"
#include "yacurs.h"

namespace YAPET {
class Globals {
   public:
    static bool records_changed;
    static CONFIG::Config config;
};

/**
 * Submitted by an object requesting self destruction.
 *
 * Since we're mostly event driven, we need an event in order to
 * request self destruction. This allows for an object to be
 * created by a class without requiring the creating class to keep
 * track of states of said object, i.e. once the object has done
 * its chores, it can request self destruction without having the
 * creating class know the exact moment.
 *
 * @see LoadFile
 * @see PromptPassword
 */
const YACURS::EventType EVT_APOPTOSIS("EVT_APOPTOSIS");
}  // namespace YAPET

#endif  // _MAINWINDOW_H
