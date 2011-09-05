/**
 *  @author Radek Pazdera (xpazde00@stud.fit.vutbr.cz)
 *
 *  CliArguments implementation.
 */


#include "cliarguments.h"

#include <string>
#include <iostream>
#include <stdlib.h>
#include <unistd.h>


CliArguments::CliArguments()
{}

void CliArguments::parse(int argc, char **argv)
{
    int option = 0;

    /* Defaults */
    port = DEFAULT_PORT;
    hostname = "";
    username = "";
    messageId = 0;

    while ((option = getopt (argc, argv, "h:p:u:")) != -1)
    {
      switch (option)
      {
        case 'h': /* Hostname */
          setHostname(optarg);
          break;
        case 'p': /* Port number */
          setPort(optarg);
          break;
        case 'u': /* Username */
          setUsername(optarg);
          break;
        case '?':
          throw GetoptError();
          break;
      }
    }

    /* id argument */
    if (optind < argc)
    {
        setMessageId(argv[optind]);
    }

    verifyMandatoryArguments();
}

void CliArguments::verifyMandatoryArguments() const
{
    if (hostname.length() <= 0)
    {
        throw MissingArgumentError("-h");
    }

    if (username.length() <= 0)
    {
        throw MissingArgumentError("-u");
    }
}

int CliArguments::convertStringToInteger(std::string numberStoredInString)
{
  return static_cast<int>(atoi(numberStoredInString.c_str()));
}

void CliArguments::setPort(char* optarg)
{
    port = convertStringToInteger(std::string(optarg));

    if (port < MIN_PORT_RANGE || port > MAX_PORT_RANGE)
    {
        throw ArgumentDomainError("-p", "Value out of range (1 ~ 65535)");
    }
}

void CliArguments::setHostname(char* optarg)
{
    hostname = std::string(optarg);
}

void CliArguments::setUsername(char* optarg)
{
    username = std::string(optarg);
}

void CliArguments::setMessageId(char* optarg)
{
    messageId = convertStringToInteger(optarg);

    if (messageId <= 0)
    {
        throw ArgumentDomainError("id", "Message id must be number greater than 0");
    }
}
