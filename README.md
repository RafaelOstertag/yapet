YAPET is a text based password manager using the AES-256 encryption
algorithm to store passwords and associated information encrypted on
disk. Its primary aim is to provide a safe way to store passwords in a
file on disk while having a small footprint, and compiling and running
under today's most popular Unix Systems.

Build Primer
===

1. Clone the repository
2. Switch to the development branch
```
git checkout development
```
3. Fetch the libyacurs submodule
```
git submodule init
git submodule update
```
4. Create stub `README`, `NEWS` and `ChangeLog` files
```
touch README NEWS ChangeLog libyacurs/ChangeLog
```
5. Run `autoreconf`
```
autoreconf -I m4 -i
```
6. Create the build directory
```
mkdir obj && cd obj
```
7. Configure and build
```
../configure
```
8. Create the man pages, `README`, `INSTALL`, and `NEWS` files
```
cd doc && gmake -f Makefile.doc
```
9. Build YAPET
```
cd .. && gmake
```

Environment Variables
===

* `EXTRA_LD_PRELOAD`: Use to add extra preload libraries to preloaded tests when using address sanitizer. 

Preprocessor Macros
===

* `CFGDEBUG`: Enable configuration handling console debug output.
* `DEBUG_LOG`: Enable debug output to `/tmp/yapet_debug.log`.