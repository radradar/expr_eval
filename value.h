#ifndef _VALUE_OPAQUE_H_
#define _VALUE_OPAQUE_H_



//
// type of data
//
typedef enum
{
	UNKNOWN,
	INTEGER,
	FLOATINGPOINT
}valueType_t;

//
// holds int or float value and type of holding value 
//
struct value_t
{
	union
	{
		double	fValue;
		__int64 iValue;
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
		ret = (type == INTEGER )? static_cast<double>(iValue): fValue; 
		return ret;
	}

	//
	// cast to integer
	//
	__int64 toInteger() const 
	{ 
		__int64 ret=0L;
#pragma warning (push)
#pragma warning( disable:4244 ) // lost precision
		ret = (type == INTEGER )?  iValue: static_cast<__int64>(fValue);
#pragma warning (pop)
		return ret;
	}

	//
	// set integer value
	//
	value_t operator=( __int64 arg)
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
	// checks equality with integer type
	//
	bool operator==( const __int64 arg )
	{
		if ( type == INTEGER )
			return (arg == iValue);
		return (arg == static_cast<__int64>(fValue));
	}

	//
	// checks equality with double type
	//
	bool operator==( const double arg )
	{
		if ( type == FLOATINGPOINT )
			return (arg == fValue);
		return (arg == static_cast<double>(iValue));
	}

	//
	// cast ot double
	//
	operator double()
	{
		if ( type == FLOATINGPOINT )
			return fValue;
		return static_cast<double>(iValue);
	}

	//
	// cast to int
	//
	operator __int64()
	{
		if ( type == INTEGER )
			return iValue;
		return static_cast<__int64>(fValue);
	}

};


bool operator==( const __int64 first, const value_t second );

value_t operator+(const value_t &first, const value_t &second);
value_t operator-(const value_t &first, const value_t &second);
value_t operator*(const value_t &first, const value_t &second);
value_t operator/(const value_t &first, const value_t &second);


#endif