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
	WINDOW
	std::stack<std::string> messages;

    public:
	void pushMsg(const std::string& m);
	void pushMsg(const char* m);
	void popMsg();
	void refresh();
	void resize();
};

#endif
