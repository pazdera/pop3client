/**
 * @author Radek Pazdera
 *
 * Base Error class. Usually thrown as exception.
 */

#include <stdexcept>
#include <string>

#ifndef _ERRORS__H
#define _ERRORS__H

class Error : public std::exception
{
    protected:
        std::string programName;
        std::string problem;
        std::string reason;

    public:
        Error(std::string what = "", std::string why = "");
        ~Error() throw();
        
        const char* what() const throw();
};

#endif
