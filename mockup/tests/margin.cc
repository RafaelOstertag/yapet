// $Id$
//
// Tests margin class

#include <iostream>

#include "margin.h"

int main() {
    Margin m1(2,3,5,7);
    Margin m2(11,13,17,19);

    Margin r = m1 + m2;

    if ( r.getTop() != 11 ||
	 r.getBottom() != 13 ||
	 r.getLeft() != 17 ||
	 r.getRight() != 19 )
	return 1;

    r = m1 - m2;

    if ( r.getTop() != 2 ||
	 r.getBottom() != 3 ||
	 r.getLeft() != 5 ||
	 r.getRight() != 7 )
	return 1;

    return 0;
}
