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

#ifdef HAVE_CTYPE_H
# include <ctype.h>
#endif

#ifdef HAVE_FUNCTIONAL
# include <functional>
#endif

#ifdef HAVE_ITERATOR
# include <iterator>
#endif

#ifdef HAVE_LIST
# include <list>
#endif

#ifdef HAVE_ALGORITHM
# include <algorithm>
#endif

#ifdef HAVE_STRING_H
# include <string.h>
#endif

#include "../intl.h"
#include "uiexception.h"
#include "colors.h"
#include "basewindow.h"

namespace YAPETUI {

    /**
     * @brief A widget showing a list of items for selecting one.
     *
     * This template shows a list of items on the screen and allows
     * the user to select one of it. If the list is larger than the
     * available screen height, it allows to scroll.
     *
     * The objects stored in the \c std::list are expected to have a
     * method \c c_str() which should return the name or whatever of
     * the item. This string is displayed on the screen. Further, the
     * object need to provide a less-than operator for sorting
     * purpose.
     *
     */
    template<class T>
    class ListWidget {
	public:
	    /**
	     * @brief The sort order available
	     *
	     * The sort order that are available for sorting the list
	     * items.
	     */
	    enum SortOrder {
		ASCENDING,
		DESCENDING
	    };

	private:
	    /**
	     * @brief Used to search for item names
	     *
	     * This class is used to search for items containing a
	     * certain search term in their name.
	     */
	    class ItemContains : public std::unary_function<T,bool> {
		private:
		    const char* searchterm;
		public:
		    explicit ItemContains(const char* t) :
			searchterm(t) {}
		    bool operator()(const T& item) {
			const char* ptr;
#ifdef HAVE_STRCASESTR
			ptr = strcasestr(item.c_str(), searchterm);
#elif HAVE_STRSTR
# ifdef HAVE_TOLOWER
			// Prepare the haystack
			size_t haystack_len = strlen(item.c_str())+1;
			char* haystack = new char[haystack_len];
			strncpy(haystack, item.c_str(), haystack_len);
			for (size_t i=0; i < haystack_len; haystack[i] = (char)tolower(haystack[i]), i++);

			// Prepare needle
			size_t needle_len = strlen(searchterm)+1;
			char* needle = new char[needle_len];
			strncpy(needle, searchterm, needle_len);
			for (size_t i=0; i < needle_len; needle[i] = (char)tolower(needle[i]), i++);

			// Now do the search
			ptr = strstr(haystack, needle);

			// Destroy that properly
			memset((void*)haystack, 0, haystack_len);
			memset((void*)needle, 0, needle_len);
			delete []haystack;
			delete []needle;
# else
			ptr = strstr(item.c_str(), searchterm);
# endif // HAVE_TOLOWER
#else
# error "Sorry, neither strcasestr() nor strstr() found"
#endif
			if (ptr != NULL)
			    return true;

			return false;
		    }
	    };

	    WINDOW* window;

	    int width;
	    int height;

	    /**
	     * @brief Indicates whether or not the widget has the focus
	     *
	     * Indicates whether or not this widget has the
	     * focus. Determines how the border has to be drawn
	     */
	    bool hasfocus;

	    /**
	     * @brief Holds the starting position within the list.
	     *
	     * This holds the position from where we start showing
	     * items on the screen.
	     */
	    int start_pos;

	    /**
	     * @brief The position within the visible items.
	     *
	     * Holds the position within the visible items. By adding
	     * \c cur_pos \c + \c start_pos the item actually selected
	     * by the user as offset from the beginning of the list is
	     * yielded.
	     */
	    int cur_pos;

	    /**
	     * @brief Holds the sort order currently applied
	     *
	     * Holds the sort order that is currently applied to the
	     * list.
	     */
	    SortOrder sortorder;

	    /**
	     * @brief Items displayed.
	     *
	     * The actual list holding the items displayed by the
	     * widget.
	     */
	    typename std::list<T> itemlist;
	    typedef typename std::list<T>::size_type l_size_type;

	    typedef typename std::list<T>::iterator list_it;
	    typedef typename std::list<T>::const_iterator c_list_it;

	    /**
	     * @brief Points the current hit of a search
	     *
	     * When the list is searched, this iterator points to the
	     * current hit of a search.
	     */
	    list_it cur_search_hit;

	    /**
	     * @brief Holds the last search term
	     *
	     * Holds the last search term used.
	     */
	    std::string last_search_term;

	    inline ListWidget(const ListWidget& lw) {}
	    inline const ListWidget& operator=(const ListWidget& lw) {
		return *this; }

	    /**
	     * @brief Validates the given iterator against the list
	     *
	     * Indicates whether or not the given iterator is still
	     * valid for the list
	     *
	     * @param it the iterator to be validated against \c
	     * itemlist.
	     *
	     * @retval a positive value (including zero) to indicate
	     * the position of the iterator, a negative value
	     * otherwise if the iterator is not valid.
	     */
	    l_size_type validateIterator(list_it& it) {
		l_size_type pos;
		list_it itit = itemlist.begin();

		for (pos = 0; itit != itemlist.end(); pos++, itit++ )
		    if (itit == it)
			return pos;

		return -1;
	    }

	    l_size_type validateIterator(c_list_it& it) const {
		l_size_type pos;
		c_list_it itit = itemlist.begin();

		for (pos = 0; itit != itemlist.end(); pos++, itit++ )
		    if (itit == it)
			return pos;

		return -1;
	    }


	    void highlightItemIter(list_it& it) {
		    l_size_type pos = validateIterator(it);

		    if (pos < 0) return;

		    if ( (pos/pagesize()) > 0) {
			start_pos=pos;
			cur_pos=0;
		    } else {
			start_pos = 0;
			cur_pos = pos;
		    }

		    showListItems();
		    showSelected(-1);
	    }

	    /**
	     * @brief Sets the border depending on the focus.
	     *
	     * Sets the border depending on whether or not the list
	     * has the focus.
	     *
	     * @retval the return value of the call to wborder().
	     */
	    int setBorder() const {
		if (hasfocus)
		    return box(window,0,0);
		else
		    return wborder(window, '|', '|', '-', '-', '+', '+', '+', '+');
	    }

	    int pagesize() { return height-2; }

	    void clearWin() throw(UIException) {
		Colors::setcolor(window, LISTWIDGET);
		int retval = wclear(window);
		if (retval == ERR)
		    throw UIException(_("Error clearing window"));

		retval = setBorder();
		if (retval == ERR)
		    throw UIException(_("Error drawing box around window"));
	    }

	    void showScrollIndicators() throw(UIException) {
		if (start_pos > 0) {
		    int retval = mvwaddch(window,
					  1,
					  width - 1,
					  '^');
		    if (retval == ERR)
			throw UIException(_("Unable to display scroll up indicator"));
		}

		if ( (itemlist.size() - 1)  > start_pos + cur_pos &&
		     itemlist.size() > pagesize()) {
		    int retval = mvwaddch(window,
					  height - 2,
					  width - 1,
					  'v');
		    if (retval == ERR)
			throw UIException(_("Unable to display scroll down indicator"));
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
			throw UIException(_("Unable to display item"));
		}

		showScrollIndicators();
	    }

	    /**
	     * @brief Highlights the selected item
	     *
	     * Highlights the selected item in the list. However, this
	     * is depending of the global member \c hasfocus. If \c
	     * hasfocus is \c false, the highlight is removed.
	     *
	     * @param old_pos tells the method the position of the old
	     * highlight, so it can be removed. If the value is less
	     * than \c 0, it does not try to clear the old highlight.
	     */
	    void showSelected(int old_pos) throw(UIException) {
		int retval = 0;

		if (itemlist.size() > 0) {
		    if (hasfocus)
			retval = mymvwchgat(window,
					    cur_pos + 1,
					    1,
					    width-2,
					    A_REVERSE,
					    Colors::getcolor(LISTWIDGET),
					    NULL);
		    else
			retval = mymvwchgat(window,
					    cur_pos + 1,
					    1,
					    width-2,
					    A_NORMAL,
					    Colors::getcolor(LISTWIDGET),
					    NULL);
		    if (retval == ERR)
			throw UIException(_("Error displaying cursor"));

		}

		if (old_pos > -1) {
		    // Reset the old position to 'normal' attribute
		    retval = mymvwchgat(window,
				      old_pos + 1,
				      1,
				      width-2,
				      A_NORMAL,
				      Colors::getcolor(LISTWIDGET),
				      NULL);
		    if (retval == ERR)
			throw UIException(_("Error move cursor"));
		}

		retval = touchwin(window);
		if (retval == ERR)
		    throw UIException(_("Error touching window"));

		retval = wrefresh(window);
		if (retval == ERR)
		    throw UIException(_("Error refreshing window"));
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
		    throw UIException(_("Error creating list window"));

		Colors::setcolor(window, LISTWIDGET);

		int retval = keypad(window, true);
		if (retval == ERR)
		    throw UIException(_("Error enabling keypad"));

		retval = setBorder();
		if (retval == ERR)
		    throw UIException(_("Error re-setting the border"));

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
				     hasfocus(false),
				     start_pos(0),
				     cur_pos(0),
				     itemlist(l),
				     cur_search_hit(itemlist.end()),
				     last_search_term("") {
		if ( sx == -1 ||
		     sy == -1 ||
		     width == -1 ||
		     height == -1 )
		    throw UIException(_("No idea of the dimension of the list"));

		setSortOrder(ASCENDING);

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
	     * @param l the list holding the items to be
	     * displayed. The items of the list are expected to have a
	     * method called \c c_str() for getting their names. Empty
	     * lists are allowed.
	     */
	    void setList(typename std::list<T>& l) {
		itemlist = l;
		start_pos = 0;
		cur_pos = 0;
		setSortOrder(this->sortorder);
		refresh();
	    }

	    /**
	     * @brief Replace the item at the current position
	     * selected.
	     *
	     * Replaces the item at the current position of the list
	     * selected by the user.
	     *
	     * @param item the new item.
	     */
	    void replaceCurrentItem(T& item) {
		typename std::list<T>::iterator itemlist_pos = itemlist.begin();
		for (int i=0;
		     i<(start_pos + cur_pos) && itemlist_pos != itemlist.end();
		     itemlist_pos++, i++);

		*itemlist_pos = item;
		setSortOrder(this->sortorder);
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
	     * Focus the widget and shows it on the screen. The widget
	     * handles the following key strokes:
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
		hasfocus = true;

		int retval = setBorder();
		if (retval == ERR)
		    throw UIException(_("Error setting the border of window"));

		showScrollIndicators();
		showSelected(-1);

		retval = wrefresh(window);
		if (retval == ERR)
		    throw UIException(_("Error refreshing the list widget"));

		int ch = 0;
		while (hasfocus) {
		    ch = wgetch(window);
		    switch (ch) {
		    case KEY_UP:
		    case 'k': // vi key
			scrollUp();
			break;
		    case KEY_DOWN:
		    case 'j': // vi key
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
			hasfocus = false;
			break;
		    }
		}

		showSelected(-1);

		retval = setBorder();
		if (retval == ERR)
		    throw UIException(_("Error re-setting the border"));
		retval = wrefresh(window);
		if (retval == ERR)
		    throw UIException(_("Error refreshing the list widget"));

		return ch;
	    }

	    void refresh() throw(UIException) {
		showListItems();
		showSelected(-1);

		int retval = wrefresh(window);
		if (retval == ERR)
		    throw UIException(_("Error refreshing list"));
	    }

	    void resize(int sx, int sy, int w, int h) throw(UIException) {
		int retval = wclear(window);
		if (retval == ERR)
		    throw UIException(_("Error clearing list"));

		retval = wrefresh(window);
		if (retval == ERR)
		    throw UIException(_("Error refreshing window"));

		retval = delwin(window);
		if (retval == ERR)
		    throw UIException(_("Error deleting window"));

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

	    /**
	     * @brief Returns the sort order applied to the list
	     *
	     * Returns the current sort order that is applied to the list.
	     *
	     * @return \c SortOrder value.
	     */
	    SortOrder getSortOrder() const { return sortorder; }

	    /**
	     * @brief Sorts the list
	     *
	     * Sorts the list using the given order. It expects that
	     * \c T has defined the less than operator.
	     *
	     * @param so value of the type \c SortOrder
	     */
	    void setSortOrder(SortOrder so) {
		itemlist.sort();
		sortorder = so;
		switch (sortorder) {
		case ASCENDING:
		    break;
		case DESCENDING:
		    std::reverse(itemlist.begin(),itemlist.end());
		    break;
		}
	    }

	    /**
	     * @brief Sorts the list with the currently set sort order
	     *
	     * Sorts the list using the currently set sort order.
	     *
	     * @sa setSortOrder
	     */
	    void setSortOrder() {
		setSortOrder(getSortOrder());
	    }

	    /**
	     * @brief Searches for a term in the list items
	     *
	     * Searches for a given term in the list items.
	     *
	     * @param t the term to search for
	     *
	     * @retval \c true if the term was found, else \c false.
	     */
	    bool searchTerm(const char* t) {
		last_search_term = t;
		cur_search_hit = std::find_if(itemlist.begin(),
					      itemlist.end(),
					      ItemContains(t));

		if (cur_search_hit != itemlist.end()) {
		    highlightItemIter(cur_search_hit);
		    return true;
		}

		return false;
	    }

	    /**
	     * @brief Searches again using the last search term used.
	     *
	     * Performs a search again using the last search term again.
	     *
	     * @retval \c true if the term was found again, else \c false
	     */
	    bool searchNext() {
		if (validateIterator(cur_search_hit) < 0 ||
		    last_search_term.empty() )
		    return false;

		// Here make the search start at the beginning, or
		// continue at the next item
		if (cur_search_hit == itemlist.end() ) {
		    cur_search_hit = itemlist.begin();
		} else {
		    // Advance to the next item. Else we would have a
		    // hit on the item pointed to by cur_search_hit...
		    cur_search_hit++;
		}

		// Indicate that we reached the end of the list. Upon
		// the next call of this method, cur_search_hit will
		// be set to the beginning of the list and the search
		// starts from the top again
		if (cur_search_hit == itemlist.end()) return false;

		cur_search_hit = std::find_if(cur_search_hit,
					      itemlist.end(),
					      ItemContains(last_search_term.c_str()));

		if (cur_search_hit != itemlist.end()) {
		    highlightItemIter(cur_search_hit);
		    return true;
		}

		return false;

	    }

    };

}
#endif // _LISTWIDGET_H
