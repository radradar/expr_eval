#ifndef _EXPRESSION_TOKENIZER_H_
#define _EXPRESSION_TOKENIZER_H_

#include <string>
#include <cctype>
#include <set>
#include <iostream>
#include <algorithm>

#include "shunting-yard.h"

#include "evaluation_exception.h"
#include "parse_exception.h"

//#define _DEBUG_MSG_

namespace expressionEval
{


class ExpressionTokenizer
{
private:
	std::string		m_expression;

	//TODO:
	// hide constructors, operators from user
	//
	//
	// 
	ExpressionTokenizer();
	ExpressionTokenizer(const ExpressionTokenizer &);
	ExpressionTokenizer operator=(const ExpressionTokenizer &);

	std::string get_number( istring_t it, istring_t end, Status &operationStatus);
	std::string get_function_name( istring_t it, istring_t end, Status &operationStatus);

	///
	/// prepares & basic check expression for validity
	///
	std::string prepare_expression( const std::string& expression, Status &operationStatus );


protected:
	// NONE

public:

	static const token_t SENTINEL;
	static bracket_t BRACKETS[];

	///
	/// C-tor
	///
	ExpressionTokenizer( const std::string &expression );

	//
	// make tokens fron an expression
	//
	tokens_t get_tokens( const std::string &expression, Status &operationStatus );

	//
	// getters
	//
	std::string get_expression() const { return m_expression; }
};

} //namespace expressionEval


#endif