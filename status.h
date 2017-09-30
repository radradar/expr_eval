#ifndef _STATUS_CLASS_H_
#define _STATUS_CLASS_H_


namespace expressionEval
{

enum class ecode_t
{
	EOK,
	EFAIL,

	EPARSING_FAILED_GENERAL,
	EPARSING_FAILED_UNKNOWN_OPERATOR,
	EPARSING_FAILED_UNKNOWN_FUNCTION,
	EPARSING_FAILED_NO_CLOSING_PARENTHESIS,
	EPARSING_FAILED_FUNCTION_ARGUMENT_MISSING,
	EPARSING_FAILED_NUMBER_READ,

	EEVALUATION_FAILED_GENERAL,
	EEVALUATION_FAILED_OVERFLOW,
	EEVALUATION_FAILED_RANGE_ERROR,
	EEVALUATION_FAILED_TOKENS_NOT_CONSUMED,
	EEVALUATION_FAILED_OPERATORS_NOT_CONSUMED,
	EEVALUATION_FAILED_OPERANDS_NOT_CONSUMED,
	EEVALUATION_FAILED_FUNCTION_NOT_CONSUMED,
	EEVALUATION_FAILED_NO_OPERATORS,
	EEVALUATION_FAILED_NO_OPERANDS,
	EEVALUATION_FAILED_NO_FUNCTIONS,
	EEVALUATION_FAILED_NO_TOKENS,

	EEVALUATION_FAILED_UNKNOW_OPERATOR,
	EEVALUATION_FAILED_UNKNOW_OPERAND,
	EEVALUATION_FAILED_UNKNOW_FUNCTION,
	EEVALUATION_FAILED_UNKNOW_TOKEN,
	EEVALUATION_FAILED_ARGUMENT_ERROR,

	EEVALUATION_CONVERSION_FAILURE
	
};


class Status
{
private:
	ecode_t	flag_;
	size_t	position_;
	//double	value_;

public:
	Status( ecode_t flag=ecode_t::EOK, size_t position=0): 
	flag_(flag)
	,position_(position_)
	//,value_(value)
	{}

	//
	//setters
	//
	void set(ecode_t flag, size_t position ) { flag_ = flag; position_ = position;} 
	void setFlag( ecode_t flag ) { flag_ = flag; } 
	void setPosition( size_t position) { position_ = position;}
	//void setValue( double value ) { value_ = value; }

	//
	// getters
	//
	ecode_t	getFlag(void) const { return flag_; } 
	size_t getPosition() const { return position_; }
	//double	getValue(void)  const { return value_; }

	std::string toString() const
	{
		std::string ret("");

#define CASE(idx_, var_, val_) case idx_: {var_ = val_;} break;

		switch (flag_)
		{
		CASE( expressionEval::ecode_t::EOK, ret, "");
		CASE( expressionEval::ecode_t::EFAIL, ret, "parsing/evaluating failed");
		CASE( expressionEval::ecode_t::EPARSING_FAILED_GENERAL,	ret, "parsing failed general");
		CASE( expressionEval::ecode_t::EPARSING_FAILED_UNKNOWN_OPERATOR, ret, "unknow operator");
		CASE( expressionEval::ecode_t::EPARSING_FAILED_UNKNOWN_FUNCTION, ret, "unknow function");
		CASE( expressionEval::ecode_t::EPARSING_FAILED_NO_CLOSING_PARENTHESIS, ret, "no closing parenthesis");
		CASE( expressionEval::ecode_t::EPARSING_FAILED_FUNCTION_ARGUMENT_MISSING, ret, "function arguments missing");
		CASE( expressionEval::ecode_t::EPARSING_FAILED_NUMBER_READ, ret, "read number error");
		CASE( expressionEval::ecode_t::EEVALUATION_FAILED_GENERAL, ret, "evaluation failed general");
		CASE( expressionEval::ecode_t::EEVALUATION_FAILED_OVERFLOW, ret, "evaluation failed overflow");
		CASE( expressionEval::ecode_t::EEVALUATION_FAILED_RANGE_ERROR, ret, "evaluation range error");
		CASE( expressionEval::ecode_t::EEVALUATION_FAILED_TOKENS_NOT_CONSUMED, ret, "tokens left (not consumed)");
		CASE( expressionEval::ecode_t::EEVALUATION_FAILED_OPERATORS_NOT_CONSUMED, ret, "operators left (not consumed)");
		CASE( expressionEval::ecode_t::EEVALUATION_FAILED_OPERANDS_NOT_CONSUMED, ret, "operands left (not consumed)");
		CASE( expressionEval::ecode_t::EEVALUATION_FAILED_FUNCTION_NOT_CONSUMED, ret, "functions left (not consumed)");
		CASE( expressionEval::ecode_t::EEVALUATION_FAILED_NO_OPERATORS, ret, "no operators left");
		CASE( expressionEval::ecode_t::EEVALUATION_FAILED_NO_OPERANDS, ret, "no operands left");
		CASE( expressionEval::ecode_t::EEVALUATION_FAILED_NO_FUNCTIONS, ret, "no functions left");
		CASE( expressionEval::ecode_t::EEVALUATION_FAILED_NO_TOKENS, ret, "no tokens left");
		CASE( expressionEval::ecode_t::EEVALUATION_FAILED_UNKNOW_OPERATOR, ret, "unknown operator");
		CASE( expressionEval::ecode_t::EEVALUATION_FAILED_UNKNOW_OPERAND, ret, "unknow operand");
		CASE( expressionEval::ecode_t::EEVALUATION_FAILED_UNKNOW_FUNCTION, ret, "unknow function");
		CASE( expressionEval::ecode_t::EEVALUATION_FAILED_UNKNOW_TOKEN,ret, "unknow token");
		CASE( expressionEval::ecode_t::EEVALUATION_FAILED_ARGUMENT_ERROR, ret, "function argument error");
		CASE( expressionEval::ecode_t::EEVALUATION_CONVERSION_FAILURE, ret, "string->number conversion failure");

		default:
			break;
		}
		return ret;
	}

	
}; // class Status

} //namespace expressionEval


#endif