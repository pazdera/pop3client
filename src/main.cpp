/**
 * @brief Simple POP3 client
 *
 * @file main.cpp
 * @author Radek Pazdera (radek.pazdera@gmail.com)
 *
 *  This module contains the basic program operation logic.
 *  That is:
 *    process cli arguments,
 *    get password from stdin,
 *    process the user's request (i.e. print a list of available messages
 *                                     or download a specific message).
 *
 * @mainpage pop3client documentation
 *  Welcome to pop3client documentation. Hopefully you'll find here
 *  exactly what you need!
 *
 *  pop3client is a simple program that is able to connect to
 *  remote POP3 server and list/download messages from there.
 *
 *  It was developed as a part of Seznam.cz job try-out.
 *
 * @section license License
@verbatim
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
@endverbatim
 */

#include <iostream>
#include <string>

#include <cstdlib>
#include <cstdio>
#include <termios.h>

#include "config.h"
#include "error.h"
#include "cliarguments.h"
#include "pop3session.h"

/**
 * @brief Read password from terminal (stdin)
 *
 * Terminal ECHO is turned off while reading the password. This is a
 * substitute for glibc's getpass() function which is deprecated.
 *
 * @warning This (most likely) won't work on Windows.
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


/**
 * @brief Print help and exit.
 *
 *  This is a glibc-like function that prints help
 *  on standard error output and then exits with
 *  \c status code.
 *
 * @param[in] status Status code to exit with. 
 * @return Never returns.
 */
void usage(int status)
{

    std::cerr << "Usage: " << __PROGRAM_NAME << " -h hostname [-p port] -u username  [id]" << std::endl;
    std::cerr << "       -h hostname     remote IP address or hostname" << std::endl;
    std::cerr << "       -p port         remote TCP port" << std::endl;
    std::cerr << "       -u username     username" << std::endl;
    std::cerr << "       id              id of the message to download" << std::endl;

    exit(status);
}

int main(int argc, char **argv)
{
    CliArguments arguments;

    try /* Try to process cli arguments */
    {
        arguments.parse(argc, argv);
    }
    catch (CliArguments::GetoptError& error)
    {
        /* Getopt print's its own error report, so we just print help. */
        usage(EXIT_FAILURE);
    }
    catch (Error& error)
    {
        std::cerr << error.what() << std::endl;
        usage(EXIT_FAILURE);
    }

    /* Get password. */
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

    /* Process user's request. */
    try
    {
        Pop3Session pop3(arguments.getHostname(), arguments.getPort());
        pop3.authenticate(arguments.getUsername(), password);

        password.clear(); // Remove password from memory

        /* Either print the list of available messages or print
           some specific message. */
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

