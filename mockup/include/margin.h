// -*- mode: c++ -*-

#ifndef MARGIN_H
#define MARGIN_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

template<class T=int>
class Margin {
    private:
	T top, bottom, left, right;

    protected:
	T max(T a, T b) const { return a>b ? a : b; }
	T min(T a, T b) const { return a<b ? a : b; }

    public:
	Margin(T t, T l, T b, T r) :
	    top(t), bottom(b), left(l), right(r) {}

	Margin():
	    top(0), bottom(0), left(0), right(0) {}

	Margin(const Margin<>& m) {
	    top = m.top;
	    bottom = m.bottom;
	    left = m.left;
	    right = m.right;
	}

	~Margin() {}
	    
	void setTop(T i) { top = i; }
	void setBottom(T i) { bottom = i; }
	void setLeft(T i) { left = i; }
	void setRight(T i) { right = i; }

	T getTop() const { return top; }
	T getBottom() const { return bottom; }
	T getLeft() const { return left; }
	T getRight() const { return right; }

	Margin<T>& operator=(const Margin<T>& m) {
	    if (this == &m) return *this;

	    top = m.top;
	    bottom = m.bottom;
	    left = m.left;
	    right = m.right;
	    return *this;
	}

	Margin<T> operator+(const Margin<T>& m) const {
	    Margin<T> tmp = *this;

	    tmp.top = max(top, m.top);
	    tmp.bottom = max(bottom, m.bottom);
	    tmp.left = max(left, m.left);
	    tmp.right = max(right, m.right);

	    return tmp;
	}

	Margin<T> operator-(const Margin<T>& m) const {
	    Margin<T> tmp = *this;

	    tmp.top = min(top, m.top);
	    tmp.bottom = min(bottom, m.bottom);
	    tmp.left = min(left, m.left);
	    tmp.right = min(right, m.right);

	    return tmp;
	}

	bool operator==(const Margin<T>& m) const {
	    if (this == &m) return true;

	    return top == m.top && bottom == m.bottom &&
		left == m.left && right == m.right;
	}

	bool operator!=(const Margin<T>& m) const {
	    return ! operator==(m);
	}
};

#endif
