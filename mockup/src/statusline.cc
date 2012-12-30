// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "curs.h"
#include "curex.h"
#include "statusline.h"


//
// Private
//

void
StatusLine::putTopMsg() {
    int retval = werase(getWindow());
    if (retval == ERR)
	throw EraseFailed();

    retval = mymvwaddstr(getWindow(),
		       0,0, messages.top().c_str());
    if (retval == ERR)
	throw AddStrFailed();
}
    

//
// Protected
//

//
// Public
//

void
StatusLine::pushMsg(const std::string& m) {
    pushMsg(m.c_str());
}

void
StatusLine::pushMsg(const char* m) {
    messages.push(m);
    putTopMsg();
}

void
StatusLine::popMsg() {
    if (messages.empty()) return;
    messages.pop();
    if (messages.empty()) return;
    putTopMsg();
}
    
void
StatusLine::resize() {
    ScreenObject::resize(Curses::getStatusDimension());
}
