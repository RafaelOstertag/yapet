// $Id$

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "curex.h"
#include "lineobject.h"

//
// Private
//

//
// Protected
//

void
LineObject::putLine() {
    if (!isRealized()) return;

    int retval = werase(getWindow());
    if (retval == ERR)
	throw EraseFailed();

    retval = mymvwaddstr(getWindow(),
			 0,0,
			 line.c_str());
    if (retval == ERR)
	throw AddStrFailed();
}

//
// Public
//

LineObject::LineObject(POSITION _pos, const Coordinates<>* _coords):
    ScreenObject(), line(), pos(_pos), coords(NULL) {
    if (_coords)
	coords = new Coordinates<>(*_coords);
}

LineObject::LineObject(POSITION _pos, const Coordinates<>* _coords, const std::string& _str):
    ScreenObject(), line(_str), pos(_pos), coords(NULL) {
    if (_coords)
	coords = new Coordinates<>(*_coords);
}

LineObject::LineObject(POSITION _pos, const Coordinates<>* _coords, const char* _str):
    ScreenObject(), line(_str), pos(_pos), coords(NULL) {
    if (_coords)
	coords = new Coordinates<>(*_coords);
}

LineObject::~LineObject() {
    if (coords)
	delete coords;
}

LineObject::LineObject(const LineObject& lo) : ScreenObject(lo) {
    line = lo.line;
    pos = lo.pos;
    if (lo.coords)
	coords = new Coordinates<>(*lo.coords);
    else
	coords=NULL;
}

LineObject&
LineObject::operator=(const LineObject& lo) {
    if (this == &lo) return *this;

    line = lo.line;
    pos = lo.pos;

    if (coords)
	delete coords;

    if (lo.coords)
	coords = new Coordinates<>(*lo.coords);
    else
	coords=NULL;

    return *this;
}

void
LineObject::realize(const Rectangle<>& r) {
    ScreenObject::realize(r);
    putLine();
}

void
LineObject::setLine(const std::string& _str) {
    line = _str;
    putLine();
}
