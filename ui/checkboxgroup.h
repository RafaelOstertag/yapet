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
// YAPET.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef _CHECKBOXGROUP_H
#define _CHECKBOXGROUP_H

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

#ifdef HAVE_INTTYPES_H
# include <inttypes.h>
#endif

#ifdef HAVE_STRING
# include <string>
#endif

#ifdef HAVE_VECTOR
# include <vector>
#endif


#include "uiexception.h"

namespace YAPET {
    namespace UI {

        /**
         * @brief A group of check boxes
         *
         * Displays a group of checkboxes, where each check boc can be individually
         * checked or unchecked. It returns a bit mask representing the checked
         * boxes set to 1 according to their positions on the screen for the caller
         * to find out which check boxes are marked (First checkbox has first bit
         * set (unset), second checkbox has second bit set unset ...).
         *
         * KEY_UP, 'k', KEY_DOWN, 'j', KEY_TAB are selection movement
         * keys. KEY_ESC, KEY_RESIZE will make it loose focus. KEY_ENTER, KEY_SPACE
         * will mark/unmark check box.
         *
         * A maximum of 16 check boxes are supported.
         */
        class CheckBoxGroup {
            private:
                enum {
                    BASE_HEIGHT = 2,
                    BASE_WIDTH = 8,
                    // ZERO BASED!
                    MAX_ITEMS = 15
                };

                WINDOW* window;

                /**
                 * @brief Title of the group
                 *
                 * Title of the group
                 */
                std::string title;

                /**
                 * @ brief the items
                 *
                 * The list of items. If there are more as 16 [sizeof(uint16_t)]
                 * items, only the first 16 are displayed.
                 */
                std::vector<std::string> items;
                // This holds the options (checked and unchecked items)
                uint16_t options;

                int start_x;
                int start_y;
                int width;
                int height;

                inline CheckBoxGroup (const CheckBoxGroup&) {}
                inline const CheckBoxGroup& operator= (const CheckBoxGroup&) {
                    return *this;
                }

                int ItemsUsableWidth() const ;
                int ItemsUsableHeight() const;
                /**
                 * @brief Last position of visible item
                 *
                 * The last (maximum) position of visible item
                 */
                int lastItemPos() const;
                void addCheckBox (int pos) throw (UIException);

                /**
                 * @brief Set/unset check mark.
                 *
                 * Set/unset check mark on the given position.
                 *
                 * @param pos the position (0 .. 15) check box
                 *
                 * @param mark set (\c true) / unset (\c false) mark.
                 */
                void setCheckMark (int pos, bool mark) throw (UIException);

                /**
                 * @brief Set/unset check mark.
                 *
                 * Automatically set/unset the check mark on the given position
                 * depending on the value of \c options.
                 *
                 * @param pos the position (0 .. 15) check box
                 */
                void setCheckMark (int pos) throw (UIException);

                void setCursor (int pos) throw (UIException);
                void displayItems() throw (UIException);

            protected:
                void createWindow() throw (UIException);

            public:
                /**
                 * @brief Constructor
                 *
                 * Constructor.
                 *
                 * @param t the title of the group
                 *
                 * @param it the check box items
                 *
                 * @param o uint16_t holding the selected checkboxes. May be 0.
                 *
                 * @param x the x start position
                 *
                 * @param y the y start position
                 *
                 * @param w the width of the widget
                 *
                 * @param h the height of the widget
                 */
                CheckBoxGroup (std::string t,
                               const std::vector<std::string>& it,
                               uint16_t o,
                               int x,
                               int y,
                               int w,
                               int h)  throw (UIException);
                virtual ~CheckBoxGroup();

                void refresh() throw (UIException);

                //! Focus the widget
                int focus() throw (UIException);

                void resize (int sx, int sy, int w, int h) throw (UIException);

                inline int getHeight() const {
                    return BASE_HEIGHT + items.size();
                }

                inline uint16_t getOptions() const {
                    return options;
                }
        };
    }
}
#endif // _CHECKBOXGROUP_H
