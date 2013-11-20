// $Id$
//
// Test whether or not the endianess conversion functions work.
//
// Depends on endianess.test file. The file holds 16, 32, 64 bit values in big
// endian order. The values are ordered as shown below having the values also as shown below:
//
// uint16_t: 258
// uint32_t: 16909060L
// uint64_t: 72623859790382856LL

#ifdef HAVE_CONFIG_H
# include "config.h"
#endif

#include <cassert>
#include <iostream>

#ifdef HAVE_SYS_TYPES_H
# include <sys/types.h>
#endif
#ifdef HAVE_SYS_STAT_H
# include <sys/stat.h>
#endif
#ifdef HAVE_FCNTL_H
# include <fcntl.h>
#endif
#ifdef HAVE_UNISTD_H
# include <unistd.h>
#endif

#include "testpaths.h"
#include <file.h>

class gainaccess : public YAPET::File {
	protected:
		YAPET::Key key;
	public:
	gainaccess() :  YAPET::File("endianess.pet", key, true, false),
					key("JustATestPasswordForKeepingSecret") {};

	template<class t>
	t myint_to_disk(t i) const {
		return int_to_disk<t>(i);
	}

	template<class t>
	t myint_from_disk(t i) const {
		return int_to_disk<t>(i);
	}
};


int main(int, char**) {
#ifndef TESTS_VERBOSE
	int stdout_redir_fd = open("/dev/null", O_WRONLY | O_APPEND);
	dup2(stdout_redir_fd,STDOUT_FILENO);
#endif
	std::cout << std::endl;

	try {
	gainaccess ga;

	int fd = open(SRCDIR "/endianess.test", O_RDONLY);
	if (fd < 0) {
		std::cout << " ==> Unable to open " SRCDIR "/endianess.test. Aborting test." << std::endl;
		return 0;
	}

	uint16_t test16;
	read(fd, &test16, sizeof(uint16_t));
	test16 = ga.myint_from_disk<uint16_t>(test16);
	if (test16 != 258) {
		std::cout << " ==> uint16_t does not match. Expected: 258, got: " << test16 << std::endl;
		close(fd);
		return 1;
	} else {
		std::cout << " --> uint16_t succeeded" << std::endl;
	}

	uint32_t test32;
	read(fd, &test32, sizeof(uint32_t));
	test32 = ga.myint_from_disk<uint32_t>(test32);
	if (test32 != 16909060) {
		std::cout << " ==> uint32_t does not match. Expected: 16909060L, got: " << test32 << std::endl;
		close(fd);
		return 1;
	} else {
		std::cout << " --> uint32_t succeeded" << std::endl;
	}

	uint64_t test64;
	read(fd, &test64, sizeof(uint64_t));
	test64 = ga.myint_from_disk<uint64_t>(test64);
#if SIZEOF_LONG == 8
	if (test64 != 72623859790382856L)
#elif SIZEOF_LONG == 4
	if (test64 != 72623859790382856LL)
#else
# error "sizeof(long) neither 8 nor 4"
#endif
		{
		std::cout << " ==> uint64_t does not match. Expected: 16909060L, got: " << test64 << std::endl;
		close(fd);
		return 1;
	} else {
		std::cout << " --> uint64_t succeeded" << std::endl;
	}
	close(fd);
	} catch(std::exception &ex) {
	std::cout << " ==> Had exception " << ex.what() << " during initialzation of gainaccess object. Aborting." << std::endl;
	return 1;
	}
	return 0;

}
