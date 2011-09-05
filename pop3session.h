
#ifndef _POP3SESSION__H
#define _POP3SESSION__H

#include <list>
#include <string>

#include "error.h"

class Socket;

class Pop3Session
{
    Socket* socket;

    public:
        Pop3Session();
        Pop3Session(std::string const& server, int const& port);
        ~Pop3Session();

        void authenticate(std::string const& username, std::string const& password);

        void printMessageList();
        void printMessage(int const& messageId);

        /* Exceptions */
        class ServerError;

    private:
        struct ServerResponse;

        void sendCommand(std::string const& command);
        void getResponse(ServerResponse* response);
        void getMultilineResponse(ServerResponse* response);

        void releaseSocket();
};

struct Pop3Session::ServerResponse
{
    bool status;
    std::string statusMessage;
    std::list<std::string> data;
};

class Pop3Session::ServerError : public Error
{
    public:
        ServerError(std::string const& what, std::string const& serverStatus)
        {
            problem = what;
            reason = serverStatus;
        }
};

#endif
