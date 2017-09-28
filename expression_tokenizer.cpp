#include "expression_tokenizer.h"

namespace expressionEval
{


const token_t ExpressionTokenizer::SENTINEL = {TOKEN_UNKNOWN, ""};

bracket_t ExpressionTokenizer::BRACKETS[] = 
{
	{TOKEN_LEFT_BRACKET, '('},
	{TOKEN_RIGHT_BRACKET, ')'},
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
* cuts string into tokens and returns an deque
* @returns:
*	deque of token_t
* @throws:
*	NONE
***************************************************************************************/
tokens_t ExpressionTokenizer::get_tokens( const std::string& expression, status_t &operationStatus )
{
	// clear data
	tokens_t tokens;

	m_expression = expression;

	try
	{
		//m_tokens = tokenize_expression( m_expression.begin(), m_expression.end(), operationStatus );
		const std::string sOperators("+-*/^%");

		istring_t it = m_expression.begin();

		while (it != m_expression.end() )
		{
			token_t token = {TOKEN_UNKNOWN, ""};

			// is it number?
			if ( isdigit( *it ) /*|| *c == '-' || *c == '+'*/ )
			{
				token.type = TOKEN_NUMBER;
				token.value = get_number( it, m_expression.end(), operationStatus );
			}
			// is it operator?
			else if ( sOperators.find(*it) != std::string::npos )
			{
				token.type = TOKEN_OPERATOR;
				token.value = *it;
#ifdef _DEBUG_MSG_
				std::cout << "operator: " << token.value << std::endl;
#endif
			} 
			// is it function?
			else if ( isalpha( *it ) )
			{
				token.type = TOKEN_FUNCTION;
				token.value = get_function_name( it, m_expression.end(), operationStatus );
			}
			// is it expresion inside parenthesis?
			else if (*it == '(')
			{
				token.type = TOKEN_LEFT_BRACKET;
#ifdef _DEBUG_MSG_
				std::cout << "Open parenthesis (" << std::endl;
#endif
			} 
			// does it close it? 
			else if (*it == ')')
			{
				token.type = TOKEN_RIGHT_BRACKET;
#ifdef _DEBUG_MSG_
				std::cout << "Close parenthesis" << std::endl;
#endif
			}

			if (!isspace(*it)) 
			{
				if ( token.type == TOKEN_UNKNOWN )
				{
					operationStatus.setFlag(EPARSING_FAILED_GENERAL);
					//operationStatus.setPosition( std::distance( m_expression.begin(), it ) );
					throw( parse_exception("Unknown token type", operationStatus ));
				}

				token.position = std::distance( m_expression.begin(), it );
				tokens.push_back( token );
			}

			std::advance(it, token.value.length() ? token.value.length() : 1);
		}

		tokens.push_back( SENTINEL );
		operationStatus.setFlag(EOK);
	}
	catch( parse_exception &e)
	{
		e = e;
		std::cout << "Parse error: " << e.what() << std::endl;
		//std::cout << std::string(operationStatus.getPosition(), ' ') << "^" << std::endl;
		//operationStatus = e.
	}

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
std::string ExpressionTokenizer::get_number( istring_t it, istring_t end, status_t &operationStatus)
{
	std::string valueString("");

	operationStatus.setFlag(EOK);

	// get all digits and dots
	while ( ((*it >= '0' && *it <= '9') || *it == '.') )
	{
		valueString += *it++;

		// passed end?
		if ( it == end )
		{ // expression ends with a number, that's ok
			break;
		}
	}

	if ( valueString.find_first_of('.') != valueString.find_last_of('.'))
	{ // two or more dots?
		operationStatus.setFlag(EFAIL);
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
std::string ExpressionTokenizer::get_function_name( istring_t it, istring_t end, status_t &operationStatus)
{
	// TODO 
	// handle parsing errors?
	std::string valueString("");

	operationStatus.setFlag(EOK);

	// get all letters/digits until it reaches parenthesis or strings ends
	while ( (isalpha(*it) || isdigit(*it))&& *it != '(' )
	{
		valueString += *it++;

		// passed end?
		if ( it == end ) 
		{ // expression ends with function name? bad idea!
			operationStatus.setFlag(EFAIL);
			break;
		}
	}

#ifdef _DEBUG_MSG_
	std::cout << "get_function_name: " << valueString << std::endl;
#endif
	
	return valueString;
}

} //namespace expressionEval

