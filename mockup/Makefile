# $Id$

CPPFLAGS	= -I/usr/xpg4/include -Iinclude/
LDFLAGS		= -L/usr/xpg4/lib -R/usr/xpg4/lib -lcurses
OBJ	= mockup.o curs.o window.o

all: mockup

clean:
	rm -f *.o
	rm -f mockup
	rm -f core
	rm -f a.out

mockup: $(OBJ)
	$(CCC) $(LDFLAGS) -o $@ $(OBJ)
