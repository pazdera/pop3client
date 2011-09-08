/**
 * @brief BSD sockets API adapter.
 *
 * @file socket.cpp
 * @author Radek Pazdera (radek.pazdera@gmail.com)
 * 
 */

#include "config.h"
#include "socket.h"
#include "error.h"

#include <string>
#include <iostream>
#include <sstream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <stdlib.h>
#include <stdio.h>

#include <unistd.h>
#include <string.h>


Socket::Socket(std::string const& inputAddress, std::string const& inputPort)
{
    socketFileDescriptor = -1;

    address = inputAddress;
    port    = inputPort;

    open();
}

Socket::Socket(std::string const& inputAddress, int inputPort)
{
    socketFileDescriptor = -1;

    std::stringstream portInString;
    portInString << inputPort;

    address = inputAddress;
    port    = portInString.str();

    open();
}

void Socket::open()
{
    struct addrinfo hints;
    struct addrinfo *result, *resultPointer;

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = PF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = 0;
    hints.ai_protocol = 0;

    int getaddrinfoReturnCode = getaddrinfo(address.c_str(), port.c_str(), &hints, &result);
    if (getaddrinfoReturnCode != 0)
    {
        throw ConnectionError(gai_strerror(getaddrinfoReturnCode));
    }

    for (resultPointer = result; resultPointer != NULL; resultPointer = resultPointer->ai_next)
    {
        socketFileDescriptor = socket(resultPointer->ai_family, resultPointer->ai_socktype, resultPointer->ai_protocol);
        if (socketFileDescriptor == -1)
        {
            continue;
        }

        if (connect(socketFileDescriptor, resultPointer->ai_addr, resultPointer->ai_addrlen) != -1)
        {
            break;
        }

        ::close(socketFileDescriptor);
    }

    if (resultPointer == NULL)
    {
        throw ConnectionError("Cannot establish connection to the server");
    }

    ::freeaddrinfo(result);
}

Socket::~Socket()
{
    if (socketFileDescriptor > 0)
    {
        close();
    }
}

void Socket::close()
{
    ::shutdown(socketFileDescriptor, SHUT_RDWR);
    ::close(socketFileDescriptor);
}

size_t Socket::read(char* buffer, size_t size)
{
    if (!isReadyToRead())
    {
        throw IOError("Recieving error", "Server not responding (connection timed out).");
    }

    ssize_t bytesRead = ::read(socketFileDescriptor, buffer, size);
    if (bytesRead < 0)
    {
        throw IOError("Recieving error", "Unable to resolve data from remote host");
    }

    return bytesRead;
}

void Socket::write(std::string request)
{
    if (::write(socketFileDescriptor, request.c_str(), request.length()) < 0)
    {
        throw IOError("Sending error", "Unable to send data to remote host");
    }
}

void Socket::readAll(std::string *response)
{
    char buffer;

    while (readCharacter(&buffer))
    {
        *response += buffer;
    }
}

bool Socket::readCharacter(char* buffer)
{
    if (read(buffer, 1) > 0)
    {
        return true;
    }
    return false;
}

size_t Socket::readLine(std::string* line)
{
    char buffer[2] = "";
    *line = "";
    size_t bytesRead = 0;

    if (readCharacter(&(buffer[1])))
    {
        do
        {
            *line     += buffer[0]; // add char to string
            buffer[0]  = buffer[1];
            bytesRead++;
        }
        while (readCharacter(&(buffer[1])) &&
               !(buffer[0] == '\r' && buffer[1] == '\n'));
     
        line->erase(0, 1);
    }

    return bytesRead;
}

bool Socket::isReadyToRead()
{
    fd_set recieveFd;
    struct timeval timeout;
    int selectReturnValue;

    FD_ZERO(&recieveFd);
    FD_SET(socketFileDescriptor, &recieveFd);

    /* 30 seconds timeout */
    timeout.tv_sec = __SOCKET_READ_TIMEOUT;
    timeout.tv_usec = 0;

    selectReturnValue = select(socketFileDescriptor + 1, &recieveFd, NULL, NULL, &timeout);

    if (selectReturnValue > 0)
    {
        return true;
    }

    return false;
}

