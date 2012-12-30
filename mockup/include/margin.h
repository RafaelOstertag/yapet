// -*- mode: c++ -*-

#ifndef MARGIN_H
#define MARGIN_H

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

class Margin {
    private:
	int top, bottom, left, right;

    protected:
	inline int max(int a, int b) { return a>b ? a : b; }
	inline int min(int a, int b) { return a<b ? a : b; }

    public:
	inline Margin(int t, int b, int l, int r) :
	    top(t), bottom(b), left(l), right(r) {}

	inline Margin():
	    top(0), bottom(0), left(0), right(0) {}

	inline Margin(const Margin& m) {
	    top = m.top;
	    bottom = m.bottom;
	    left = m.left;
	    right = m.right;
	}

	inline ~Margin() {}
	    
	inline void setTop(int i) { top = i; }
	inline void setBottom(int i) { bottom = i; }
	inline void setLeft(int i) { left = i; }
	inline void setRight(int i) { right = i; }

	inline int getTop() const { return top; }
	inline int getBottom() const { return bottom; }
	inline int getLeft() const { return left; }
	inline int getRight() const { return right; }

	inline Margin& operator=(const Margin& m) {
	    if (this == &m) return *this;

	    top = m.top;
	    bottom = m.bottom;
	    left = m.left;
	    right = m.right;
	    return *this;
	}

	inline Margin operator+(const Margin& m) {
	    Margin tmp = *this;

	    tmp.top = max(top, m.top);
	    tmp.bottom = max(bottom, m.bottom);
	    tmp.left = max(left, m.left);
	    tmp.right = max(right, m.right);

	    return tmp;
	}

	inline Margin operator-(const Margin& m) {
	    Margin tmp = *this;

	    tmp.top = min(top, m.top);
	    tmp.bottom = min(bottom, m.bottom);
	    tmp.left = min(left, m.left);
	    tmp.right = min(right, m.right);

	    return tmp;
	}

	inline bool operator==(const Margin& m) {
	    if (this == &m) return true;

	    return top == m.top && bottom == m.bottom &&
		left == m.left && right == m.right;
	}

	friend class Dimension;
};

#endif
