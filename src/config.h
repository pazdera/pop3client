/**
 * @brief Global program configuration
 *
 * @file config.h
 * @author Radek Pazdera (radek.pazdera@gmail.com)
 * 
 *  Global configuration macros and values.
 *
 * @warning Beware of what you put in here and how do you
 *          name it! This file is global and the names can
 *          interfere with those defined anywhere in the program!
 */

#ifndef _CONFIG__H
#define _CONFIG__H

#define __PROGRAM_NAME "pop3client"

#define __DEFAULT_PORT 110

/* Timeout for reading from socket. In case
   the server doesn't respond */
#define __SOCKET_READ_TIMEOUT 30 // seconds

#endif
