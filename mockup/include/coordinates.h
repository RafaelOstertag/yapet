// -*- mode: c++ -*-
//
// $Id$

#ifndef COORDINATES_H
#define COORDINATES_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

template<class T=int>
class Coordinates {
    private:
	T x, y;

    public:
	Coordinates() : x(0), y(0) {}
	Coordinates(T _x, T _y) : x(_x), y(_y) {}
	Coordinates(const Coordinates<T>& _c) {
	    x = _c.x;
	    y = _c.y;
	}
	virtual ~Coordinates() {}
	
	T getX() const { return x; }
	T getY() const { return y; }
	void setX(T _x) { x = _x; }
	void setY(T _y) { y = _y; }
	
	Coordinates<T>& operator=(const Coordinates<T>& rhs) {
	    x = rhs.x;
	    y = rhs.y;
	    return *this;
	}
	
	Coordinates<T>& operator+=(const Coordinates<T>& rhs) {
	    x += rhs.x;
	    y += rhs.y;
	    return *this;
	}

	Coordinates<T>& operator+=(const T& rhs) {
	    x += rhs;
	    y += rhs;
	    return *this;
	}
	
	Coordinates<T>& operator-=(const Coordinates<T>& rhs) {
	    x -= rhs.x;
	    y -= rhs.y;
	    return *this;
	}

	Coordinates<T>& operator-=(const T& rhs) {
	    x -= rhs;
	    y -= rhs;
	    return *this;
	}
	
	Coordinates<T> operator+(const Coordinates<T>& rhs) const {
	    Coordinates<T> tmp = *this;
	    return tmp+=rhs;
	}

	Coordinates<T> operator+(const T& rhs) const {
	    Coordinates<T> tmp = *this;
	    return tmp+=rhs;
	}

	Coordinates<T> operator-(const Coordinates<T>& rhs) const {
	    Coordinates<T> tmp = *this;
	    return tmp-=rhs;
	}

	Coordinates<T> operator-(const T& rhs) const {
	    Coordinates<T> tmp = *this;
	    return tmp-=rhs;
	}
	
	bool operator==(const Coordinates<T>& rhs) const {
	    return x == rhs.x && y == rhs.y;
	}

	bool operator!=(const Coordinates<T>& rhs) const {
	    return !operator==(rhs);
	}
	
};

#endif // COORDINATES_H
