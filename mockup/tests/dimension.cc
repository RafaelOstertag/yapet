// $Id$
//
// Test Dimension class

#include "dimension.h"


int main() {
    Dimension d(24,79,1,1);
    Margin m(1,1,1,1);

    Dimension r = d-m;

    if (r.getLines() != 23 ||
	r.getCols() != 78 ||
	r.getY() != 2 ||
	r.getX() != 2 )
	return 1;

    return 0;
}
