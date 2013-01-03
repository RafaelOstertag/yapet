// -*- mode: c++ -*-
//
// $Id$

#ifndef LINEOBJECT_H
#define LINEOBJECT_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#ifdef HAVE_STRING
#include <string>
#endif // HAVE_STRING

#include "screenobject.h"


/**
 * A line object occupies one entire line of the screen.
 */
class LineObject: public ScreenObject {
    public:
	enum POSITION {
	    POS_TOP,
	    POS_BOTTOM,
	    POS_ABSOLUTE
	};

    private:
	std::string line;
	POSITION pos;
	/// Only used when in absolut positioning mode
	Coordinates<>* coords;

    protected:
	void putLine();

    public:
	LineObject(POSITION _pos, const Coordinates<>* _coords);
	LineObject(POSITION _pos, const Coordinates<>* _coords, const std::string& _str);
	LineObject(POSITION _pos, const Coordinates<>* _coords, const char* _str);
	~LineObject();
	LineObject(const LineObject& lo);
	LineObject& operator=(const LineObject& lo);

	void realize(const Rectangle<>& r);

	void setLine(const std::string& _str);
	inline std::string getLine() const { return line; }
};

#endif // LINEOBJECT_H
