// $Id$

#include "dialogbox.h"

using namespace GPSUI;

DialogBox::DialogBox(std::string t, std::string m) throw(UIException) : MessageBox(t, m),
									cancelbutton(NULL),
									answer(ANSWER_CANCEL) {
    cancelbutton = new Button("Cancel", getStartX() + 2 + getOkButtonLength(), getStartY() + getBaseHeight() -2);
    cancelbutton->refresh();
									}

DialogBox::~DialogBox() {
    delete cancelbutton;
}

int
DialogBox::run() throw(UIException) {
    while (true) {
	int ch = MessageBox::run();
	if (ch == '\n') {
	    answer = ANSWER_OK;
	    return ch;
	}

	ch = cancelbutton->focus();
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
    cancelbutton->refresh();
}

void
DialogBox::refresh() throw(UIException) {
    MessageBox::refresh();
    cancelbutton->refresh();
}

