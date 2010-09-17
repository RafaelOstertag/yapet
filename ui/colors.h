// -*- c++ -*-
//
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

#ifndef _COLORS_H
#define _COLORS_H

#ifdef HAVE_CONFIG_H
# include <config.h>
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

namespace YAPET {
    namespace UI {
        /**
         * @brief The indices of the color array.
         *
         * This are the indices of the color array.
         */
        enum COLORS {
            /**
             * The default color.
             */
            DEFAULT = 0,
            /**
             * The color used for the title of a message box.
             */
            MESSAGEBOX_TITLE,
            /**
             * The color of the message box
             */
            MESSAGEBOX,
            /**
             * The color for an input widget without focus.
             */
            INPUTWIDGET_NOFOCUS,
            /**
             * Color for an input widget with focus.
             */
            INPUTWIDGET_FOCUS,
            /**
             * Color of buttons without focus.
             */
            BUTTON_NOFOCUS,
            /**
             * Color of buttons with focus.
             */
            BUTTON_FOCUS,
            /**
             * Color of listwidgets.
             */
            LISTWIDGET,
            /**
             * Color of Check Box Groups
             */
            CHECKBOXGROUP,
            /**
             * Color of the Check Box Group Title
             */
            CHECKBOXGROUP_TITLE
        };

        /**
         * @brief Struct holding the color.
         *
         * The purpose of this struct is to hold the color and the
         * attribute for the given color. The attribute is used in case
         * the terminal does not support colors.
         */
        struct color {
            /**
             * @brief The number of the pair.
             *
             * The number of the pair as used by (n)curses.
             */
            short no;
            /**
             * @brief The foreground color.
             *
             * The foreground color of the color pair
             */
            short fg;
            /**
             * @brief The background color.
             *
             * The background color of the pair.
             */
            short bg;
            /**
             * @brief The attribute.
             *
             * The attribute used when no color is available.
             */
            int attr;
        };

        /**
         * @brief Class for managing colors.
         *
         * This class is used for managing colors.
         */
        class Colors {
            private:
                /**
                 * @brief Indicates whether or colors has been initialized
                 *
                 * Indicates whether or colors has been initialized by
                 * calling \c initColors().
                 */
                static bool initialized;
                /**
                 * @brief Array holding the colors.
                 *
                 * The color pairs for curses are access by using the
                 * names defined in the \c COLOR enum.
                 */
                static color colors[];
            public:
                /**
                 * @brief Initializes the color pairs.
                 *
                 * Initializes the color pairs used by curses if the
                 * terminal supports colors.
                 */
                static void initColors();
                /**
                 * @brief Set the color of the curses window.
                 *
                 * Sets the color of the curses window. If colors are not
                 * supported by the terminal, it returns the value of the
                 * \c attr field of the \c color struct.
                 */
                static void setcolor (WINDOW* w, COLORS c);
                static void unsetcolor (WINDOW* w, COLORS c);
                /**
                 * @brief Returns the number of the color pair.
                 *
                 * Returns the number used by curses for the given
                 * color. If color support is unavailable, it always
                 * returns 0.
                 *
                 * @return the number of the color pair, or zero if color
                 * support is not available.
                 */
                static short getcolor (COLORS c);
        };

    }
}

#endif // _COLORS_H
