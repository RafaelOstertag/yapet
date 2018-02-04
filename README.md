YAPET is a text based password manager using the Blowfish encryption
algorithm to store passwords and associated information encrypted on
disk. Its primary aim is to provide a safe way to store passwords in a
file on disk while having a small footprint, and compiling and running
under today's most popular Unix Systems.

Build Primer
===

1. Clone the repository
1. Switch to the development branch
```
git checkout development
```

2. Fetch the libyacurs submodule

    git submodule init
    git submodule update

3. Create stub `README` and `ChangeLog` files

    touch README ChangeLog libyacurs/ChangeLog

4. Run `autoreconf`

    autoreconf -I m4 -i

5. Create the build directory

    mkdir obj && cd obj

6. Configure and build

    ../configure

7. Create the man pages and `README` file

    cd doc && gmake -f Makefile.doc

8. Build YAPET

    cd .. && gmake
	
Bootstrapping the project is cumbersome because I don't keep the
`README` and man pages in the repository. They are built
from Docbook sources in the `doc/` directory. The `ChangeLog` is
pulled from the VCS.