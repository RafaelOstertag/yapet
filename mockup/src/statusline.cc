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
    setLine(messages.top());
}


//
// Protected
//

//
// Public
//

void
StatusLine::pushMsg(const std::string& m) {
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
