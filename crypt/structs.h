// -*- c++ -*-
//
// $Id$
//
// YAPET -- Yet Another Password Encryption Tool
// Copyright (C) 2008  Rafael Ostertag
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef _STRUCTS_H
#define _STRUCTS_H

#ifdef HAVE_CONFIG_H
# include <config.h>
#endif

#ifdef HAVE_INTTYPES_H
# include <inttypes.h>
#endif

namespace YAPET {

    enum {
	/**
	 * The length of the control string used to check whether
	 * decryption was successful
	 */
	HEADER_CONTROL_SIZE = 20,
	/**
	 * The maximum length of the name string of a password record.
	 */
	NAME_SIZE = 128,
	/**
	 * The maximum length of the host string of a password
	 * records.
	 */
	HOST_SIZE = 256,
	/**
	 * The maximum length of the user name string of a password
	 * record.
	 */
	USERNAME_SIZE = 256,
	/**
	 * The maximum length of the password string of a password
	 * record.
	 */
	PASSWORD_SIZE = 256,
	/**
	 * The maximum length of the comment string of a password
	 * record.
	 */
	COMMENT_SIZE = 512
    };

    /**
     * @brief The file header.
     *
     * This is the file header structure.
     */
    struct FileHeader {
	    /**
	     * @brief Version
	     *
	     * Holds the version of the file structure.
	     */
	    uint8_t version;
	    /**
	     * @brief Control string
	     *
	     * Holds the control string used to verify the decryption
	     * of the file. It is filled with a known sequence of
	     * characters before encryption. After decryption the
	     * sequence of characters stored in this field and the
	     * predefined are compared. If they are identical, the
	     * decryption is assumed to be successful.
	     */
	    uint8_t control[HEADER_CONTROL_SIZE];
	    /**
	     * @brief The date the key has first been used.
	     *
	     * Holds the date the key has first been used to encrypt
	     * the file.
	     */
	    time_t pwset;
    };

    /**
     * @brief Structure of the password record.
     *
     * The structure of the password records.
     */
    struct PasswordRecord {
	    /**
	     * @brief Name of the record.
	     *
	     * Name of the password records. \c PartDec decrypts the
	     * record to get this field. It is used to give the user
	     * an idea about the content of the record.
	     */
	    uint8_t name[NAME_SIZE];
	    /**
	     * @brief The host for which the password is used
	     *
	     * Holds the host name for which the password is used.
	     */
	    uint8_t host[HOST_SIZE];
	    /**
	     * @brief The user name
	     *
	     * The user name for the password.
	     */
	    uint8_t username[USERNAME_SIZE];
	    /**
	     * @brief The password.
	     *
	     * The password.
	     */
	    uint8_t password[PASSWORD_SIZE];
	    /**
	     * @brief A comment.
	     *
	     * An additional comment.
	     */
	    uint8_t comment[COMMENT_SIZE];
    };
}

#endif // _STRUCTS_H
