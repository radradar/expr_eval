#ifndef _EVALUATION_EXCEPTION_H_
#define _EVALUATION_EXCEPTION_H_

#include <exception>
#include <string>

#include "Status.h"

namespace expressionEval
{

	class evaluation_exception: public std::exception
	{
	public:

		explicit evaluation_exception(const char* message, const Status &operationStatus, double value=0.0):
		msg_(message)
		,stat_(operationStatus) 
		,value_(value)
		{}

		explicit evaluation_exception(const std::string& message, const Status &operationStatus, double value=0.0):
		msg_(message)
		,stat_(operationStatus)
		,value_(value)
		{}

		virtual ~evaluation_exception() throw ()
		{}

		virtual const char* what() const throw ()
		{
			return msg_.c_str();
		}

		double getValue() const throw()
		{
			return value_;
		}

		const Status& getStatus() const throw()
		{
			return stat_;
		}

	protected:
		std::string msg_;
		Status stat_;
		double	value_;
	};

} //namespace expressionEval

#endif