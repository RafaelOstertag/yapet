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

#ifndef _DIALOGBOX_H
#define _DIALOGBOX_H

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

#include "messagebox.h"

namespace YAPET {
    namespace UI {
        /**
         * @brief The answers available.
         *
         * The answers available for the user. The answer can be retrieved
         * by \c getAnswer().
         */
        enum ANSWER {
            /**
             * The user pressed the \c OK button.
             */
            ANSWER_OK,
            /**
             * The user pressed the \c Cancel button.
             */
            ANSWER_CANCEL
        };

        /**
         * @brief Class for displaying a dialog box.
         *
         * A dialog box presents the user a message and the buttons
         * \c OK and \c Cancel.
         *
         * It is displayed by calling \c run(). To find out which button
         * was pressed, call \c getAnswer().
         */
        class DialogBox : public MessageBox {
            private:
                Button* cancelbutton;
                ANSWER answer;

            public:
                DialogBox (std::string t, std::string m) throw (UIException);
                virtual ~DialogBox();

                /**
                 * @brief Display and runs the dialog.
                 *
                 * Display and runs the dialog. To find out which button
                 * was pressed by the user, call \c getAnswer().
                 *
                 * @return the key the user pressed. In the current
                 * implementation this is always \c \\n.
                 */
                virtual int run() throw (UIException) ;
                virtual void resize() throw (UIException);
                virtual void refresh() throw (UIException);
                /**
                 * @brief Returns the answer of the user.
                 *
                 * Returns the answer of the user.
                 *
                 * @return \c ANSWER_OK if the user pressed to \c OK
                 * button or \c ANSWER_CANCEL if the user pressed the \c
                 * Cancel button.
                 */
                ANSWER getAnswer() const {
                    return answer;
                }
        };

    }
}
#endif // _DIALOGBOX_H
