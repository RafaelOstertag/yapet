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

#ifndef _INPUTWIDGET_H
#define _INPUTWIDGET_H

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

#ifdef HAVE_STRING
# include <string>
#endif

#include "uiexception.h"
#include "secstring.h"

namespace YAPET {
    namespace UI {
        /**
         * @brief A widget where text can be entered.
         *
         * A single line widget where text can be entered. The text
         * entered can be obtained by calling \c getText().
         *
         * The text is stored in a \c secstring.
         *
         * To activate the widget, call \c focus().
         *
         * @sa secstring
         */
        class InputWidget {
            private:
                WINDOW* window;
                secstring buffer;

                int max_length;
                int start_pos;
                int pos;
                int width;
                bool text_changed;
		bool readonly;

                inline InputWidget (const InputWidget&) {}
                inline const InputWidget& operator= (const InputWidget&) {
                    return *this;
                }

                void moveBackward() throw (UIException);
                void moveForward() throw (UIException);
                void moveHome() throw (UIException);
                void moveEnd() throw (UIException);

            protected:
                virtual void processBackspace() throw (UIException);
                virtual void processDelete() throw (UIException);
                virtual void processInput (int ch) throw (UIException);
                virtual void createWindow (int sx, int sy, int w) throw (UIException);
                virtual inline const WINDOW* getWindow() const {
                    return window;
                }
                virtual inline WINDOW* getWindow() {
                    return window;
                }
                virtual inline int getStartPos() const {
                    return start_pos;
                }
                virtual inline int getPos() const {
                    return pos;
                }
                virtual inline int getWidth() const {
                    return width;
                }
                virtual inline secstring& getBuffer() {
                    return buffer;
                }
                virtual inline const secstring& getBuffer() const {
                    return buffer;
                }

            public:
                InputWidget (int sx, int sy, int w, int ml = 512, bool ro = false) throw (UIException);
                inline InputWidget (int sx, int sy, int w, bool ro) throw (UIException) {
		    InputWidget(sx, sy, w, 512, ro);
		}
                virtual ~InputWidget();

                virtual int focus() throw (UIException);
                virtual void refresh() throw (UIException);
                virtual void resize (int sx, int sy, int w) throw (UIException);
                virtual void setText (secstring t) throw (UIException);
                virtual inline secstring getText() const {
                    return buffer;
                }
                virtual void clearText();
                /**
                 * setText() does not change that flag, so we can tank
                 * inputwidgets with data, without having to fear to ask the
                 * user whether or not to save changes by mistake.
                 */
                inline void setTextChanged (bool b) {
                    text_changed = b;
                }
                inline bool isTextChanged() const {
                    return text_changed;
                }
                inline bool hasText() const {
                    return !buffer.empty();
                }
		inline void setReadonly(bool ro) {
		    readonly = ro;
		}
		inline bool getReadonly() const {
		    return readonly;
		}
        };

    }
}
#endif // _INPUTWIDGET_H
