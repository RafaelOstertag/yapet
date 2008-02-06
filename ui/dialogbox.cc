// $Id$

#include "dialogbox.h"

using namespace GPSUI;

DialogBox::DialogBox(std::string t, std::string m) throw(UIException) : MessageBox(t, m),
									cancelbutton(NULL),
									answer(ANSWER_CANCEL) {
    cancelbutton = new Button("Cancel", getStartX() + 2 + getOkButtonLength(), getStartY() + getBaseHeight() -2);
}

DialogBox::~DialogBox() {
    delete cancelbutton;
}

int
DialogBox::run() throw(UIException) {
    refresh();
    while (true) {
#ifdef HAVE_WRESIZE
	int ch;
	while ( (ch = MessageBox::run()) == KEY_RESIZE ) 
	    Resizeable::resizeAll();
#else // HAVE_RESIZE
	int ch = MessageBox::run();
#endif // HAVE_RESIZE
	if (ch == '\n') {
	    answer = ANSWER_OK;
	    return ch;
	}

#ifdef HAVE_WRESIZE
	while ( (ch = cancelbutton->focus()) == KEY_RESIZE )
	    Resizeable::resizeAll();
#else // HAVE_RESIZE
	ch = cancelbutton->focus();
#endif // HAVE_RESIZE
	if (ch == '\n') {
	    answer = ANSWER_CANCEL;
	    return ch;
	}
    }
}

void
DialogBox::resize() throw(UIException) {
    MessageBox::resize();
    delete cancelbutton;
    cancelbutton = new Button("Cancel", getStartX() + 2 + getOkButtonLength(), getStartY() + getBaseHeight() -2);
}

void
DialogBox::refresh() throw(UIException) {
    MessageBox::refresh();
    cancelbutton->refresh();
}
