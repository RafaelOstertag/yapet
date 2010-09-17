// -*- c++ -*-
//
// $Id$
//
// Copyright (C) 2009  Rafael Ostertag
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

#ifndef _INTINWIDGET_H
#define _INTINWIDGET_H

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#ifdef HAVE_NCURSES_H
# include <ncurses.h>
#else // HAVE_NCURSES_H
# ifdef HAVE_CURSES_H
#  include <curses.h>
# else
#  error "Neither curses.h nor ncurses.h available"
# endif // HAVE_CURSES_H
#endif // HAVE_NCURSES_H
#include "curswa.h" // Leave this here. It depends on the above includes.

#ifdef HAVE_CMATH
# include <cmath>
#endif

#include "inputwidget.h"

namespace YAPET {
    namespace UI {

        template<class t> int getDigitsForType() {
            return static_cast<int>(std::floor ( std::log10 (std::pow (2, (double) sizeof (t) *8 ) ) ) + 1);
        }

        /**
         * @brief An input widget accepting only digits.
         *
         * Basically the same as \c InputWidget, but does only allow digits as
         * input.
         */
        class IntInWidget : public InputWidget {
            private:
                inline const IntInWidget& operator= (const IntInWidget&) {
                    return *this;
                }

            protected:
                virtual void processInput (int ch) throw (UIException);

            public:
                IntInWidget (int sx, int sy, int w, int ml = getDigitsForType<unsigned long>(), bool ro = false ) throw (UIException);
		inline IntInWidget (int sx, int sy, int w, bool ro ) throw (UIException) :
		    InputWidget (sx, sy, w, getDigitsForType<unsigned long>() , ro) {}
                virtual ~IntInWidget();
                unsigned long getInt() const;
		void setInt(unsigned long i);
        };

    }
}
#endif // _INTINWIDGET_
