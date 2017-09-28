#include "expression.h"

//
// map of all available functions and theirs lambdas
//
map_functions_t availableFunctions::m_mf;

const availableFunctions Expression::m_availableFunction;

// all operators
/*const*/ operator_t Expression::OPERATORS[] = 
{
	{'^', 1, OPERATOR_BINARY, OPERATOR_RIGHT},
	{'+', 2, OPERATOR_UNARY,  OPERATOR_RIGHT},// znak przd wartoscia
	{'-', 2, OPERATOR_UNARY,  OPERATOR_RIGHT}, // znak przed wartoscia
	{'*', 3, OPERATOR_BINARY, OPERATOR_LEFT},
	{'/', 3, OPERATOR_BINARY, OPERATOR_LEFT},
	{'%', 3, OPERATOR_BINARY, OPERATOR_LEFT},
	{'+', 4, OPERATOR_BINARY, OPERATOR_LEFT},
	{'-', 4, OPERATOR_BINARY, OPERATOR_LEFT},
	{'(', 5, OPERATOR_OTHER,  OPERATOR_NONE},
	//{'[', 5, OPERATOR_OTHER,  OPERATOR_NONE},
	//{'{', 5, OPERATOR_OTHER,  OPERATOR_NONE}
};


/**************************************************************************************
*
* gets operator type 
* @returns:
*	operator type (unary/binary) 
*
***************************************************************************************/
operatorType_t Expression::get_arity(char oper, 
									 const token_t *previous) 
{
	if (previous->type == TOKEN_LEFT_BRACKET ||
		previous->type == TOKEN_OPERATOR || 
		previous->type == TOKEN_UNKNOWN )
		return OPERATOR_UNARY;
	return OPERATOR_BINARY;
}

/**************************************************************************************
*
* gets operator type 
* @returns:
*	pointer to operator definition
*
***************************************************************************************/
const operator_t *Expression::get_operator(char oper, 
										   operatorType_t arity) 
{
	for (size_t i = 0; i < sizeof OPERATORS / sizeof OPERATORS[0]; ++i) 
	{
		if (OPERATORS[i].oper == oper && OPERATORS[i].arity == arity)
			return &OPERATORS[i];
	}
	return NULL;
}


/**************************************************************************************
*
* gets value as string, checks if represents an integer or a float, then pushes it on a stack of operands
* @returns:
*	EOK
* @throws:
*	evaluation_exception
***************************************************************************************/
int Expression::push_number(const std::string &valueString, 
							operands_t &operands) 
{
	size_t idx;
	double fValue = 0.0f;
	__int64 iValue = 0L;
	value_t value;

	// TODO 
	// - replace comma with dot?

	if ( valueString.find_first_of('.') != std::string::npos )
	{ // floating point
		value.fValue = stod( valueString, &idx );
		if ( idx != valueString.length() )
			throw( evaluation_exception("value conversion failure ", fValue ));
		value.type = FLOATINGPOINT;
	}
	else
	{ // integer
		value.iValue = stol( valueString, &idx );
		if ( idx != valueString.length() )
			throw( evaluation_exception("value conversion failure ", fValue ));
		value.type = INTEGER;
	}

	operands.push( value );
	return EOK;
}

/**************************************************************************************
*
* applies unary opeartor to an operand
* @returns:
*	EOK
* @throws:
*	evaluation_exception, std::exception
***************************************************************************************/
int Expression::apply_unary_operator(const operator_t *oper, 
									 operands_t &operands) 
{
	if ( !operands.size() )
		throw( std::exception("operand missing"));

	value_t x = operands.top();
	operands.pop();

	switch (oper->oper) 
	{
	case '+': // '+' sign in front of a number doesn't make a difference
		break;
	case '-':
		if ( x.type == INTEGER )
			x.iValue = -x.iValue;
		else if ( x.type == FLOATINGPOINT )
			x.fValue = - x.fValue;
		else
			throw( std::exception("operation on unknown value"));
		break;

	default:
		throw( std::exception("unrecognized unary operator"));
	}
	operands.push(x);
	return EOK;
}


/**************************************************************************************
*
* applies binary operator to an operand
* @returns:
*	EOK
* @throws:
*	evaluation_exception, std::exception
***************************************************************************************/
int Expression::apply_binary_operator(const operator_t *oper, 
									  operands_t &operands) 
{
	if (!operands.size())
		throw ( std::exception("1st operand's missing"));

	value_t y = operands.top();
	operands.pop();

	if (!operands.size())
		throw ( std::exception("2nd operand's missing"));

	value_t x = operands.top();
	operands.pop();

	switch (oper->oper) 
	{
	case '+':
		x = x + y;
		break;
	case '-':
		x = x - y;
		break;
	case '*':
		x = x * y;
		break;
	case '/':
		x = x / y;
		break;
	case '%': // modulo
		{
			if ( (long)y.toInteger() == 0L )
				throw( evaluation_exception("fmod argument equals to 0", y.toDouble()));
			x = fmod(x.toInteger(), y.toInteger());
		}
		break;
	case '^': // power
		{ 
			if ( x.type == INTEGER && y.type == INTEGER )
				x = static_cast<double>(pow(x.toDouble(), y.toDouble()));
			else
				x = pow(x.toDouble(), y.toDouble());
		}
		break;
	default:
		throw( std::exception("unrecognized operator"));
	}

	operands.push(x);
	return EOK;
}


/**************************************************************************************
*
* detects kind of operator we are dealing with
* @returns:
*	EOK
* @throws:
*	evaluation_exception
***************************************************************************************/
int Expression::apply_operator(const operator_t *oper, 
							   operands_t &operands) 
{
	if ( !oper )
		throw( evaluation_exception("operator is missing!"));

	if ( !operands.size())
		throw( evaluation_exception("operands is missing!"));

	if (oper->arity == OPERATOR_UNARY)
		return apply_unary_operator(oper, operands);
	else
		return apply_binary_operator(oper, operands);
}


/**************************************************************************************
*
* calls specified in operand functions from a map op available functions
* @returns:
*	EOK
* @throws:
*	evaluation_exception
***************************************************************************************/
int Expression::apply_function(const std::string &function, 
							   operands_t &operands) 
{
	if (!operands.size())
		throw( evaluation_exception("No functions arguments", 0));

	value_t x = operands.top();
	operands.pop();

	status_t operationStatus;
	auto found_function = availableFunctions::get( function, operationStatus );

	if ( operationStatus.getFlag() == EOK )
	{
		if ( x.type == INTEGER )
			x = found_function(x.toDouble());
		else
			x = found_function(x.fValue);// REMARK!!! all functions returns and works with double!
	}
	else
	{
		throw( std::exception("undefined function"));
	}

	operands.push(x);
	return EOK;
}


/**************************************************************************************
*
* adds an operator to a operators stack
* @returns:
*	EOK
* @throws:
*	evaluation_exception, std::exception
***************************************************************************************/
int Expression::push_operator(const operator_t *oper, 
							  operands_t &operands,
							  operators_t &operators) 
{
	if (!oper)
		throw( std::exception("operator missing"));

	int Status = 0;
	while (operators.size() && Status == 0) 
	{
		const operator_t &stack_operator = operators.top();
		if (oper->arity == OPERATOR_UNARY ||
			oper->precedence < stack_operator.precedence ||
			(oper->associativity == OPERATOR_RIGHT &&
			oper->precedence == stack_operator.precedence))
			break;

		Status = apply_operator(&operators.top(), operands);
		operators.pop();
	}

	operators.push( *oper);
	return Status;
}

/**************************************************************************************
*
* iterates through tokens pushing on selected stack dependig on type of token & evaluates values of (operator | functions ) & operands
* @returns:
*	void
* @throws:
*	evaluation_exception
***************************************************************************************/
void Expression::parse( tokens_t & tokens, 
					   operands_t &operands, 
					   operators_t &operators, 
					   functions_t &functions, 
					   status_t &operationStatus)
{
	if ( !tokens.size() )
	{
		throw std::exception("no tokens available");
	}

	auto token = tokens.front();

	token_t previous = ExpressionTokenizer::SENTINEL;

	while( token.type != TOKEN_UNKNOWN)
	{
		m_lastParsedTokenPosition = token.position;

		switch( token.type )
		{
		case TOKEN_NUMBER:				
			if (previous.type == TOKEN_NUMBER ||
				previous.type == TOKEN_FUNCTION ||
				previous.type == TOKEN_RIGHT_BRACKET )
			{
				throw( parse_exception("syntax error - no operator?", status_t(EPARSING_FAILED_GENERAL)));
			}
			else 
			{
				push_number(token.value, operands);
			}
			break;

		case TOKEN_LEFT_BRACKET:	
			operators.push(*get_operator('(', OPERATOR_OTHER) );
			break;

		case TOKEN_RIGHT_BRACKET:	
			{
				bool found_parenthesis = false;
				while (operators.size() && !found_parenthesis) 
				{
					const operator_t &oper = operators.top();
					operators.pop();

					if (oper.oper == '(')
						found_parenthesis = true;
					else
						apply_operator(&oper, operands);
				}

				if (!found_parenthesis)
					throw( parse_exception("No corresponding parenthesis", status_t(EPARSING_FAILED_GENERAL, token.position)));
				else 
					if (functions.size())
					{
						apply_function(functions.top(), operands);
						functions.pop();
					}
			}
			break;

		case TOKEN_OPERATOR:
			push_operator(
				get_operator(token.value[0],
				get_arity(token.value[0], &previous)),
				operands, 
				operators);
			break;

		case TOKEN_FUNCTION:
			functions.push(token.value);
			break;

		default:
			throw( std::exception("unknown token"));
			break;

		}

		previous = token;
		tokens.pop_front();
		token = tokens.front();
	}


	// Apply all remaining operators.
	while (operators.size() && operationStatus.getFlag() == EOK) 
	{
		const operator_t &oper = operators.top();
		operators.pop();
		if (oper.oper == '(')
			//operationStatus.setFlag( EFAIL );
			throw( parse_exception("No corresponding parenthesis", status_t(EPARSING_FAILED_NO_CLOSING_PARENTHESIS, token.position )));

		else
			apply_operator(&oper, operands);
	}
}


/**************************************************************************************
*
* main function, clears stacks, inserts expression and goes through parsed tokens computing operands values
* @returns:
*	value_t with calculated value and status
* @throws:
*	evaluation_exception
***************************************************************************************/
value_t Expression::evaluate( const std::string& expression, status_t &operationStatus )
{
	// clear all necessary containers
	m_tokens.clear();

	while( m_operands.size() )
		m_operands.pop();

	while( m_operators.size() )
		m_operators.pop();

	while( m_functions.size() )
		m_functions.pop();

	// get tokens
	m_tokens = m_tokenizer.get_tokens( expression, operationStatus );

	value_t answer;

	if ( operationStatus.getFlag() == EOK )
	{
		if ( m_tokens.size() <= 1 )
		{ // tokens missing
			operationStatus.setFlag( EPARSING_FAILED_GENERAL );
			return answer;
		}

		try
		{
			// parser tokens -> 
			parse( m_tokens, m_operands, m_operators, m_functions, operationStatus );

			// TODO 
			// - update answer to nan?
			// check if all elements are consumed except one operand which is real answer
			if ( (m_operands.size() == 1) && ( m_tokens.size() == 1/*unknown left*/ && !m_operators.size() && !m_functions.size() ) )
			{
				answer = m_operands.top();
			}
			else
			{
				//operationStatus.setFlag( EFAIL );
				if ( m_operands.size() != 1 )
				{
					operationStatus.setFlag( EEVALUATION_FAILED_OPERANDS_NOT_CONSUMED );
					std::cerr << "Parsing error: " << " operand(s) left on stack" << std::endl;
				}
				else
					if ( m_tokens.size() != 1 ) /*unknown left*/
					{
						operationStatus.setFlag( EEVALUATION_FAILED_TOKENS_NOT_CONSUMED );
						std::cerr << "Parsing error: " << " token(s) left on stack" << std::endl;
					}
					else
						if ( m_operators.size() )
						{
							operationStatus.setFlag( EEVALUATION_FAILED_OPERATORS_NOT_CONSUMED );
							std::cerr << "Parsing error: " << " operator(s) left on stack" << std::endl;
						}
						else
							if ( m_functions.size() )
							{
								operationStatus.setFlag( EEVALUATION_FAILED_FUNCTION_NOT_CONSUMED );
								std::cerr << "Parsing error: " << " function(s) left on stack" << std::endl;
							}
			}	
		}
		catch( const evaluation_exception &e)
		{
			operationStatus.setFlag(EEVALUATION_FAILED_GENERAL);
			std::cerr << "Evaluation error: " << e.what() << ", near position = " << get_last_eval_position() << std::endl;
			//std::cout << std::string(operationStatus.getPosition(), ' ') << "^" << std::endl;
		}
		catch( const std::exception &e )
		{
			operationStatus.setFlag(EEVALUATION_FAILED_GENERAL);
			std::cerr << "Evaluation error: " << e.what() << ", near position = " << get_last_eval_position() << std::endl;
			//std::cout << std::string(get_last_eval_position(), ' ') << "^" << std::endl;			
		}

#ifdef _DEBUG_MSG_
		std::cout << "stack tokens: " << m_tokens.size() << std::endl;
		std::cout << "stack operands: " << m_operands.size() << std::endl;
		std::cout << "stack operators: " << m_operators.size() << std::endl;
		std::cout << "stack functiosn: " << m_functions.size() << std::endl;
#endif
	}
	else
	{
		std::cerr << "Tokenize error: " << ", near position = " << get_last_eval_position() << std::endl;
		//std::cout << std::string(operationStatus.getPosition(), ' ') << "^" << std::endl;
	}

	return answer;
}


/**************************************************************************************
*
* load data from precedence.cfg file and updates OPERATORS array with new precedences
* @returns:
*	number of successed applied changes
* @throws:
*	NONE
***************************************************************************************/
size_t Expression::updateOperatorPrecedence()
{
	std::ifstream ifs("precedence.cfg");
	size_t success=0;
	if ( ifs.is_open())
	{
		while(true)
		{
			std::string line;
			std::getline(ifs, line );

			std::regex base_regex("^([^+-*\\/%]){1}=(\\d{1,2}){1}$"); // regex for symbol =  number (max 2 digits)
			std::smatch base_match;

			if (std::regex_match(line, base_match, base_regex)) 
			{
				// The first sub_match is the whole string then symbol and value
				if (base_match.size() == 3) 
				{
					std::ssub_match base_sub_match1 = base_match[1];
					std::ssub_match base_sub_match2 = base_match[2];
					char symbol = base_sub_match1.str().at(0);
					int precedence = stoi(base_sub_match2.str(), NULL);
					//std::string base1 = base_sub_match1.str();
					//std::string base2 = base_sub_match2.str();
					//std::cout << symbol << " = " << precedence << '\n';

					// replace precedence
					for (size_t i = 0; i < sizeof OPERATORS / sizeof OPERATORS[0]; ++i) 
					{
						if ( OPERATORS[i].oper == symbol )
						{
							OPERATORS[i].precedence = precedence;
							++success;
							break;
						}
					}
				} // if (base_match.size() == 3) 
			} // regex_match...

			// EOF ?
			if ( ifs.eof() ) 
				break;
		}// while(true)
	}

	return success;
}


