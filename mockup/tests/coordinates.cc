// $Id$
//
// Test Dimension class

#include "rectangle.h"

int main() {
    Coordinates<> o1(1,2);
    Coordinates<> o2(3,5);

    Coordinates<> r = o1-o2;

    if ( r.getX() != -2 ||
	 r.getY() != -3)
	return 1;

    r+=1;

    if ( r.getX() != -1 ||
	 r.getY() != -2 )
	return 1;

    if ( o1 == o2 )
	return 1;

    r-=1;

    if ( r != o1-o2 )
	return 1;

    if (!(r == o1-o2 ))
	return 1;

    o1 = o2;

    if ( o1 != o2 )
	return 1;

    if (!(o1 == o2))
	return 1;
	 
    o1 = r;

    if (o1 == o2)
	return 1;

    if (o1 != o1)
	return 1;

    if (!(o1 == o1))
	return 1;

    o1.setX(2);
    o1.setY(3);

    if ( o1.getX() != 2 ||
	 o1.getY() != 3 )
	 return 1;

    o2.setY(5);
    o2.setX(7);

    if ( o2.getX() != 7 ||
	 o2.getY() != 5 )
	 return 1;

    o1+=o2;

    if ( o1.getX() != 9 ||
	 o1.getY() != 8)
	return 1;

    if ( o2.getX() != 7 ||
	 o2.getY() != 5 )
	 return 1;

    o1 = o2 - Coordinates<>(2,3);

    if ( o1.getX() != 5 ||
	 o1.getY() != 2 )
	 return 1;

    if ( o2.getX() != 7 ||
	 o2.getY() != 5 )
	 return 1;
    return 0;
}
