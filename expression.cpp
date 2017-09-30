#include "expression.h"


// TODO


namespace expressionEval
{

//
// map of all available functions and theirs lambdas
//
map_functions_t availableFunctions::m_mf;

const availableFunctions Expression::m_availableFunction;

// all operators
/*const*/ operator_t expressionEval::Expression::OPERATORS[] = 
{
	{'^', 1, operatorType_t::EOPERATOR_BINARY, operatorAssociativity_t::EOPERATOR_RIGHT},
	{'+', 2, operatorType_t::EOPERATOR_UNARY,  operatorAssociativity_t::EOPERATOR_RIGHT},// znak przd wartoscia
	{'-', 2, operatorType_t::EOPERATOR_UNARY,  operatorAssociativity_t::EOPERATOR_RIGHT}, // znak przed wartoscia
	{'*', 3, operatorType_t::EOPERATOR_BINARY, operatorAssociativity_t::EOPERATOR_LEFT},
	{'/', 3, operatorType_t::EOPERATOR_BINARY, operatorAssociativity_t::EOPERATOR_LEFT},
	{'%', 3, operatorType_t::EOPERATOR_BINARY, operatorAssociativity_t::EOPERATOR_LEFT},
	{'+', 4, operatorType_t::EOPERATOR_BINARY, operatorAssociativity_t::EOPERATOR_LEFT},
	{'-', 4, operatorType_t::EOPERATOR_BINARY, operatorAssociativity_t::EOPERATOR_LEFT},
	{'(', 5, operatorType_t::EOPERATOR_OTHER,  operatorAssociativity_t::EOPERATOR_NONE},
	//{'[', 5, tokenType_t::operatorType_t::EOPERATOR_OTHER,  operatorAssociativity_t::EOPERATOR_NONE},
	//{'{', 5, tokenType_t::operatorType_t::EOPERATOR_OTHER,  operatorAssociativity_t::EOPERATOR_NONE}
};


/**************************************************************************************
*
* gets operator type 
* @returns:
*	operator type (unary/binary) 
*
***************************************************************************************/
operatorType_t Expression::get_arity(const char &oper, 
									 const token_t *previous) 
{
	if (previous->type == tokenType_t::ETOKEN_LEFT_BRACKET ||
		previous->type == tokenType_t::ETOKEN_OPERATOR || 
		previous->type == tokenType_t::ETOKEN_UNKNOWN ) // is it possible?
		return operatorType_t::EOPERATOR_UNARY;
	return operatorType_t::EOPERATOR_BINARY;
}

/**************************************************************************************
*
* gets operator type 
* @returns:
*	pointer to operator definition
*
***************************************************************************************/
const operator_t *Expression::get_operator(const char &oper, 
										   const operatorType_t &arity) 
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
*	ecode_t::EOK
* @throws:
*	evaluation_exception
***************************************************************************************/
int Expression::push_number(const std::string &valueString, 
							operands_t &operands) 
{
	size_t idx;
	value_t value;

	// TODO 
	// - replace comma with dot?

	if ( valueString.find_first_of('.') != std::string::npos )
	{ // floating point
		try
		{
			value.fValue = stod( valueString, &idx );
			if ( idx != valueString.length() )
				throw( evaluation_exception("value conversion string -> double failure ", Status(ecode_t::EEVALUATION_CONVERSION_FAILURE) ));
			value.type = valueType_t::FLOATINGPOINT;
		}
		catch( const std::invalid_argument & )
		{
			throw ( evaluation_exception("invalid argument string->double)", Status(ecode_t::EEVALUATION_CONVERSION_FAILURE ))); 
		}
		catch( const std::out_of_range & )
		{
			throw ( evaluation_exception("invalid argument string->double)", Status(ecode_t::EEVALUATION_CONVERSION_FAILURE ))); 
		}	
	}
	else
	{ // integer
		try
		{
			value.iValue = stol( valueString, &idx );
			if ( idx != valueString.length() )
				throw( evaluation_exception("value conversion string -> integer point failure ",Status(ecode_t::EEVALUATION_CONVERSION_FAILURE) ));
			value.type = valueType_t::INTEGER;
		}
		catch( const std::invalid_argument & )
		{
			throw ( evaluation_exception("invalid argument string->integer)", Status(ecode_t::EEVALUATION_CONVERSION_FAILURE ))); 
		}
		catch( const std::out_of_range & )
		{
			throw ( evaluation_exception("invalid argument string->integer)", Status(ecode_t::EEVALUATION_CONVERSION_FAILURE ))); 
		}
	}

	operands.push( value );
	return static_cast<int>(ecode_t::EOK);
}

/**************************************************************************************
*
* applies unary opeartor to an operand
* @returns:
*	ecode_t::EOK
* @throws:
*	evaluation_exception
***************************************************************************************/
int Expression::apply_unary_operator(const operator_t *oper, 
									 operands_t &operands) 
{
	if ( !operands.size() )
		throw( evaluation_exception("operands missing", Status( ecode_t::EEVALUATION_FAILED_NO_OPERANDS)));

	value_t x = operands.top();
	operands.pop();

	switch (oper->oper) 
	{
	case '+': // '+' sign in front of a number doesn't make a difference
		break;
	case '-':
		if ( x.type == valueType_t::INTEGER )
			x.iValue = -x.iValue;
		else if ( x.type == valueType_t::FLOATINGPOINT )
			x.fValue = -x.fValue;
		else
			throw( evaluation_exception("operation on value of a unknown type ", Status(ecode_t::EEVALUATION_CONVERSION_FAILURE)));
		break;

	default:
		throw( evaluation_exception("unrecognized unary operator", Status(ecode_t::EEVALUATION_FAILED_UNKNOW_OPERATOR)));
	}
	operands.push(x);
	return static_cast<int>(ecode_t::EOK);
}


/**************************************************************************************
*
* applies binary operator to an operand
* @returns:
*	ecode_t::EOK
* @throws:
*	evaluation_exception
***************************************************************************************/
int Expression::apply_binary_operator(const operator_t *oper, 
									  operands_t &operands) 
{
	if (!operands.size())
		throw ( evaluation_exception("1st operand's missing", Status( ecode_t::EEVALUATION_FAILED_NO_OPERANDS)));

	value_t y = operands.top();
	operands.pop();

	if (!operands.size())
		throw ( evaluation_exception("2nd operand's missing", Status( ecode_t::EEVALUATION_FAILED_NO_OPERANDS)));

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
		{
			if ( y.type == valueType_t::INTEGER )
			{
				if ( y.iValue == 0L )
					throw( evaluation_exception("divide by 0 (integer)", Status(ecode_t::EEVALUATION_FAILED_ARGUMENT_ERROR)));
			}else
				if ( y.type == valueType_t::FLOATINGPOINT )
				{
					if (y.fValue == 0.0f )
						throw( evaluation_exception("divide by 0.0 (float)", Status(ecode_t::EEVALUATION_FAILED_ARGUMENT_ERROR)));
				}
		x = x / y;
		}
		break;
	case '%': // modulo
		{
			if ( (long)y.toInteger() == 0L )
				throw( evaluation_exception("fmod argument equals to 0", Status(ecode_t::EEVALUATION_FAILED_ARGUMENT_ERROR)));
			x = fmod(x.toInteger(), y.toInteger());
		}
		break;
	case '^': // power
		{ 
			if ( x.type == valueType_t::INTEGER && y.type == valueType_t::INTEGER )
				x = static_cast<double>(pow(x.toDouble(), y.toDouble()));
			else
				x = pow(x.toDouble(), y.toDouble());
		}
		break;
	default:
		throw( evaluation_exception("unrecognized operator", Status(ecode_t::EEVALUATION_FAILED_UNKNOW_OPERATOR)));
		break;
	}

	operands.push(x);
	return static_cast<int>(ecode_t::EOK);
}


/**************************************************************************************
*
* detects kind of operator we are dealing with
* @returns:
*	ecode_t::EOK
* @throws:
*	evaluation_exception
***************************************************************************************/
int Expression::apply_operator(const operator_t *oper, 
							   operands_t &operands) 
{
	if ( !oper )
		throw( evaluation_exception("operator is missing!", Status(ecode_t::EEVALUATION_FAILED_NO_OPERATORS)));

	if ( !operands.size())
		throw( evaluation_exception("operands is missing!", Status(ecode_t::EEVALUATION_FAILED_NO_OPERANDS)));

	if (oper->arity == operatorType_t::EOPERATOR_UNARY)
		return apply_unary_operator(oper, operands);
	else
		return apply_binary_operator(oper, operands);
}


/**************************************************************************************
*
* calls specified in operand functions from a map op available functions
* @returns:
*	ecode_t::EOK
* @throws:
*	evaluation_exception
***************************************************************************************/
int Expression::apply_function(const std::string &function, 
							   operands_t &operands) 
{
	if (!operands.size())
		throw( evaluation_exception("No functions arguments", Status(ecode_t::EEVALUATION_FAILED_ARGUMENT_ERROR)));

	value_t x = operands.top();
	operands.pop();

	Status operationStatus;
	auto found_function = availableFunctions::get( function, operationStatus );

	if ( operationStatus.getFlag() == ecode_t::EOK )
	{
		if ( x.type == valueType_t::INTEGER )
			x = found_function(x.toDouble());
		else
			x = found_function(x.fValue);// REMARK!!! all functions returns and works with double!
	}
	else
	{
		throw( evaluation_exception("undefined function", Status(ecode_t::EEVALUATION_FAILED_UNKNOW_FUNCTION)));
	}

	operands.push(x);
	return static_cast<int>(ecode_t::EOK);
}


/**************************************************************************************
*
* adds an operator to a operators stack
* @returns:
*	ecode_t::EOK
* @throws:
*	evaluation_exception
***************************************************************************************/
int Expression::push_operator(const operator_t *oper, 
							  operands_t &operands,
							  operators_t &operators) 
{
	if (!oper)
		throw( evaluation_exception("operator missing", Status(ecode_t::EEVALUATION_FAILED_NO_OPERATORS)));

	int Status = 0;
	while (operators.size() && Status == 0) 
	{
		const operator_t &stack_operator = operators.top();
		if (oper->arity == operatorType_t::EOPERATOR_UNARY ||
			oper->precedence < stack_operator.precedence ||
			(oper->associativity == operatorAssociativity_t::EOPERATOR_RIGHT &&
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
void Expression::calculate( tokens_t & tokens, 
					   operands_t &operands, 
					   operators_t &operators, 
					   functions_t &functions, 
					   Status &operationStatus)
{
	if ( !tokens.size() )
	{
		operationStatus.setFlag( ecode_t::EEVALUATION_FAILED_NO_TOKENS);
		throw evaluation_exception("no tokens available", operationStatus);
	}

	auto token = tokens.front();

	token_t previous = ExpressionTokenizer::SENTINEL;

	// for all tokens
	while( token.type != tokenType_t::ETOKEN_UNKNOWN)
	{
		m_lastParsedTokenPosition = token.position;

		switch( token.type )
		{
		case tokenType_t::ETOKEN_NUMBER:				
			if (previous.type == tokenType_t::ETOKEN_NUMBER ||
				previous.type == tokenType_t::ETOKEN_FUNCTION ||
				previous.type == tokenType_t::ETOKEN_RIGHT_BRACKET )
			{
				operationStatus.set(ecode_t::EPARSING_FAILED_GENERAL, token.position);
				throw( evaluation_exception("syntax error - no operator?", operationStatus));
			}
			else 
			{
				push_number(token.value, operands);
			}
			break;

		case tokenType_t::ETOKEN_LEFT_BRACKET:	
			operators.push(*get_operator('(', operatorType_t::EOPERATOR_OTHER) );
			break;

		case tokenType_t::ETOKEN_RIGHT_BRACKET:	
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
				{
					operationStatus.set(ecode_t::EPARSING_FAILED_NO_CLOSING_PARENTHESIS, token.position);
					throw( evaluation_exception("No closing parenthesis", operationStatus));
				}
				else 
					if (functions.size())
					{
						apply_function(functions.top(), operands);
						functions.pop();
					}
			}
			break;

		case tokenType_t::ETOKEN_OPERATOR:
			push_operator(
				get_operator(token.value[0],
				get_arity(token.value[0], &previous)),
				operands, 
				operators);
			break;

		case tokenType_t::ETOKEN_FUNCTION:
			functions.push(token.value);
			break;

		default:
			operationStatus.set(ecode_t::EPARSING_FAILED_NO_CLOSING_PARENTHESIS, token.position);
			throw( evaluation_exception("Unknown token", operationStatus));
			break;

		}

		previous = token;

		// remove top token
		tokens.pop_front();

		// get next token
		token = tokens.front();
	}


	// Apply all remaining operators.
	while (operators.size() && operationStatus.getFlag() == ecode_t::EOK) 
	{
		const operator_t oper = operators.top();
		operators.pop();
		if (oper.oper == '(')
		{
			operationStatus.set( ecode_t::EFAIL ,token.position);
			throw( evaluation_exception("No closing parenthesis", operationStatus));
		}
		else
		{
			apply_operator(&oper, operands);
		}
	}
}


/**************************************************************************************
*
* clears all containers etc
* @returns:
*	void
* @throws:
*	no throw
***************************************************************************************/
void Expression::prepare_for_evaluation()
{

	// clear all necessary containers
	m_tokens.clear();

	while( m_operands.size() )
		m_operands.pop();

	while( m_operators.size() )
		m_operators.pop();

	while( m_functions.size() )
		m_functions.pop();

}

/**************************************************************************************
*
* main function, clears stacks, inserts expression and goes through parsed tokens computing operands values
* @returns:
*	value_t with calculated value and status
* @throws:
*	evaluation_exception
***************************************************************************************/
value_t Expression::evaluate( const std::string& expression, Status &operationStatus )
{
	value_t answer;

	prepare_for_evaluation();

	// get tokens
	m_tokens = m_tokenizer.get_tokens( expression, operationStatus );


	if ( operationStatus.getFlag() == ecode_t::EOK )
	{
		if ( m_tokens.size() <= 1 )
		{ // tokens missing
			operationStatus.setFlag( ecode_t::EPARSING_FAILED_GENERAL );
			return answer;
		}

		try
		{
			// parser tokens -> 
			calculate( m_tokens, m_operands, m_operators, m_functions, operationStatus );

			// TODO 
			// - update answer to nan?
			// check if all elements are consumed except one operand which is real answer
			if ( operationStatus.getFlag() == ecode_t::EOK && m_operands.size() == 1 &&  m_tokens.size() == 1/*unknown left*/ && !m_operators.size() && !m_functions.size() )
			{ // evaluation seems to be ok, so answer is on top of operands stack
				answer = m_operands.top();
			}
			else
			{ // evaluation failure
				auto operationFlag = operationStatus.getFlag();
				if ( operationFlag != ecode_t::EOK )
				{
					/*switch( operationFlag() )
					{
						case
					}*/
					
				}
				else
				if ( m_operands.size() != 1 )
				{
					operationStatus.setFlag( ecode_t::EEVALUATION_FAILED_OPERANDS_NOT_CONSUMED );
					std::cerr << "Evaluation error: " << " operand(s) left on stack" << std::endl;
				}
				else
					if ( m_tokens.size() != 1 ) /*unknown left*/
					{
						operationStatus.setFlag( ecode_t::EEVALUATION_FAILED_TOKENS_NOT_CONSUMED );
						std::cerr << "Evaluation error: " << " token(s) left on stack" << std::endl;
					}
					else
						if ( m_operators.size() )
						{
							operationStatus.setFlag( ecode_t::EEVALUATION_FAILED_OPERATORS_NOT_CONSUMED );
							std::cerr << "Evaluation error: " << " operator(s) left on stack" << std::endl;
						}
						else
							if ( m_functions.size() )
							{
								operationStatus.setFlag( ecode_t::EEVALUATION_FAILED_FUNCTION_NOT_CONSUMED );
								std::cerr << "Evaluation error: " << " function(s) left on stack" << std::endl;
							}
			}	
		}
		catch( const evaluation_exception &e)
		{
			operationStatus.setFlag(ecode_t::EEVALUATION_FAILED_GENERAL);
			std::cerr << "Evaluation error: " << e.what() << ", near position = " << get_last_eval_position() << std::endl;
		}


#ifdef _DEBUG_MSG_
		std::cout << "stack tokens: " << m_tokens.size() << std::endl;
		std::cout << "stack operands: " << m_operands.size() << std::endl;
		std::cout << "stack operators: " << m_operators.size() << std::endl;
		std::cout << "stack functiosn: " << m_functions.size() << std::endl;
#endif
	}
	else
	{ // failure at get_tokens()=> parsing error
		std::cerr << "Parsing error: " <<  operationStatus.toString() << " near position = " << operationStatus.getPosition() << std::endl;
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
size_t Expression::update_operator_precedence()
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

} //namespace expressionEval

