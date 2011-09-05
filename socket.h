/**
 *  IPK 1 - webclient
 *  Author: Radek Pazdera (xpazde00@stud.fit.vutbr.cz)
 *  Date: 13.03.2010
 *
 *  Description:
 *    Socket class is abstraction of BSD socket API.
 */

#ifndef _SOCKET__H
#define _SOCKET__H

#include <string>

#include "error.h"
/**
 * Socket Class
 *
 * Tries to enstabilish socket connection between you and remote host using
 * TCP protocol. The class also offers interface for sending and recieving
 * messages through the open socket.
 *
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

        /** Just interface for read(2) */
        size_t read(char* buffer, size_t size);

        /** Send data to remote host */
        void write(std::string request);

        /** Reads everything available from socket to a std::string  */
        void readAll(std::string* response);

        /** Read until \r\n or the end */
        void readLine(std::string* line);


        /* Exceptions */
        class ConnectionError;
        class IOError;

    private:
        /** Tries to enstabilish connection between you and the remote host */
        void open();
        /** Terminate socket connection */
        void close();

        /** Reads one byte from the socket */
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
