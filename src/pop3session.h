/**
 * @brief Client part of POP3 Session
 *
 * @file pop3session.h
 * @author Radek Pazdera (radek.pazdera@gmail.com)
 * 
 */

#ifndef _POP3SESSION__H
#define _POP3SESSION__H

#include <list>
#include <string>

#include "error.h"

class Socket; /* Forward-declaration. */

/**
 * @brief POP3 client session.
 *
 *  This class is responsible for communication with the server using POP3
 *  over TCP. It sends POP3 commands to the server and process its responses.
 *
 */
class Pop3Session
{
    Socket* socket;

    public:
        Pop3Session();
        Pop3Session(std::string const& server, int port);
        ~Pop3Session();

        /**
         * @brief Authenticate user on the remote server.
         *
         *  The authentication is plain-text by issuing commands
         *  USER and PASS.
         *
         * @param[in] username Username on remote POP3 server.
         * @param[in] password Password in plain-text form.
         * @return void
         */
        void authenticate(std::string const& username, std::string const& password);

        /**
         * @brief Print list of available messages.
         *
         *  Retrieve a list of available messages and print it.
         *  This method issues LIST command to the server.
         *
         *  Prints a notice when no messages are available.
         *
         * @return void
         */
        void printMessageList();

        /**
         * @brief Print message by it's ID.
         *
         *  This method downloads and prints a message specified
         *  by a message ID.
         *
         * @param[in] messageId Id of the message to download.
         * @return void
         */
        void printMessage(int messageId);

        /* Exceptions */
        class ServerError;

    private:
        struct ServerResponse;

        /**
         * @brief Send POP3 command.
         *
         *  Sends \c command to the remote server. The command consists of
         *  a keyword and a set of parameters. The \\r\\n to confirm
         *  the command is added to the string automatically within this
         *  method.
         *
         * @param[in] command POP3 command to the server.
         * @return void
         */
        void sendCommand(std::string const& command);

        /**
         * @brief Fetch response from the remote server.
         *
         *  This method reads server's response and stores it within
         *  the \c response data structure.
         *
         * @param[out] response Server's response.
         * @return void
         */
        void getResponse(ServerResponse* response);

        /**
         * @brief Fetch \b multiline data part of the response.
         *
         *  This method can be used as a follow-up to getResponse(),
         *  when there are some multiline data apended to the response.
         *
         * @param[out] response Where to store the data.
         * @return void
         */
        void getMultilineData(ServerResponse* response);

        void open(std::string const& server, int port);
        void close();
};

/**
 * @brief Data structure for POP3 server responses.
 *
 *  This is used internaly by Pop3Session to store server's
 *  responses.
 */
struct Pop3Session::ServerResponse
{

    bool status; /*< It's true on +OK, false on -ERR */
    std::string statusMessage;
    std::list<std::string> data; /*< Multi-line data in case, they were present. */
};

/**
 * @brief Indicates that server answered with -ERR status.
 *
 *  An instance of this class is thrown when the server reports
 *  some failure such as authentication problem or a nonexistent
 *  message.
 */
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
