/**
 * This is a part of pop3client.
 *
 * @file error.h
 * @author Radek Pazdera (radek.pazdera@gmail.com)
 * 
 * @brief BSD sockets API adapter
 */

#ifndef _SOCKET__H
#define _SOCKET__H

#include <string>

#include "error.h"

/**
 * This class is a simple object-oriented wrapper (adapter)
 * around the BSD socket API. Only the TCP client part of
 * the api is supported at the moment. 
 */
class Socket
{
    int socketFileDescriptor;
    std::string address;
    std::string port;

    public:
        Socket(std::string const& inputAddress, int inputPort);
        Socket(std::string const& inputAddress, std::string const& inputPort);
        ~Socket();

        /** 
         * Read exact number of bytes from the socket
         *
         *  Reads at most \c size bytes from the socket and
         *  stores them to \c buffer. The buffer's size must
         *  be greater then the \c size, otherwise expect
         *  some segfaults.
         * 
         * @param[out] buffer Where to store the data
         * @param[in] size How many bytes to read
         *
         * @return Number of bytes actually read
         */
        size_t read(char* buffer, size_t size);

        /**
         * Send data to remote host
         *
         *  Writes string stored in \c request to the socket
         *  without the terminatin '\0'.
         *
         * @param[in] request String to write to the socket
         * @return void
         */
        void write(std::string request);

        /**
         * Read all available data from the socket
         *
         *  Reads all available data from the socket byte-by-byte
         *  and stores it to \c response.
         *
         * @param[out] response Where to store the data
         * @return void
         */
        void readAll(std::string* response);

        /** Read until \r\n or the end */
        void readLine(std::string* line);


        /* Exceptions */
        class ConnectionError;
        class IOError;

    private:
        void open();
        void close();

        /** Read exactly one byte from the socket
         *
         * @param[out] buffer Recieved byte
         * @return True on success otherwise False, when
         *         there's nothing to read.
         */
        bool readCharacter(char* buffer);

};

class Socket::ConnectionError : public Error
{
    public:
        ConnectionError(std::string const& cause)
        {
            problem = "Unable to connect";
            reason  = cause;
        }
};

class Socket::IOError : public Error
{
    public:
        IOError(std::string const& issue, std::string const& cause)
        {
            problem = issue;
            reason  = cause;
        }
};

#endif
