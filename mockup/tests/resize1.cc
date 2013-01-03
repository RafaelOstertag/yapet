// $Id$
//
// Test basic functionality of Curses class
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_UNISTD_H
#include <unistd.h>
#endif // HAVE_UNISTD_H

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif // HAVE_SYS_TYPES_H

#ifdef HAVE_STROPTS_H
#include <stropts.h>
#endif // HAVE_STROPTS_H

#ifdef HAVE_SYS_TERMIOS_H
#include <sys/termios.h>
#endif // HAVE_SYS_TERMIOS_H

#ifdef HAVE_IOSTREAM
#include <iostream>
#endif // HAVE_IOSTREAM

#include "curs.h"
#include "lineobject.h"
#include "statusline.h"
#include "window.h"

int main() {
    winsize ws, wsave;
    if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &ws) == -1) {
	return 1;
    }

    wsave = ws;

    try {
	Curses::init();

	LineObject* title = new LineObject(LineObject::POS_TOP,
					   NULL, "Resize 1");
	Curses::setTitle(title);

	Window* w1 = new Window;
	w1->setFrame(true);

	Curses::setWindow(w1);

	StatusLine* sl = new StatusLine();
	Curses::setStatusLine(sl);


	Curses::getStatusLine()->pushMsg("Status 1");
	Curses::getStatusLine()->pushMsg("Status 2");
	Curses::show();

	int line = ws.ws_row;
	for (; ws.ws_col>10; ws.ws_col--) {
	    for (ws.ws_row = line; ws.ws_row>10; ws.ws_row--) {
		if (ioctl(STDIN_FILENO, TIOCSWINSZ, &ws) == -1) {
		    return 1;
		}

		char buff[10];
		sprintf(buff,"%d", ws.ws_col);
		std::string tmp1 = buff;
		sprintf(buff,"%d", ws.ws_row);
		std::string tmp2 = buff;

		std::string msg = tmp1 + ":" + tmp2;
		Curses::getStatusLine()->pushMsg(msg);
	    }
	}

	line = ws.ws_row;
	for (; ws.ws_col<=wsave.ws_col; ws.ws_col++) {
	    for ( ws.ws_row = line; ws.ws_row<=wsave.ws_row; ws.ws_row++) {
		if (ioctl(STDIN_FILENO, TIOCSWINSZ, &ws) == -1) {
		    return 1;
		}

		char buff[10];
		sprintf(buff,"%d", ws.ws_col);
		std::string tmp1 = buff;
		sprintf(buff,"%d", ws.ws_row);
		std::string tmp2 = buff;

		std::string msg = tmp1 + ":" + tmp2;
		Curses::getStatusLine()->pushMsg(msg);
	    }
	}

	delete title;
	delete w1;
	delete sl;
	Curses::end();
    } catch (std::exception& e) {
	std::cerr << e.what() << std::endl;
	sleep(2);
	goto _ERR;
    }

    if (ioctl(STDIN_FILENO, TIOCSWINSZ, &wsave) == -1) {
	return 1;
    }

    return 0;
 _ERR:
    Curses::end();
    if (ioctl(STDIN_FILENO, TIOCSWINSZ, &wsave) == -1) {
	return 1;
    }
    return 1;
}
