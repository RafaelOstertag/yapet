// $Id$
//
// Tests margin class

#include <iostream>

#include "margin.h"

int main() {
    Margin<> m1(2,3,5,7);
    Margin<> m2(11,13,17,19);

    Margin<> r = m1 + m2;

    if ( r.getTop() != 11 ||
	 r.getLeft() != 13 ||
	 r.getBottom() != 17 ||
	 r.getRight() != 19 )
	return 1;

    r = m1 - m2;

    if ( r.getTop() != 2 ||
	 r.getLeft() != 3 ||
	 r.getBottom() != 5 ||
	 r.getRight() != 7 )
	return 1;

    if ( r != m1 - m2)
	return 1;

    if (!(r == m1 - m2))
	return 1;

    m1 = m2;

    if ( m1 != m2 )
	return 1;

    if (!(m1 == m2))
	return 1;

    return 0;
}
