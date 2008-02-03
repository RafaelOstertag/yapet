// $Id$

#include <iostream>

#include <crypt.h>
#include <structs.h>
#include <record.h>
#include <bdbuffer.h>
#include <key.h>

#define CONTROL_STR "ABCDEFGHIJKLMNOPQRSTUVW"

int main(int argc, char** argv) {
    try {
	GPSAFE::Key key("TEST");
	GPSAFE::Crypt crypt(key);

	GPSAFE::FileHeader header;
	header.version = 1;
	memcpy(header.control, CONTROL_STR, GPSAFE::HEADER_CONTROL_SIZE);


	GPSAFE::Record<GPSAFE::FileHeader> record(header);


	GPSAFE::BDBuffer* data = crypt.encrypt(record);

	GPSAFE::Record<GPSAFE::FileHeader>* dec_header;

	dec_header=crypt.decrypt<GPSAFE::FileHeader>(*data);
	delete data;

	GPSAFE::FileHeader* fh_ptr(*dec_header);
	std::cout << fh_ptr->control << std::endl;
	delete dec_header;

    } catch (std::exception& ex) {
	std::cout << ex.what() << std::endl;
	return 1;
    }

    return 0;

}
