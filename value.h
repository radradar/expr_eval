#ifndef _VALUE_OPAQUE_H_
#define _VALUE_OPAQUE_H_

#include <stdint.h>

namespace expressionEval
{

//
// type of data
//
typedef enum
{
	UNKNOWN,
	INTEGER,
	FLOATINGPOINT,
	BOOLEAN,
}valueType_t;

//
// holds int or float value and type of holding value 
//
struct value_t
{
	union
	{
		double	fValue;
		int64_t iValue;
		bool	bValue;
	};

	valueType_t type;

	value_t():iValue(0L), type(UNKNOWN)
	{}

	//
	// cast to double
	//
	double toDouble() const 
	{ 
		double ret=0.0f;
		
		if (type == INTEGER )
			ret = static_cast<double>(iValue);
		else
		if (type == BOOLEAN )
			ret = static_cast<double>(bValue);
		else
			ret =  fValue; 
		return ret;
	}

	//
	// cast to integer
	//
	int64_t toInteger() const 
	{ 
		int64_t ret=0L;
#pragma warning (push)
#pragma warning( disable:4244 ) // lost precision
		if (type == INTEGER )
			ret = iValue;
			else
			if ( type == BOOLEAN )
				ret = static_cast<int64_t>(bValue);
				else
					ret = static_cast<int64_t>(fValue);
#pragma warning (pop)
		return ret;
	}


	//
	// set integer value
	//
	value_t operator=( int64_t arg)
	{
		iValue = arg;
		type = INTEGER;
		return *this;
	}

	//
	// set double value
	//
	value_t operator=( double arg)
	{
		fValue = arg;
		type = FLOATINGPOINT;
		return *this;
	}

	//
	// set boolean value
	//
	value_t operator=( bool arg)
	{
		bValue = arg;
		type = BOOLEAN;
		return *this;
	}

	//
	// checks equality with integer type
	//
	bool operator==( const int64_t arg )
	{
		if ( type == INTEGER )
			return (arg == iValue);

		if ( type == BOOLEAN )
			return (arg == static_cast<int64_t>(bValue));

		return (arg == static_cast<int64_t>(fValue));
	}

	//
	// checks equality with double type
	//
	bool operator==( const double arg )
	{
		if ( type == FLOATINGPOINT )
			return (arg == fValue);

		if ( type == BOOLEAN )
			return (arg == static_cast<double>(bValue));

		return (arg == static_cast<double>(iValue));
	}

	//
	// checks equality with boolean type
	//
	bool operator==( const bool arg )
	{
		if ( type == BOOLEAN )
			return (arg == bValue);

#pragma warning (push)
#pragma warning( disable:4800 ) // lost precision
		if ( type == INTEGER )
			return (arg == static_cast<bool>(iValue));

		return (arg == static_cast<bool>(fValue));
#pragma warning (pop)
	}

	//
	// cast ot double
	//
	operator double()
	{
		if ( type == FLOATINGPOINT )
			return fValue;
		if ( type == BOOLEAN )
			return static_cast<double>(bValue);
		return static_cast<double>(iValue);
	}

	//
	// cast to int
	//
	operator int64_t()
	{
		if ( type == INTEGER )
			return iValue;
		if ( type == BOOLEAN )
			return static_cast<int64_t>(bValue);
		return static_cast<int64_t>(fValue);
	}


	//
	// cast to bool
	//
	operator bool()
	{
		if ( type == BOOLEAN )
			return bValue;

#pragma warning (push)
#pragma warning( disable:4800 ) // lost precision
		if ( type == INTEGER )
			return static_cast<bool>(iValue);
		
		return static_cast<bool>(fValue);
#pragma warning (pop)
	}
};


bool operator==( const int64_t first, const value_t second );

value_t operator+(const value_t &first, const value_t &second);
value_t operator-(const value_t &first, const value_t &second);
value_t operator*(const value_t &first, const value_t &second);
value_t operator/(const value_t &first, const value_t &second);


} // namespace expressionEval
#endif