// -*- c++ -*-
//
// $Id$

#ifndef _STRUCTS_H
#define _STRUCTS_H

#ifdef HAVE_CONFIG_H
#include <config.h>
#endif


namespace GPSAFE {

    enum {
	HEADER_CONTROL_SIZE = 20,
	NAME_SIZE = 128,
	HOST_SIZE = 256,
	USERNAME_SIZE = 256,
	PASSWORD_SIZE = 256,
	COMMENT_SIZE = 512
    };

    struct FileHeader {
	    uint8_t version;
	    uint8_t control[HEADER_CONTROL_SIZE];
	    time_t pwset;
    };

    struct PasswordRecord {
	    uint8_t name[NAME_SIZE];
	    uint8_t host[HOST_SIZE];
	    uint8_t username[USERNAME_SIZE];
	    uint8_t password[PASSWORD_SIZE];
	    uint8_t comment[COMMENT_SIZE];
    };
}

#endif // _STRUCTS_H
