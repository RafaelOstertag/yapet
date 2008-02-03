// -*- c++ -*-
//
// $Id$

#ifndef _PASSWORDRECORD_H
#define _PASSWORDRECORD_H

#ifdef HAVE_CONFIG_H
#include <config.h>
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

#include <string>

#include <key.h>
#include <partdec.h>

#include <resizeable.h>
#include <scrdim.h>
#include <button.h>
#include <passwordwidget.h>

class PasswordRecord : protected GPSUI::Resizeable,
		      protected GPSUI::ScrDim {
    private:
	enum {
	    HEIGHT = 14
	};

	WINDOW* window;
	GPSUI::InputWidget* name;
	GPSUI::InputWidget* host;
	GPSUI::InputWidget* username;
	GPSUI::InputWidget* password;
	GPSUI::InputWidget* comment;
	GPSUI::Button* okbutton;
	GPSUI::Button* cancelbutton;
	GPSAFE::Key* key;
	GPSAFE::PartDec* encentry;

	inline PasswordRecord(const PasswordRecord&) {}
	inline const PasswordRecord& operator=(const PasswordRecord&) { return *this; }

	inline int getWidth() const {
	    return maxX() - 8;
	}

	inline int getHeight() const {
	    return HEIGHT;
	}

	inline int getStartX() const {
	    return maxX()/2 - getWidth()/2;
	}

	inline int getStartY() const {
	    return maxY()/2 - getHeight()/2;
	}
	    
	void createWindow() throw(GPSUI::UIException);

    public:
	PasswordRecord(GPSAFE::Key& k, GPSAFE::PartDec* pe) throw(GPSUI::UIException);
	~PasswordRecord();

	void run() throw(GPSUI::UIException);
	void resize() throw(GPSUI::UIException);
	void refresh() throw(GPSUI::UIException);
	/**
	 * Caller is responsible for freeing the pointer returned
	 */
	inline GPSAFE::PartDec* getEncEntry() const { return encentry; }
	inline bool entryChanged() const {
	    return name->isTextChanged() ||
		host->isTextChanged() ||
		username->isTextChanged() ||
		password->isTextChanged() ||
		comment->isTextChanged();
	}
		
};

#endif // _PASSWORDRECORD_H
