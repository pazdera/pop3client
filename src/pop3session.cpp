/**
 * @brief Client part of POP3 session
 *
 * @file pop3session.cpp
 * @author Radek Pazdera (radek.pazdera@gmail.com)
 * 
 */

#include "pop3session.h"

#include <iostream>
#include <sstream>

#include "socket.h"

Pop3Session::Pop3Session()
    : socket(NULL)
{}

Pop3Session::Pop3Session(std::string const& server, int port)
{
    open(server, port);
}

Pop3Session::~Pop3Session()
{
   close();
}

void Pop3Session::sendCommand(std::string const& command)
{
    socket->write(command + "\r\n");
}

void Pop3Session::getResponse(ServerResponse* response)
{
    std::string buffer;
    socket->readLine(&buffer);

    if (buffer[0] == '+')
    {
        response->status = true;
        buffer.erase(0, 3); // Remove the "+OK "
    }
    else
    {
        response->status = false;
        buffer.erase(0, 4); // Remove the "-ERR "
    }

    response->statusMessage = buffer;
    response->data.clear();
}

void Pop3Session::getMultilineData(ServerResponse* response)
{
    std::string buffer;

    while (true)
    {
        buffer.clear();
        socket->readLine(&buffer);
        
        if (buffer == ".")
        {
            break;
        }
        else
        {
            if (buffer[0] == '.') /* Strip byte stuffed characters. */
            {
                buffer.erase(0, 1);
            }

            response->data.push_back(buffer);
        }
    }
}

void Pop3Session::open(std::string const& server, int port)
{
    socket = new Socket(server, port);
    
    ServerResponse welcomeMessage;
    
    
    getResponse(&welcomeMessage);

    if (!welcomeMessage.status)
    {
        throw ServerError("Conection refused", welcomeMessage.statusMessage);
    }
}

void Pop3Session::close()
{
    if (socket != NULL)
    {
        delete socket;
    }
}

void Pop3Session::authenticate(std::string const& username, std::string const& password)
{
    ServerResponse response;

    sendCommand("USER " + username);
    getResponse(&response);

    if (!response.status)
    {
        throw ServerError("Authentication failed", response.statusMessage);
    }

    sendCommand("PASS " + password);
    getResponse(&response);

    if (!response.status)
    {
        throw ServerError("Authentication failed", response.statusMessage);
    }
}

void Pop3Session::printMessageList()
{
    ServerResponse response;

    sendCommand("LIST");

    getResponse(&response);
    if (!response.status)
    {
        throw ServerError("Unable to retrieve message list", response.statusMessage);
    }

    getMultilineData(&response);

    if (response.data.size() == 0)
    {
        std::cout << "No messages available on the server." << std::endl;
    }

    int spacePosition = 0;
    for (std::list<std::string>::iterator line = response.data.begin();
         line != response.data.end();
         line++)
    {
        spacePosition = line->find(' ');
        std::cout << line->substr(0, spacePosition) << std::endl;
    }
}

void Pop3Session::printMessage(int messageId)
{
    ServerResponse response;

    std::stringstream command;
    command << "RETR " << messageId;

    sendCommand(command.str());

    getResponse(&response);
    if (!response.status)
    {
        throw ServerError("Unable to retrieve message list", response.statusMessage);
    }

    getMultilineData(&response);
    
    for (std::list<std::string>::iterator line = response.data.begin();
         line != response.data.end();
         line++)
    {
        std::cout << *line << std::endl;
    }
}
