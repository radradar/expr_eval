#include "expression_tokenizer.h"

#include "string_helpers.h"

namespace expressionEval
{


const token_t ExpressionTokenizer::SENTINEL = {tokenType_t::ETOKEN_UNKNOWN, ""};

bracket_t ExpressionTokenizer::BRACKETS[] = 
{
	{tokenType_t::ETOKEN_LEFT_BRACKET, '('},
	{tokenType_t::ETOKEN_RIGHT_BRACKET, ')'},
};

/**************************************************************************************
*
* c-tor
* @returns:
*	NONE
* @throws:
*	NONE
***************************************************************************************/
ExpressionTokenizer::ExpressionTokenizer( const std::string &expression ): m_expression(expression)
{
}


/**************************************************************************************
*
* check expression string for validity
* @returns:
*	prepared string
* @throws:
*	evaluation_exception
***************************************************************************************/
std::string ExpressionTokenizer::prepare_expression( const std::string& expression, Status &operationStatus )
{
	std::string inputString(expression);

	// trim white chars
	inputString = trim( inputString );
	// to lower
	std::transform( inputString.begin(), inputString.end(), inputString.begin(), ::tolower );
	// TODO:
	// -remove unnecessary spaces inside?
	// -check for unknown symbols != ( alpha + digit + operator + parentheses )

	return inputString;
}

/**************************************************************************************
*
* cuts string into tokens and returns an deque of tokens
* @returns:
*	deque of token_t
* @throws:
*	NONE
***************************************************************************************/
tokens_t ExpressionTokenizer::get_tokens( const std::string& expression, Status &operationStatus )
{
	// clear data
	tokens_t tokens;

	operationStatus.setFlag(ecode_t::EOK);

	m_expression = prepare_expression(expression, operationStatus);

	if ( operationStatus.getFlag() != ecode_t::EOK )
	{ // failure
		return tokens;
	}

	const std::string sOperators("+-*/^%");

	istring_t it = m_expression.begin();

	while (it != m_expression.end() )
	{
		token_t token = {tokenType_t::ETOKEN_UNKNOWN, ""};

		// is it number?
		if ( isdigit( *it ) /*|| *c == '-' || *c == '+'*/ )
		{
			token.type = tokenType_t::ETOKEN_NUMBER;
			token.value = get_number( it, m_expression.end(), operationStatus );
			if ( operationStatus.getFlag() != ecode_t::EOK )
			{ // somthing went wrong when reading number
				operationStatus.setPosition( std::distance( m_expression.begin(), it ) + token.value.length() );
				break;			
			}
		}
		// is it operator?
		else if ( sOperators.find(*it) != std::string::npos )
		{
			token.type = tokenType_t::ETOKEN_OPERATOR;
			token.value = *it;
#ifdef _DEBUG_MSG_
			std::cout << "operator: " << token.value << std::endl;
#endif
		} 
		// is it function?
		else if ( isalpha( *it ) )
		{
			token.type = tokenType_t::ETOKEN_FUNCTION;
			token.value = get_function_name( it, m_expression.end(), operationStatus );
			if ( operationStatus.getFlag() != ecode_t::EOK )
			{ // parse function failed
				operationStatus.setPosition( std::distance( m_expression.begin(), it ) + token.value.length() );
				break;
			}
		}
		// is it expresion inside parenthesis?
		else if (*it == '(')
		{
			token.type = tokenType_t::ETOKEN_LEFT_BRACKET;
#ifdef _DEBUG_MSG_
			std::cout << "Open parenthesis (" << std::endl;
#endif
		} 
		// does it close it? 
		else if (*it == ')')
		{
			token.type = tokenType_t::ETOKEN_RIGHT_BRACKET;
#ifdef _DEBUG_MSG_
			std::cout << "Close parenthesis" << std::endl;
#endif
		}

		if (!isspace(*it)) 
		{
			if ( token.type == tokenType_t::ETOKEN_UNKNOWN )
			{ // failure, set reason flag & position
				operationStatus.set(ecode_t::EPARSING_FAILED_UNKNOWN_OPERATOR, std::distance( m_expression.begin(), it ));
				break;
			}

			token.position = std::distance( m_expression.begin(), it );
			tokens.push_back( token );
		}

		// move iterator forward
		std::advance(it, token.value.length() ? token.value.length() : 1);
	}

	tokens.push_back( SENTINEL );

	return tokens;
}

/**************************************************************************************
*
* tries to read number from string and convert to a number
* @returns:
*	string representation of a number
* @throws:
*	NONE
***************************************************************************************/
std::string ExpressionTokenizer::get_number( istring_t it, istring_t end, Status &operationStatus)
{
	std::string valueString("");

	operationStatus.setFlag(ecode_t::EOK);

	// get all digits and dots
	while ( ((*it >= '0' && *it <= '9') || *it == '.') )
	{
		valueString += *it++;

		// passed end?
		if ( it == end )
		{ // expression ends with a number i.e "2+3", that's ok
			break;
		}
	}

	if ( valueString.find_first_of('.') != valueString.find_last_of('.'))
	{ // two or more dots?
		operationStatus.setFlag(ecode_t::EPARSING_FAILED_NUMBER_READ);
	}

#ifdef _DEBUG_MSG_
	std::cout << "get_number: " << valueString << std::endl;
#endif
	return valueString;
}


/**************************************************************************************
*
* tries to read a function name from a string (until reaches '(' or end of string)
* @returns:
*	function name
* @throws:
*	NONE
***************************************************************************************/
std::string ExpressionTokenizer::get_function_name( istring_t it, istring_t end, Status &operationStatus)
{
	// TODO 
	// handle parsing errors?
	std::string valueString("");

	operationStatus.setFlag(ecode_t::EOK);

	// get all letters/digits until it reaches parenthesis or strings ends
	// i.e. sin(, log10( etc
	while ( (isalpha(*it) || isdigit(*it)) && *it != '(' )
	{
		valueString += *it++;

		// passed end?
		if ( it == end ) 
		{ // expression ends with function name i.e. "2*sin"? bad idea!
			operationStatus.setFlag(ecode_t::EPARSING_FAILED_FUNCTION_ARGUMENT_MISSING);
			break;
		}
	}

#ifdef _DEBUG_MSG_
	std::cout << "get_function_name: " << valueString << std::endl;
#endif
	
	return valueString;
}

} //namespace expressionEval


