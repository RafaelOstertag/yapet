// -*- mode: c++ -*-
//
// $Id$

#ifndef STATUSLINE_H
#define STATUSLINE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <stack>

#include "screenobject.h"

class StatusLine: public ScreenObject {
    private:
	std::stack<std::string> messages;
	
	void putTopMsg();

    public:
	StatusLine() : ScreenObject() { resize(); }
	StatusLine(const StatusLine& sl) : ScreenObject(sl) {}
	void pushMsg(const std::string& m);
	void pushMsg(const char* m);
	void popMsg();
	void resize();
};

#endif
