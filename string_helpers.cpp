#include <string>

#include "string_helpers.h"

#define DEF_STR_WHITECHARS   " \n\r\t\a\f\v"


std::string& ltrim(std::string& inString)
{
	inString.erase(0, inString.find_first_not_of(DEF_STR_WHITECHARS));
	return inString;
}

std::string& rtrim(std::string& inString)
{
	inString.erase(inString.find_last_not_of(DEF_STR_WHITECHARS) + 1);
	return inString;
}

std::string& trim(std::string& inString)
{
	return rtrim(ltrim(inString));
}
