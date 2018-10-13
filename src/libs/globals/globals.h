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

#ifndef _GLOBALS_H
#define _GLOBALS_H 1

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
