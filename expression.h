#ifndef _EXPRESSION_H_
#define _EXPRESSION_H_

#include <string>
#include <sstream>
#include <cctype>
#include <set>
#include <functional>
#include <cmath>
#include <map>
#include <fstream>
#include <regex>

#include "shunting-yard.h"

#include "evaluation_exception.h"
#include "parse_exception.h"

#include "expression_tokenizer.h"

///
/// lambdas wrapers (vs2012-> no resolution on overloaded functions when wrapping in std::function object )
///
// TODO
// change it!!!
static auto fn_abs		= [](double val)->double { return fabs(val);};
static auto fn_sqrt		= [](double val)->double { return sqrt(val);};
static auto fn_sin		= [](double val)->double { return sin(val);};
static auto fn_cos		= [](double val)->double { return cos(val);};
static auto fn_tan		= [](double val)->double { return tan(val);};
static auto fn_ctg		= [](double val)->double { return 1./tan(val);};
static auto fn_log		= [](double val)->double { return log10(val);};
static auto fn_log10	= [](double val)->double { return log10(val);};
static auto fn_ln		= [](double val)->double { return log(val);};
static auto fn_floor	= [](double val)->double { return floor(val);};
static auto fn_ceil		= [](double val)->double { return ceil(val);};
static auto fn_unknown	= [](double val)->double { return 0.0f;};

///
/// convenient insert macro...
///
#define INSERT_FN(map_fn, fn_name) map_fn[#fn_name] = fn_##fn_name;

///
/// typdefs...
///
typedef std::map<std::string, std::function<double(double)> > map_functions_t;

///
/// wraper for a map of links (function name <-> std::function<> )
///
class availableFunctions
{
private:
	static map_functions_t m_mf;		// map of names & functions
	availableFunctions(const availableFunctions &);

public:

	/*******************************************
	*
	* c-tor
	* 
	********************************************/
	availableFunctions()
	{
		if ( m_mf.size() )
			return;

		INSERT_FN(m_mf, abs);
		INSERT_FN(m_mf, sqrt);
		INSERT_FN(m_mf, sin);
		INSERT_FN(m_mf, cos);
		INSERT_FN(m_mf, tan);
		INSERT_FN(m_mf, ctg);
		INSERT_FN(m_mf, log);
		INSERT_FN(m_mf, log10);
		INSERT_FN(m_mf, ln);
		INSERT_FN(m_mf, floor);
		INSERT_FN(m_mf, ceil );

		// manual -> polish fn names... tg,ctg... etc
		m_mf["tg"] = fn_tan;
		m_mf["ctn"] = fn_ctg;

		// undefined function
		INSERT_FN(m_mf, unknown );
	}

	/*******************************************
	*
	* get function pointer 
	* @returns:
	*	pointer to a funtion or unknown function which does nothing
	*
	********************************************/
	static std::function<double(double)> get( const std::string &fname, status_t &operationStatus )
	{
		operationStatus.setFlag(EPARSING_FAILED_GENERAL);
		auto found = m_mf.find( fname );
		if ( found != m_mf.end() )
		{
			operationStatus.setFlag(EOK);
			return found->second;
		}else
		{
			operationStatus.setFlag(EFAIL);
			return m_mf["unknown"];
		}
	}

};

///
/// expresion evaluator
///
class Expression
{
private:
	ExpressionTokenizer m_tokenizer;	// cuts expresion into tokens
	tokens_t			m_tokens;		// stack of tokens  
	operands_t			m_operands;		// stack of operands
	operators_t			m_operators;	// stack of operators
	functions_t			m_functions;	// stack of functions

	size_t				m_lastParsedTokenPosition;

	static /*const*/ operator_t OPERATORS[];
	static const availableFunctions m_availableFunction;

	///
	///
	///
	operatorType_t	get_arity(char oper, const token_t *previous);
	const operator_t *get_operator(char oper, operatorType_t arity);

	///
	/// stack manipulators
	///
	int push_number(const std::string &value, operands_t &operands);
	int push_operator(const operator_t *oper, operands_t &operands,  operators_t &operators);

	///
	/// stack 
	///
	int apply_operator(const operator_t *oper, operands_t &operands);
	int apply_unary_operator(const operator_t *oper, operands_t &operands);
	int apply_binary_operator(const operator_t *oper, operands_t &operands);
	int apply_function(const std::string &function, operands_t &operands);

	///
	/// 
	///
	size_t updateOperatorPrecedence();

	///
	/// main parse function
	///
	void parse( tokens_t &tokens, operands_t &operands, operators_t &operators, functions_t &functions, status_t &operationStatus);

	///
	/// error findig helper
	///
	size_t get_last_eval_position() const { return m_lastParsedTokenPosition; }

	///
	///
	///
	Expression( const Expression &);
	Expression operator=(const Expression &);

protected:
	// NONE
public:

	///
	/// default c-tor
	///
	Expression(): m_tokenizer("")
	{}

	///
	/// d-tor
	///
	~Expression() {}

	///
	/// evaluates expression 
	///
	value_t evaluate( const std::string &expression, status_t &operationStatus );

};

#endif