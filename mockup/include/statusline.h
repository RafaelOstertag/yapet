// -*- mode: c++ -*-
//
// $Id$

#ifndef STATUSLINE_H
#define STATUSLINE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_STACK
#include <stack>
#endif // HAVE_STACK

#include "lineobject.h"

class StatusLine: public LineObject {
    private:
	std::stack<std::string> messages;
	
	void putTopMsg();

    public:
	inline StatusLine() : LineObject(POS_BOTTOM, NULL) {}
	inline StatusLine(const StatusLine& sl) : LineObject(sl) {}
	void pushMsg(const std::string& m);
	void popMsg();
};

#endif
