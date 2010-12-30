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

#ifdef HAVE_STDEXCEPT
# include <stdexcept>
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
	 * If the widget is set hidden, it will not display the text. Useful
	 * for passwords.
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
		int start_x;
		int start_y;
		// This is a variable width which is used to display hidden
		// text
                int width;
		// This has all time to hold the max width of the widget
		int full_width;
                bool text_changed;
		bool readonly;
		/**
		 * Hidden indicates that the foreground color is the same as
		 * the background color. It is not to be confused with the
		 * widget being hidden or the passwordinput wideget.
		 */
		bool hidden;

		/**
		 * Using the default constructor will lead to SIGSEGV. This is
		 * due to the way we handle initialization of curses stuff.
		 *
		 * You will have to explicitely write every constructor as
		 * shown below.
		 */
		inline InputWidget() throw (std::runtime_error) {
		    throw std::runtime_error(_("Default constructor must not be used!"));
		}

                inline InputWidget (const InputWidget&) {}
                inline const InputWidget& operator= (const InputWidget&) {
                    return *this;
                }

                void moveBackward() throw (UIException);
                void moveForward() throw (UIException);
                void moveHome() throw (UIException);
                void moveEnd() throw (UIException);
		void hide(bool h) throw(UIException);

		enum {
		    DEFAULT_TEXT_LEN=512
		};

            protected:
                virtual void processBackspace() throw (UIException);
                virtual void processDelete() throw (UIException);
                virtual void processInput (int ch) throw (UIException);
                virtual void createWindow () throw (UIException);
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
                InputWidget (int sx, int sy, int w, int ml = DEFAULT_TEXT_LEN, bool ro = false, bool h = false) throw (UIException);
                InputWidget (int sx, int sy, int w, bool ro, bool h) throw (UIException);

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

		void setReadonly(bool ro);

		inline bool getReadonly() const {
		    return readonly;
		}

		void setHidden(bool h);
		inline bool getHidden() const {
		    return hidden;
		}
        };

    }
}
#endif // _INPUTWIDGET_H
