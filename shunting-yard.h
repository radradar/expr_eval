#ifndef _SHUNTING_YARD_COMMON_DEFS_H_
#define _SHUNTING_YARD_COMMON_DEFS_H_

#include <string>
#include <stack>
#include <deque>

#include "value.h"


/*
https://en.wikipedia.org/wiki/Shunting-yard_algorithm

while there are tokens to be read:
	read a token.
	if the token is a number, then push it to the output queue.
	if the token is an operator, then:
		while there is an operator at the top of the operator stack with
			greater than or equal to precedence and the operator is left associative:
				pop operators from the operator stack, onto the output queue.
		push the read operator onto the operator stack.
	if the token is a left bracket (i.e. "("), then:
		push it onto the operator stack.
	if the token is a right bracket (i.e. ")"), then:
		while the operator at the top of the operator stack is not a left bracket:
			pop operators from the operator stack onto the output queue.
		pop the left bracket from the stack.
		/* if the stack runs out without finding a left bracket, then there are
		mismatched parentheses. /

if there are no more tokens to read:
	while there are still operator tokens on the stack:
		/* if the operator token on the top of the stack is a bracket, then
		there are mismatched parentheses. /
		pop the operator onto the output queue.
exit.
*/

// TODO:
// - move typedef enum to enum class ....

//enum class tokenType_t;
//
// types of tokens
//
typedef enum 
{
	TOKEN_UNKNOWN,
	TOKEN_NUMBER,
	TOKEN_LEFT_BRACKET,
	TOKEN_RIGHT_BRACKET,
	TOKEN_OPERATOR,
	TOKEN_FUNCTION
} tokenType_t;

//
// definition of a token
//
typedef struct 
{
	tokenType_t type;
	std::string value;
	size_t		position;
} token_t;

//
// operator arity
//
typedef enum 
{
	OPERATOR_UNARY,
	OPERATOR_BINARY,
	OPERATOR_OTHER
} operatorType_t;

//
// operator assoc.
//
typedef enum 
{
	OPERATOR_NONE,
	OPERATOR_LEFT,
	OPERATOR_RIGHT
} operatorAssociativity_t;


//
// definition od an operator
//
typedef struct 
{
	char					oper;			// opertator symbol/symbols?: +,-,>=, etc  
	size_t					precedence;		// range of the operator
	operatorType_t			arity;			// operator type
	operatorAssociativity_t	associativity;	// operator assoc
} operator_t;


//
//
//
typedef struct
{
	tokenType_t	token;
	char		symbol;
} bracket_t;


/*********************************************************************************************************
*
* typedefs for common containers
*
*********************************************************************************************************/
typedef std::deque<token_t>		tokens_t;
typedef std::stack<value_t>		operands_t;
typedef std::stack<operator_t>	operators_t;
typedef std::stack<std::string> functions_t;

typedef std::string::iterator		istring_t;
typedef std::string::const_iterator cistring_t;

#endif