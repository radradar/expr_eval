#ifndef _VALUE_OPAQUE_H_
#define _VALUE_OPAQUE_H_

#include <stdint.h>

//#if __GNUC__ > 4 || (__GNUC__ == 4 && __GNUC_MINOR__ >= 5 ) || _MSC_VER >= 1700 
//#define ENUM(arg) enum class arg
//#else
//#define ENUM(arg) typedef enum arg
//#endif

namespace expressionEval
{

//
// type of data
//
enum class valueType_t
{
	UNKNOWN,
	INTEGER,
	FLOATINGPOINT,
	BOOLEAN,
};

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

	value_t():iValue(0L), type(valueType_t::UNKNOWN)
	{}

	//
	// cast to double
	//
	double toDouble() const 
	{ 
		double ret=0.0f;
		
		if (type == valueType_t::INTEGER )
			ret = static_cast<double>(iValue);
		else
		if (type == valueType_t::BOOLEAN )
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
		if (type == valueType_t::INTEGER )
			ret = iValue;
			else
			if ( type == valueType_t::BOOLEAN )
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
		type = valueType_t::INTEGER;
		return *this;
	}

	//
	// set double value
	//
	value_t operator=( double arg)
	{
		fValue = arg;
		type = valueType_t::FLOATINGPOINT;
		return *this;
	}

	//
	// set boolean value
	//
	value_t operator=( bool arg)
	{
		bValue = arg;
		type = valueType_t::BOOLEAN;
		return *this;
	}

	//
	// checks equality with integer type
	//
	bool operator==( const int64_t arg )
	{
		if ( type == valueType_t::INTEGER )
			return (arg == iValue);

		if ( type == valueType_t::BOOLEAN )
			return (arg == static_cast<int64_t>(bValue));

		return (arg == static_cast<int64_t>(fValue));
	}

	//
	// checks equality with double type
	//
	bool operator==( const double arg )
	{
		if ( type == valueType_t::FLOATINGPOINT )
			return (arg == fValue);

		if ( type == valueType_t::BOOLEAN )
			return (arg == static_cast<double>(bValue));

		return (arg == static_cast<double>(iValue));
	}

	//
	// checks equality with boolean type
	//
	bool operator==( const bool arg )
	{
		if ( type == valueType_t::BOOLEAN )
			return (arg == bValue);

#pragma warning (push)
#pragma warning( disable:4800 ) // lost precision
		if ( type == valueType_t::INTEGER )
			return (arg == static_cast<bool>(iValue));

		return (arg == static_cast<bool>(fValue));
#pragma warning (pop)
	}

	//
	// cast ot double
	//
	operator double()
	{
		if ( type == valueType_t::FLOATINGPOINT )
			return fValue;
		if ( type == valueType_t::BOOLEAN )
			return static_cast<double>(bValue);
		return static_cast<double>(iValue);
	}

	//
	// cast to int
	//
	operator int64_t()
	{
		if ( type == valueType_t::INTEGER )
			return iValue;
		if ( type == valueType_t::BOOLEAN )
			return static_cast<int64_t>(bValue);
		return static_cast<int64_t>(fValue);
	}


	//
	// cast to bool
	//
	operator bool()
	{
		if ( type == valueType_t::BOOLEAN )
			return bValue;

#pragma warning (push)
#pragma warning( disable:4800 ) // lost precision
		if ( type == valueType_t::INTEGER )
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