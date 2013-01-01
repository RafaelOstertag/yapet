// -*- mode: c++ -*-

#ifndef RECTANGLE_H
#define RECTANGLE_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "coordinates.h"
#include "margin.h"

template<class T=int>
class Rectangle: public Coordinates<T> {
    private:
	Coordinates<T> c2;

    public:
	Rectangle() : Coordinates<T>(), c2() {}
	Rectangle(int _y1, int _x1, int _nlines, int _ncols) :
	    Coordinates<T>(_x1, _y1),
	    c2(_x1 + _ncols - 1, /* Transform to coords */
	       _y1 + _nlines - 1 /* Transform to coords */)  {}

	Rectangle(const Coordinates<T>& c) :
	    Coordinates<T>(c), c2(0,0) {}

	Rectangle(const Rectangle<T>& d) : Coordinates<T>(d) {
	    c2 = d.c2;
	}

	~Rectangle() {}

	void setLines(T l) { c2.setY(this->getY() + l-1); }
	void setCols(T c)  { c2.setX(this->getX() + c-1); }

	T getLines() const { return (c2 - *this).getY() + 1; }
	T getCols() const { return (c2 - *this).getX() + 1; }

	Rectangle<T>& operator=(const Rectangle<T> &d) {
	    if (this == &d) return *this;

	    c2 = d.c2;
	    Coordinates<T>::operator=(d);

	    return *this;
	}

	bool operator==(const Rectangle<T>& d) const {
	    if (this == &d) return true;

	    return Coordinates<T>::operator==(*this) && c2 == d.c2;
	}

	bool operator!=(const Rectangle<T>& d) const {
	    return ! operator==(d);
	}

	bool operator==(const Coordinates<T>& d) const {
	    return false;
	}

	Rectangle<T>& operator-=(const Margin<T>& rhs) {
	    c2.setY(c2.getY() - rhs.getBottom());
	    c2.setX(c2.getX() - rhs.getRight());
	    this->setY(this->getY() + rhs.getTop());
	    this->setX(this->getX() + rhs.getLeft());
	    return *this;
	}

	Rectangle<T> operator-(const Margin<T>& rhs) const {
	    Rectangle<T> tmp = *this;
	    return tmp-=rhs;
	}

};

#endif
