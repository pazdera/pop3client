/**
 * @brief BSD sockets API adapter.
 *
 * @file socket.h
 * @author Radek Pazdera (radek.pazdera@gmail.com)
 * 
 */

#ifndef _SOCKET__H
#define _SOCKET__H

#include <string>

#include "error.h"

/**
 * @brief Object-oriented BSD socket API wrapper.
 *
 *  This class is a simple object-oriented wrapper (adapter)
 *  around the BSD socket API. Only the TCP client part of
 *  the api is supported at the moment. 
 */
class Socket
{
    int socketFileDescriptor;
    std::string address;
    std::string port;

    public:
        //Socket(); /* No default constructor. */
        Socket(std::string const& inputAddress, int inputPort);
        Socket(std::string const& inputAddress, std::string const& inputPort);
        ~Socket();

        /** 
         * @brief Read exact number of bytes from the socket.
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
         * @brief Send data to remote host.
         *
         *  Writes string stored in \c request to the socket
         *  without the terminatin '\\0'.
         *
         * @param[in] request String to write to the socket
         * @return void
         */
        void write(std::string request);

        /**
         * @brief Read all available data from the socket.
         *
         *  Reads all available data from the socket byte-by-byte
         *  and stores it to \c response.
         *
         * @param[out] response Where to store the data
         * @return void
         */
        void readAll(std::string* response);

        /**
         * @brief Read a single line from socket.
         * 
         *  This method reads byte-by-byte from the socket until
         *  \\r\\n is detected or no more data are available.
         *
         * @param [out] line Acquired data.
         * @return void
         */
        void readLine(std::string* line);


        /* Exceptions */
        class ConnectionError;
        class IOError;

    private:
        void open();
        void close();

        /**
         * @brief Read exactly one byte from the socket.
         *
         * @param[out] buffer Recieved byte
         * @return True on success otherwise False, when
         *         there's nothing to read.
         */
        bool readCharacter(char* buffer);

        bool isReadyToRead();

};

/**
 * @brief Indicates trouble with connection.
 *
 *  This is thrown when the remote host wasn't found
 *  or when it doesn't accept the connection.
 */
class Socket::ConnectionError : public Error
{
    public:
        ConnectionError(std::string const& cause)
        {
            problem = "Unable to connect";
            reason  = cause;
        }
};

/**
 * @brief Indicates problems with receiving/sending data.
 *
 *  Instance of this class is thrown when reading or writing
 *  data to socket fails.
 */
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
