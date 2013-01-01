// $Id$
//
// Test Dimension class

#include "rectangle.h"

int main() {
    Rectangle<> o(0,0,25,80);
    Margin<> m(1,2,3,5);

    Rectangle<> r = o-m;

    if (r.getLines() != 21 ||
	r.getCols() != 73 ||
	r.getY() != 1 ||
	r.getX() != 2 )
	return 1;

    o = Rectangle<>(2,2,5,5);
    m = Margin<>(1,1,0,0);
    r = o-m;

    if (r.getLines() != 4 &&
	r.getCols() != 4)
	return 1;

    Rectangle<> o1(1,2,3,5);
    Rectangle<> o2(7,11,13,17);

    if (o1 == o2)
	return 1;

    if (o1 != o1)
	return 1;

    if (!(o1 == o1))
	return 1;

    o1 = o2;

    if (o1 != o2)
	return 1;

    if (!(o1 == o2))
	return 1;

    if (o1.getLines() != 13 ||
	o1.getCols() != 17)
	return 1;

    o1.setLines(25);
    if (o1.getLines() != 25 ||
	o1.getCols() != 17 )
	return 1;

    o1.setCols(80);
    if (o1.getLines() != 25 ||
	o1.getCols() != 80)
	return 1;

    o1.setCols(80);
    if (o1.getLines() != 25 ||
	o1.getCols() != 80)
	return 1;

    o1.setLines(25);
    if (o1.getLines() != 25 ||
	o1.getCols() != 80 )
	return 1;



    return 0;
}
