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

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include "../intl.h"
#include "intinwidget.h"

#ifdef HAVE_CTYPE_H
# include <ctype.h>
#endif

#ifdef HAVE_STDLIB_H
# include <stdlib.h>
#endif

using namespace YAPET::UI;

IntInWidget::IntInWidget (int sx, int sy, int w, int ml, bool ro)
    throw (UIException) : InputWidget (sx, sy, w, ml, ro) {
}

IntInWidget::~IntInWidget() {
}

void
IntInWidget::processInput (int ch) throw (UIException) {
    if (isdigit (ch) )
        InputWidget::processInput (ch);
}

unsigned long
IntInWidget::getInt() const {
    if (!hasText() ) return 0;

    return strtol (getText().c_str(), NULL, 10);
}

void
IntInWidget::setInt(unsigned long i) {
    const int size = 40;
    char buff[size];
    snprintf(buff,size,"%lu", i);
    setText(buff);
}
