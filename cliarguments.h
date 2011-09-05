/**
 *  Author: Radek Pazdera (xpazde00@stud.fit.vutbr.cz)
 *  Date: 10.10.2010
 *
 *  Description:
 *    This class parses cli arguments and then suplies
 *    abstract interface for accessing them.
 */


#ifndef _CLIARGUMENTS__H
#define _CLIARGUMENTS__H

#include <string>
#include <unistd.h>

#include "error.h"

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

    public: /* Interface */
        CliArguments();

        void parse(int argc, char **argv);

        int getPort() const { return port; }
        std::string getUsername() const { return username; }
        std::string getHostname() const { return hostname; }
        int getMessageId() const { return messageId; }

    private:
        static int convertStringToInteger(std::string numberStoredInString);

        void setPort(char* optarg);
        void setHostname(char* optarg);
        void setUsername(char* optarg);
        void setMessageId(char* optarg);

        void verifyMandatoryArguments() const;

    public: /* Exceptions */
        class GetoptError;
        class ArgumentDomainError;
        class MissingArgumentError;
};


class CliArguments::GetoptError : public Error
{};

class CliArguments::ArgumentDomainError : public Error
{
    public:
        ArgumentDomainError(std::string const& argument, std::string const& cause)
        {
            problem = "Invalid " + argument + " argument";
            reason  = cause;
        }
};

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

