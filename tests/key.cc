// $Id$

#include <stdio.h>
#include <iostream>
#include <exception>

#include <key.h>

const uint8_t expected_key[] = {
    0x3e,
    0xc3,
    0x34,
    0x5d,
    0x72,
    0x83,
    0xbd,
    0x09,
    0x60,
    0xa3,
    0x4f,
    0x6b,
    0x59,
    0x5b,
    0x93,
    0xb6,
    0x66,
    0x91,
    0x15,
    0x2c,
    0x65,
    0xc8,
    0x1a,
    0xdc,
    0x0a,
    0xc9,
    0x90,
    0xa7,
    0x93,
    0x70,
    0x96,
    0x94,
    0xa1,
    0x6d,
    0x81,
    0x06,
    0xc1,
    0x9a,
    0xba,
    0xd0,
    0x0e,
    0x94,
    0xd0,
    0xd1,
    0xf3,
    0xeb,
    0x5d,
    0x4d,
    0xd8,
    0xd1,
    0x95,
    0x72,
    0xac,
    0x33,
    0x86,
    0xe0
};

int main() {
    try {
	YAPET::Key key("JustATestPasswordForKeepingSecret");

	std::cout << " ==> ";
	for(unsigned int i=0; i < key.size(); i++)
	    printf("%02x", key()[i]);

	printf("\n");

	for(unsigned int i=0; i < key.size(); i++) {
	    if (key()[i] != expected_key[i]) {
		std::cout << " ##> Error in key at position " << i << std::endl;
		return 1;
	    }
	}
    } catch (std::exception& ex) {
	std::cout << ex.what() << std::endl;
	return 1;
    }
    return 0;

}
