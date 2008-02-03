// $I$

#include <stdio.h>
#include <iostream>
#include <exception>

#include <key.h>

int main() {
    try {
	GPSAFE::Key key("JustATestPasswordForKeepingSecret");

    for(unsigned int i=0; i < key.size(); i++)
	printf("%02x", key()[i]);

    printf("\n");
    } catch (std::exception& ex) {
	std::cout << ex.what() << std::endl;
	return 1;
    }    
    return 0;

}
