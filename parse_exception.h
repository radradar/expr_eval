#ifndef _PARSER_EXCEPTION_H_
#define _PARSER_EXCEPTION_H_

#include <exception>
#include <string>

#include "Status.h"

class parse_exception: public std::exception
{
public:

    explicit parse_exception(const char* message, status_t operationStatus):
      msg_(message), stat_(operationStatus)
      {}

	  explicit parse_exception(const std::string& message, status_t operationStatus):
      msg_(message), stat_(operationStatus)
      {}

    virtual ~parse_exception() throw ()
	{}

    virtual const char* what() const throw ()
	{
       return msg_.c_str();
    }

	status_t getPosition() const 
	{
		return stat_;
	}

protected:
    std::string msg_;
	status_t	stat_;
};


#endif