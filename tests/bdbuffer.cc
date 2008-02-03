// $Id$

#include <iostream>
#include <bdbuffer.h>

void test(GPSAFE::BDBuffer data) {
    std::cout << data.size() << std::endl;
}

int main(int argc, char** argv) {
    try {

	GPSAFE::BDBuffer data1;
	GPSAFE::BDBuffer data2(512);
	GPSAFE::BDBuffer data3(1024);

	data1 = data2;
	data2 = data3;
	data3 = data1;

	test(data3);
	data3.resize(2);
	test(data3);
	data3.resize(2048);
	test(data3);
	data3.resize(1);
	test(data3);

	data3.resize(100);

	data3[0] = 'a';
	data3[1] = 'b';
	data3[2] = 'c';
	data3[3] = '\0';

	uint8_t* tmp = data3;
	std::cout << tmp << std::endl;


    } catch (std::exception& ex) {
	std::cout << ex.what() << std::endl;
	return 1;
    }

    return 0;
}
