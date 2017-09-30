#ifndef _PARSER_EXCEPTION_H_
#define _PARSER_EXCEPTION_H_

#include <exception>
#include <string>

#include "Status.h"


namespace expressionEval
{

	class parse_exception: public std::exception
	{
	public:

		explicit parse_exception(const char* message, const Status &operationStatus):
		msg_(message), 
		stat_(operationStatus)
		{}

		explicit parse_exception(const std::string& message, const Status &operationStatus):
		msg_(message), 
		stat_(operationStatus)
		{}

		virtual ~parse_exception() throw ()
		{}

		virtual const char* what() const throw ()
		{
			return msg_.c_str();
		}

		const Status& getStatus() const throw()
		{
			return stat_;
		}

	protected:
		std::string msg_;
		Status		stat_;
	};

} //namespace expressionEval

#endif