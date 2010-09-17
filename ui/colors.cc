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

#ifdef HAVE_ASSERT_H
# include <assert.h>
#endif

#include "colors.h"

using namespace YAPET::UI;

bool Colors::initialized = false;

#ifndef _GIRLYCOLORS
color Colors::colors[] = {
    // Normal
    {1, COLOR_WHITE, COLOR_BLUE, A_NORMAL},
    // Message box title
    {2, COLOR_CYAN, COLOR_BLACK, A_NORMAL},
    // Message box
    {3, COLOR_BLACK, COLOR_CYAN, A_NORMAL},
    // Input widget no focus
    {4, COLOR_BLACK, COLOR_WHITE, A_REVERSE},
    // Input widget focus
    {5, COLOR_BLACK, COLOR_YELLOW, A_REVERSE},
    // Button no focus
    {6, COLOR_WHITE, COLOR_BLACK, A_NORMAL},
    // Button focus
    {7, COLOR_BLACK, COLOR_GREEN, A_REVERSE},
    // List widget
    {8, COLOR_WHITE, COLOR_BLUE, A_NORMAL},
    // Check Box Group
    {9, COLOR_BLACK, COLOR_CYAN, A_NORMAL},
    // Check Box Group Title
    {10, COLOR_CYAN, COLOR_BLACK, A_NORMAL},
    // The marker of the end
    {0, 0, 0, 0}
};
#else
//
// Don't ask, my wife insisted... ;)
//
color Colors::colors[] = {
    // Normal
    {1, COLOR_BLUE, COLOR_CYAN, A_NORMAL},
    // Message box title
    {2, COLOR_CYAN, COLOR_BLACK, A_NORMAL},
    // Message box
    {3, COLOR_BLACK, COLOR_CYAN, A_NORMAL},
    // Input widget no focus
    {4, COLOR_BLACK, COLOR_WHITE, A_REVERSE},
    // Input widget focus
    {5, COLOR_WHITE, COLOR_MAGENTA, A_REVERSE},
    // Button no focus
    {6, COLOR_BLUE, COLOR_CYAN, A_NORMAL},
    // Button focus
    {7, COLOR_CYAN, COLOR_MAGENTA, A_REVERSE},
    // List widget
    {8, COLOR_BLUE, COLOR_CYAN, A_NORMAL},
    // Check Box Group
    {9, COLOR_BLACK, COLOR_CYAN, A_NORMAL},
    // Check Box Group Title
    {10, COLOR_CYAN, COLOR_BLACK, A_NORMAL},
    // The marker of the end
    {0, 0, 0, 0}
};
#endif


void
Colors::initColors() {
    if (initialized) return;

    if (has_colors() == FALSE) {
        initialized = true;
        return;
    }

    start_color();
    assert (colors[CHECKBOXGROUP_TITLE+1].no == 0);

    for (int i = 0; colors[i].no != 0; i++)
        init_pair (colors[i].no,
                   colors[i].fg,
                   colors[i].bg);

    initialized = true;
}

void
Colors::setcolor (WINDOW* w, COLORS c) {
    if (has_colors() == TRUE) {
        wattron (w, COLOR_PAIR (colors[c].no) );
        wbkgd (w, ' ' | COLOR_PAIR (colors[c].no) );
    } else {
        wattron (w, colors[c].attr);
        wbkgd (w, ' ' | colors[c].attr);
    }
}

short
Colors::getcolor (COLORS c) {
    if (has_colors() == TRUE) {
        return colors[c].no;
    }

    return 0;
}

void
Colors::unsetcolor (WINDOW* w, COLORS c) {
    if (has_colors() == TRUE)
        wattroff (w, COLOR_PAIR (colors[c].no) );
    else
        wattroff (w, colors[c].attr);

    wbkgd (w, ' ');
}
