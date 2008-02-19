// -*- c++ -*-
//
// $Id$
//
// YAPET -- Yet Another Password Encryption Tool
// Copyright (C) 2008  Rafael Ostertag
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef _LISTWIDGET_H
#define _LISTWIDGET_H

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

#ifdef HAVE_ITERATOR
# include <iterator>
#endif

#ifdef HAVE_LIST
# include <list>
#endif

#include "uiexception.h"
#include "colors.h"
#include "basewindow.h"

namespace YAPETUI {

    /**
     * @todo Making focus clearly visible.
     *
     * @brief A widget showing a list of items for selecting one.
     *
     * This template shows a list of items on the screen and allows the user to
     * select one of it. If the list is larger than the available screen
     * height, it allows to scroll.
     *
     * The objects stored in the \c std::list are expected to have a method \c
     * c_str() which should return the name or whatever of the item. This
     * string is displayed on the screen
     *
     */
    template<class T>
    class ListWidget {
	private:
	    WINDOW* window;

	    int width;
	    int height;

	    /**
	     * @brief Holds the starting position within the list.
	     *
	     * This holds the position from where we start showing items on the
	     * screen.
	     */
	    int start_pos;
	    /**
	     * @brief The position within the visible items.
	     *
	     * Holds the position within the visible items. By adding \c
	     * cur_pos \c + \c start_pos the item actually selected by the user
	     * as offset from the beginning of the list is yielded.
	     */
	    int cur_pos;

	protected:
	    typename std::list<T> itemlist;
	    typedef typename std::list<T>::size_type l_size_type;

	    inline ListWidget(const ListWidget& lw) {}
	    inline const ListWidget& operator=(const ListWidget& lw) { return *this; }

	    int pagesize() { return height-2; }

	    void clearWin() throw(UIException) {
		Colors::setcolor(window, LISTWIDGET);
		int retval = wclear(window);
		if (retval == ERR)
		    throw UIException("Error clearing window");

		retval = box(window, 0, 0);
		if (retval == ERR)
		    throw UIException("Error drawing box around window");
	    }

	    void showScrollIndicators() throw(UIException) {
 		if (start_pos > 0) {
		    int retval = mvwaddch(window,
					  1,
					  width - 1,
					  '^');
		    if (retval == ERR)
			throw UIException("Unable to display scroll up indicator");
		}

		if ( itemlist.size() - 1  > start_pos + cur_pos &&
		     itemlist.size() > pagesize()) {
		    int retval = mvwaddch(window,
					  height - 2,
					  width - 1,
					  'v');
		    if (retval == ERR)
			throw UIException("Unable to display scroll down indicator");
		}
	    }

	    void showListItems() throw(UIException) {
		int usable_width = width - 2;

		clearWin();

		typename std::list<T>::iterator itemlist_pos = itemlist.begin();
		// Advance to the start point
		for(int i=0; i<start_pos; itemlist_pos++,i++);

		for(int i=0; i<pagesize() && itemlist_pos != itemlist.end(); itemlist_pos++, i++) {
		    int retval = mymvwaddnstr(window,
					      1 + i,
					      1,
					      (*itemlist_pos).c_str(),
					      usable_width);
		    if (retval == ERR)
			throw UIException("Unable to display item");
		}

		showScrollIndicators();
	    }

	    void showSelected(int old_pos) throw(UIException) {
		int retval = 0;

		if (itemlist.size() > 0) {
		    retval = mymvwchgat(window,
				      cur_pos + 1,
				      1,
				      width-2,
				      A_REVERSE,
				      Colors::getcolor(LISTWIDGET),
				      NULL);
		    if (retval == ERR)
			throw UIException("Error move cursor");

		}

		if (old_pos > -1) {
		    retval = mymvwchgat(window,
				      old_pos + 1,
				      1,
				      width-2,
				      A_NORMAL,
				      Colors::getcolor(LISTWIDGET),
				      NULL);
		    if (retval == ERR)
			throw UIException("Error move cursor");
		}
		retval = touchwin(window);
		if (retval == ERR)
		    throw UIException("Error touching window");

		retval = wrefresh(window);
		if (retval == ERR)
		    throw UIException("Error refreshing window");
	    }

	    void scrollUp() {
		if (itemlist.size() == 0) return;

		if (cur_pos>0) {
		    int old_pos = cur_pos--;
		    showSelected(old_pos);
		} else {
		    if (start_pos>0) {
			start_pos--;
			showListItems();
			showSelected(-1);
		    }
		}
	    }

	    void scrollDown() {
		if (itemlist.size() == 0) return;

		if ( ((l_size_type)(cur_pos+start_pos))<itemlist.size()-1) {
		    if (cur_pos < pagesize()-1 ) {
			int old_pos = cur_pos++;
			showSelected(old_pos);
		    } else {
			if (((l_size_type)start_pos)<itemlist.size()-1) {
			    start_pos++;
			    showListItems();
			    showSelected(-1);
			}
		    }
		}
	    }

	    void scrollPageUp() {
		if (itemlist.size() == 0) return;

		int old_pos = cur_pos;
		cur_pos = 0;
		if ( start_pos - pagesize() > 0 ) {
		    start_pos -= pagesize();
		}else {
		    start_pos = 0;
		}
		showListItems();
		showSelected(old_pos);
	    }

	    void scrollPageDown() {
		if (itemlist.size() == 0) return;

		int old_pos = cur_pos;
		if ( ((l_size_type)pagesize()) > itemlist.size() - 1 ) {
		    cur_pos = itemlist.size() - 1;
		    start_pos = 0;
		} else {
		    start_pos += pagesize() - 1;
		    if ( ((l_size_type)start_pos) > itemlist.size() -1 ) {
			start_pos = itemlist.size() - pagesize() - 1;
		    }
		    cur_pos = 0;
		}

		showListItems();
		showSelected(old_pos);
	    }

	    void scrollHome() {
		if (itemlist.size() == 0) return;

		start_pos = 0;
		int old_pos = cur_pos;
		cur_pos = 0;
		showListItems();
		showSelected(old_pos);
	    }

	    void scrollEnd() {
		if (itemlist.size() == 0) return;

		int old_pos = cur_pos;
		start_pos = itemlist.size() - pagesize();
		if (start_pos < 0) {
		    start_pos = 0;
		    cur_pos = itemlist.size()-1;
		} else {
		    cur_pos = pagesize()-1;
		}

		showListItems();
		showSelected(old_pos);
	    }

	    void createWindow(int sx, int sy, int w, int h) throw(UIException) {
		window = newwin(h, w, sy, sx);
		if (window == NULL)
		    throw UIException("Error creating list window");

		Colors::setcolor(window, LISTWIDGET);

		int retval = keypad(window, true);
		if (retval == ERR)
		    throw UIException("Error enabling keypad");

		box(window, 0, 0);

		// We set them here in case the window was resized
		width = w;
		height = h;
	    }

	public:
	    /**
	     * @brief Constructor.
	     *
	     * Initializes the widget, but does not show it.
	     *
	     * @param l the list holding the items to be
	     * displayed. The items of the list are expected to have a
	     * method called \c c_str() for getting their names. Empty
	     * lists are allowed.
	     *
	     * @param sx the horizontal start position of the widget
	     * on the screen.
	     *
	     * @param sy the vertical start position of the widget on
	     * the screen.
	     *
	     * @param w the width of the widget.
	     *
	     * @param h the height of the widget.
	     */
	    ListWidget(std::list<T> l, int sx, int sy, int w, int h)
		throw(UIException) : window(NULL),
				     width(w),
				     height(h),
				     start_pos(0),
				     cur_pos(0),
				     itemlist(l) {
		if ( sx == -1 ||
		     sy == -1 ||
		     width == -1 ||
		     height == -1 )
		    throw UIException("No idea of the dimension of the list");

		createWindow(sx, sy, width, height);
	    }

	    virtual ~ListWidget() {
			wclear(window);
			delwin(window);
	    }

	    /**
	     * @brief Sets a new list of items to display.
	     *
	     * Sets a new list of items to display.
	     *
	     * @param l the list holding the items to be displayed. The items
	     * of the list are expected to have a method called \c c_str() for
	     * getting their names. Empty lists are allowed.
	     */
	    void setList(typename std::list<T>& l) {
		itemlist = l;
		start_pos = 0;
		cur_pos = 0;
		showListItems();
		showSelected(-1);
	    }

	    /**
	     * @brief Replace the item at the current position selected.
	     *
	     * Replaces the item at the current position of the list selected by the user.
	     *
	     * @param item the new item.
	     */
	    void replaceCurrentItem(T& item) {
		typename std::list<T>::iterator itemlist_pos = itemlist.begin();
		for (int i=0;
		     i<(start_pos + cur_pos) && itemlist_pos != itemlist.end();
		     itemlist_pos++, i++);

		*itemlist_pos = item;
	    }

	    void deleteSelectedItem() {
		if (itemlist.size()==0) return;
		typename std::list<T>::iterator itemlist_pos = itemlist.begin();
		for (int i=0;
		     i<(start_pos + cur_pos) && itemlist_pos != itemlist.end();
		     itemlist_pos++, i++);

		if (itemlist_pos == itemlist.end()) return;
		itemlist.erase(itemlist_pos);
		scrollUp();
	    }


	    const std::list<T>& getList() const { return itemlist; }
	    std::list<T>& getList() { return itemlist; }

	    /**
	     * @brief Sets the focus to this widget.
	     *
	     * Focus the widget and shows it on the screen. The widget handles
	     * the following key strokes:
	     *
	     * - \c KEY_UP
	     * - \c KEY_DOWN
	     * - \c KEY_HOME
	     * - \c KEY_A1
	     * - \c KEY_END
	     * - \c KEY_C1
	     * - \c KEY_NPAGE
	     * - \c KEY_C3
	     * - \c KEY_PPAGE
	     * - \c KEY_A3
	     * - \c KEY_REFRESH
	     *
	     * Every other key stroke make it loosing the focus.
	     *
	     * @return the key stroke that made it loose the focus.
	     */
	    virtual int focus() throw(UIException) {
		int retval = box(window, 0, 0);
		if (retval == ERR)
		    throw UIException("Error re-setting the border");

		showScrollIndicators();

		retval = wrefresh(window);
		if (retval == ERR)
		    throw UIException("Error refreshing the list widget");

		int ch;
		bool stay_in_loop = true;
		while (stay_in_loop) {
		    ch = wgetch(window);
		    switch (ch) {
		    case KEY_UP:
			scrollUp();
			break;
		    case KEY_DOWN:
			scrollDown();
			break;
		    case KEY_HOME:
		    case KEY_A1:
			scrollHome();
			break;
		    case KEY_END:
		    case KEY_C1:
			scrollEnd();
			break;
		    case KEY_NPAGE:
		    case KEY_C3:
			scrollPageDown();
			break;
		    case KEY_PPAGE:
		    case KEY_A3:
			scrollPageUp();
			break;
		    case KEY_REFRESH:
			BaseWindow::refreshAll();
			break;
		    default:
			stay_in_loop = false;
			break;
		    }
		}

		retval = box(window, 0, '-');
		if (retval == ERR)
		    throw UIException("Error re-setting the border");
		retval = wrefresh(window);
		if (retval == ERR)
		    throw UIException("Error refreshing the list widget");

		return ch;
	    }

	    void refresh() throw(UIException) {
		showListItems();
		showSelected(-1);

		int retval = wrefresh(window);
		if (retval == ERR)
		    throw UIException("Error refreshing list");
	    }

	    void resize(int sx, int sy, int w, int h) throw(UIException) {
		int retval = wclear(window);
		if (retval == ERR)
		    throw UIException("Error clearing list");

		retval = wrefresh(window);
		if (retval == ERR)
		    throw UIException("Error refreshing window");

		retval = delwin(window);
		if (retval == ERR)
		    throw UIException("Error deleting window");

		createWindow(sx, sy, w, h);
	    }

	    int getListPos() { return start_pos + cur_pos; }

	    T getSelectedItem() {
		typename std::list<T>::iterator itemlist_pos = itemlist.begin();
		for (int i=0;
		     i<(start_pos + cur_pos) && itemlist_pos != itemlist.end();
		     itemlist_pos++, i++);
		return *itemlist_pos;
	    }

	    l_size_type size() { return itemlist.size(); }
    };

}
#endif // _LISTWIDGET_H
