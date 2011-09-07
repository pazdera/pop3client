/**
 * This is a part of pop3client.
 *
 * @file error.h
 * @author Radek Pazdera (radek.pazdera@gmail.com)
 * 
 * @brief Error handling and reporting tools
 */

#ifndef _ERROR__H
#define _ERROR__H

#include <stdexcept>
#include <string>

#define PROGRAM_NAME "pop3client"

/**
 * Base class for errors and exceptions
 *
 * All exceptions in this program should be derived
 * from this class.
 */
class Error : public std::exception
{
    protected:
        std::string programName;
        std::string problem;
        std::string reason;

    public:
        Error(std::string what = "", std::string why = "");
        virtual ~Error() throw();
        
        /**
         * Get error report
         *
         * Return an error report in the following format:
         *     <programName>: <problem>: <reason>
         *
         * @return Error report
         */
        const char* what() const throw();
};

#endif
