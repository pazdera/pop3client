/**
 * @brief Error class implementation
 *
 * @file error.cpp
 * @author Radek Pazdera (radek.pazdera@gmail.com)
 * 
 */

#include "error.h"

#include <stdexcept>
#include <string>
#include <error.h>
#include <errno.h>


Error::Error(std::string what, std::string why)
{
    programName = PROGRAM_NAME;

    problem = what;
    reason  = why;
}

Error::~Error() throw()
{}

const char* Error::what() const throw()
{
    std::string message = programName;

    if (problem.length() > 0)
    {
        message += ": " + problem;
    }

    if (reason.length() > 0)
    {
        message += ": " + reason;
    }

    return message.c_str();
}

