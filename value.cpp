#include "value.h"

namespace expressionEval
{

//
// adds 2 values, depends on type of arguments
//
value_t operator+(const value_t &first, const value_t &second)
{
	// TODO: how to add unknown types?
	if ( first.type == valueType_t::UNKNOWN || second.type == valueType_t::UNKNOWN )
		return first;

	value_t ret;

	if (first.type == valueType_t::INTEGER )
	{
		if ( second.type == valueType_t::INTEGER )
			ret = first.iValue + second.iValue;
		else
			ret = first.toDouble() + second.fValue;
	}else
	{
		if ( second.type == valueType_t::INTEGER )
			ret = first.fValue + second.toDouble();
		else
			ret = first.fValue + second.fValue;
	}
	return ret;
}

//
// subtracts 2 values, depends on type of arguments
//
value_t operator-(const value_t &first, const value_t &second)
{
	// TODO: how to sub unknown types?
	if ( first.type == valueType_t::UNKNOWN || second.type == valueType_t::UNKNOWN )
		return first;

	value_t ret;

	if (first.type == valueType_t::INTEGER )
	{
		if ( second.type == valueType_t::INTEGER )
			ret = first.iValue - second.iValue;
		else
			ret = first.toDouble() - second.fValue;
	}else
	{
		if ( second.type == valueType_t::INTEGER )
			ret = first.fValue - second.toDouble();
		else
			ret = first.fValue - second.fValue;
	}
	return ret;
}

	
//
// multiplies 2 values, depends on type of arguments
//
value_t operator*(const value_t &first, const value_t &second)
{
	// TODO: how to mul unknown types?
	if ( first.type == valueType_t::UNKNOWN || second.type == valueType_t::UNKNOWN )
		return first;

	value_t ret;

	if (first.type == valueType_t::INTEGER )
	{
		if ( second.type == valueType_t::INTEGER )
			ret = first.iValue * second.iValue;
		else
			ret = first.toDouble() * second.fValue;
	}else
	{
		if ( second.type == valueType_t::INTEGER )
			ret = first.fValue * second.toDouble();
		else
			ret = first.fValue * second.fValue;
	}
	return ret;
}


//
// divides 2 values, depends on type of arguments
//
value_t operator/(const value_t &first, const value_t &second)
{
	// TODO: how to div unknown types?
	if ( first.type == valueType_t::UNKNOWN || second.type == valueType_t::UNKNOWN )
		return first;

	value_t ret;

	if (first.type == valueType_t::INTEGER )
	{
		if ( second.type == valueType_t::INTEGER )
			ret = first.iValue / second.iValue;
		else
			ret = first.toDouble() / second.fValue;
	}else
	{
		if ( second.type == valueType_t::INTEGER )
			ret = first.fValue / second.toDouble();
		else
			ret = first.fValue / second.fValue;
	}
	return ret;
}

//
// compare value_t object with integer value
//
bool operator==( const int64_t first, const value_t second )
{
	return (first == second.toInteger());
}

} //namespace expressionEval

