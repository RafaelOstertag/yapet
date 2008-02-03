// -*- c++ -*-
//
// $Id$

#ifndef _INPUTWIDGET_H
#define _INPUTWIDGET_H

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

#include "uiexception.h"
#include "secstring.h"

namespace GPSUI {
    class InputWidget {
	private:
	    WINDOW* window;
	    secstring buffer;
	    
	    int max_length;
	    int start_pos;
	    int pos;
	    int width;
	    bool text_changed;
	    
	    
	    inline InputWidget(const InputWidget&) {}
	    inline const InputWidget& operator=(const InputWidget&) { return *this; }
	    
	    void moveBackward() throw(UIException);
	    void moveForward() throw(UIException);
	    void moveHome() throw(UIException);
	    void moveEnd() throw(UIException);
	    void processBackspace() throw(UIException);
	    void processDelete() throw(UIException);
	    void processInput(int ch) throw(UIException);	
	    
	protected:
	    void createWindow(int sx, int sy, int w) throw(UIException);
	    inline const WINDOW* getWindow() const { return window; }
	    inline WINDOW* getWindow() { return window; }
	    inline int getStartPos() const { return start_pos; }
	    inline int getPos() const { return pos; }
	    inline int getWidth() const { return width; }
	    inline secstring& getBuffer() { return buffer; }
	    inline const secstring& getBuffer() const { return buffer; }
	    
	public:
	    InputWidget(int sx, int sy, int w, int ml = 512) throw(UIException);
	    virtual ~InputWidget();
	    
	    int focus() throw(UIException);
	    virtual void refresh() throw(UIException);
	    void resize(int sx, int sy, int w) throw(UIException);
	    void setText(secstring t) throw(UIException);
	    inline secstring getText() const { return buffer; }
	    void clearText();
	    bool isTextChanged() const { return text_changed; }
    };
    
}

#endif // _INPUTWIDGET_H
