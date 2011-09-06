/**
 * This is pop3client.
 *
 * @file main.cpp
 * @author Radek Pazdera
 * 
 * @brief Simple POP3 client
 *
 * This module contains the basic program operation logic.
 * That is:
 *    1) process cli arguments
 *    2) get password from stdin
 *    3) process the user's request (i.e. print a list of
 *       available messages or download a specific message)
 */

#include <iostream>
#include <string>

#include <cstdlib>
#include <cerrno>
#include <cstdio>
#include <termios.h>

#include "error.h"
#include "cliarguments.h"
#include "pop3session.h"

#ifdef _GNU_SOURCE
    extern "C" const std::string PROGRAM_NAME(program_invocation_name);
#else
    extern "C" const std::string PROGRAM_NAME("pop3client");
#endif

/** Read password from terminal (stdin).
 *
 * Terminal ECHO is turned off while reading the
 * password. This is a substitute for glibc
 * getpass() function which is deprecated.
 *
 * @return Entered password.
 */
std::string getPassword()
{
    struct termios oldTerminalFlags, newTerminalFlags;

    if (tcgetattr(fileno(stdin), &oldTerminalFlags) != 0)
    {
        throw std::runtime_error("Unable to set terminal flags");
    }

    newTerminalFlags = oldTerminalFlags;
    newTerminalFlags.c_lflag &= ~( ECHO);
 
    if (tcsetattr(fileno(stdin), TCSAFLUSH, &newTerminalFlags) != 0)
    {
        throw std::runtime_error("Unable to set terminal flags");
    }

    /* Read the password. */
    std::string password;
    std::getline(std::cin, password);    

    /* Restore terminal. */
    tcsetattr(fileno(stdin), TCSAFLUSH, &oldTerminalFlags);

    return password;
}


void usage(int status)
{

    std::cerr << "Usage: " << PROGRAM_NAME << " -h hostname [-p port] -u username  [id]" << std::endl;
    std::cerr << "       -h hostname     remote IP address or hostname" << std::endl;
    std::cerr << "       -p port         remote TCP port" << std::endl;
    std::cerr << "       -u username     username" << std::endl;
    std::cerr << "       id              id of the message to download" << std::endl;

    exit(status);
}

int main(int argc, char **argv)
{
    CliArguments arguments;

    try
    {
        arguments.parse(argc, argv);
    }
    catch (CliArguments::GetoptError& error)
    {
        usage(EXIT_FAILURE);
    }
    catch (Error& error)
    {
        std::cerr << error.what() << std::endl;
        usage(EXIT_FAILURE);
    }

    std::cerr << "Port: " << arguments.getPort() << std::endl;
    std::cerr << "User: " << arguments.getUsername() << std::endl;
    std::cerr << "Host: " << arguments.getHostname() << std::endl;
    std::cerr << "ID:   " << arguments.getMessageId() << std::endl;


    std::string password;
    try
    {
        std::cout << "Password for " << arguments.getUsername() << ": ";
        password = getPassword();
        std::cout << std::endl;
    }
    catch (std::exception& error)
    {
        std::cerr << error.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    try /* contacting POP3 server */
    {
        Pop3Session pop3(arguments.getHostname(), arguments.getPort());
        pop3.authenticate(arguments.getUsername(), password);

        /* Remove password from memory. */
        password.clear();

        if (arguments.isMessageIdSet())
        {
            pop3.printMessage(arguments.getMessageId());
        }
        else
        {
            pop3.printMessageList();
        }
    }
    catch (Error& error)
    {
        std::cerr << error.what() << std::endl;
        exit(EXIT_FAILURE);
    }

    return EXIT_SUCCESS;
}

