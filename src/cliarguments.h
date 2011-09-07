/**
 * This is part of pop3client.
 *
 * @file cliarguments.h
 * @author Radek Pazdera (radek.pazdera@gmail.com)
 *
 * @brief Program interface to CLI arguments
 *
 * This module contains interface to CLI arguments that were
 * supplied to the program at runtime.
 */

#ifndef _CLIARGUMENTS__H
#define _CLIARGUMENTS__H

#include <string>
#include <unistd.h>

#include "error.h"

/** Interface to CLI arguments
 *
 * This class can parse arguments using getopt and it
 * also provides interface for accessing the values later
 * in the program
 */
class CliArguments
{
    private:
      static const int  MIN_PORT_RANGE = 1;
      static const int  MAX_PORT_RANGE = 65535;
      static const int  DEFAULT_PORT   = 110;

      int port;
      std::string username;
      std::string hostname;
      int messageId;

    public:
        CliArguments();

        void parse(int argc, char **argv);

        int getPort() const { return port; }
        std::string getUsername() const { return username; }
        std::string getHostname() const { return hostname; }
        int getMessageId() const { return messageId; }

        bool isMessageIdSet() const { return messageId != 0; }

        /* Exceptions */
        class GetoptError;
        class ArgumentDomainError;
        class MissingArgumentError;

    private:
        static int convertStringToInteger(std::string numberStoredInString);

        void setPort(char* optarg);
        void setHostname(char* optarg);
        void setUsername(char* optarg);
        void setMessageId(char* optarg);

        void checkMandatoryArguments() const;
};

/**
 * This exception is thrown when the getopt() function
 * fails (i.e. returns '?'). There are no information
 * associated with the error, because getopt prints its
 * own error report to stderr. So you don't need to
 * report anything when you catch this error.
 */
class CliArguments::GetoptError : public Error
{};

/**
 * Argument of some option was out of it's domain. It
 * might be caused -- for example -- by supplying
 * a string where a number was expected etc.
 */
class CliArguments::ArgumentDomainError : public Error
{
    public:
        ArgumentDomainError(std::string const& argument, std::string const& cause)
        {
            problem = "Invalid " + argument + " argument";
            reason  = cause;
        }
};

/**
 * Instance of this class is thrown when some of the
 * mandatory CLI arguments were missing.
 */
class CliArguments::MissingArgumentError : public Error
{
    public:
        MissingArgumentError(std::string const& argument)
        {
            problem = "Missing argument";
            reason  = argument + " is mandatory";
        }
};

#endif

