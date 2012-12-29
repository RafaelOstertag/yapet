// -*- mode: c++ -*-

#ifndef DIMENSION_H
#define DIMENSION_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "curs.h"
#include "margin.h"

class Dimension {
    private:
	int nlines, ncols, y, x;

    public:
	inline Dimension(int nl, int nc, int _y, int _x) :
	    nlines(nl), ncols(nc), y(_y), x(_x) {}

	inline Dimension() {
	    Dimension::Dimension(Curses::getDimension());
	}

	inline Dimension(const Dimension& d) {
	    nlines = d.nlines;
	    ncols = d.ncols;
	    y = d.y;
	    x = d.x;
	}

	inline ~Dimension() {}

	inline void setLines(int l) { nlines = l; }
	inline void setCols(int c)  { ncols = c; }
	inline void setY(int _y)  { y = _y; }
	inline void setX(int _x)  { x = _x; }

	inline int getLines() const { return nlines; }
	inline int getCols() const { return ncols; }
	inline int getY() const { return y; }
	inline int getX() const { return x; }

	inline Dimension& operator=(const Dimension &d) {
	    if (this == &d) return *this;

	    nlines = d.nlines;
	    ncols = d.ncols;
	    y = d.y;
	    x = d.x;

	    return *this;
	}

	inline bool operator==(const Dimension& d) {
	    if (this == &d) return true;

	    return nlines == d.nlines && ncols == d.ncols &&
		y == d.y && x == d.x;
	}

	inline Dimension& operator-=(const Margin& rhs) {
	    nlines -= rhs.bottom;
	    ncols -= rhs.right;
	    y += rhs.top;
	    x += rhs.left;
	    return *this;
	}

	inline Dimension operator-(const Margin& rhs) {
	    Dimension tmp = *this;
	    return tmp-=rhs;
	}

};

#endif
