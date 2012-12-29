// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "statusline.h"


//
// Private
//

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
#warning "Not Implemented"
    abort();
}

void
StatusLine::popMsg() {
#warning "Not Implemented"
    abort();
}
    
void
StatusLine::refresh() {
#warning "Not Implemented"
    abort();
}

void
StatusLine::resize() {
#warning "Not Implemented"
    abort();
}
