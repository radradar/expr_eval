#ifndef _STATUS_CLASS_H_
#define _STATUS_CLASS_H_


namespace expressionEval
{

enum class ecode_t
{
	EOK,
	EFAIL,

	EPARSING_FAILED_GENERAL,
	EPARSING_FAILED_NO_OPERATORS,
	EPARSING_FAILED_NO_OPERANDS,
	EPARSING_FAILED_NO_FUNCTIONS,
	EPARSING_FAILED_NO_TOKENS,
	EPARSING_FAILED_UNKNOWN_OPERATOR,
	EPARSING_FAILED_UNKNOWN_FUNCTION,
	EPARSING_FAILED_NO_CLOSING_PARENTHESIS,
	EPARSING_FAILED_FUNCTION_ARGUMENT_MISSING,

	EEVALUATION_FAILED_GENERAL,
	EEVALUATION_FAILED_OVERFLOW,
	EEVALUATION_FAILED_RANGE_ERROR,
	EEVALUATION_FAILED_TOKENS_NOT_CONSUMED,
	EEVALUATION_FAILED_OPERATORS_NOT_CONSUMED,
	EEVALUATION_FAILED_OPERANDS_NOT_CONSUMED,
	EEVALUATION_FAILED_FUNCTION_NOT_CONSUMED
	
};


class Status
{
private:
	ecode_t	flag_;
	size_t	position_;
	double	value_;

public:
	Status( ecode_t flag=ecode_t::EOK, size_t position=0, double value=0.0f): 
	flag_(flag),
	position_(position_),
	value_(value)
	{}

	//
	//setters
	//
	void setFlag( ecode_t flag ) { flag_ = flag; } 
	void setPosition( size_t position) { position_ = position;}
	void setValue( double value ) { value_ = value; }

	//
	// getters
	//
	ecode_t	getFlag(void) const { return flag_; } 
	size_t getPosition() const { return position_; }
	double	getValue(void)  const { return value_; }

	std::string toString() const
	{
		std::string ret("");

		switch (flag_)
		{
		case expressionEval::ecode_t::EOK:
			break;
		case expressionEval::ecode_t::EFAIL:
			break;
		case expressionEval::ecode_t::EPARSING_FAILED_GENERAL:
			ret = "parsing failed general";
			break;
		case expressionEval::ecode_t::EPARSING_FAILED_NO_OPERATORS:
			ret = "no operators left";
			break;
		case expressionEval::ecode_t::EPARSING_FAILED_NO_OPERANDS:
			ret = "no operands left";
			break;
		case expressionEval::ecode_t::EPARSING_FAILED_NO_FUNCTIONS:
			ret = "no functions left";
			break;
		case expressionEval::ecode_t::EPARSING_FAILED_NO_TOKENS:
			ret = "no tokens left";
			break;
		case expressionEval::ecode_t::EPARSING_FAILED_UNKNOWN_OPERATOR:
			ret = "unknow operator";
			break;
		case expressionEval::ecode_t::EPARSING_FAILED_UNKNOWN_FUNCTION:
			ret = "unknow function";
			break;
		case expressionEval::ecode_t::EPARSING_FAILED_NO_CLOSING_PARENTHESIS:
			ret = "no closing parenthesis";
			break;
		case expressionEval::ecode_t::EPARSING_FAILED_FUNCTION_ARGUMENT_MISSING:
			ret = "function arguments missing";
			break;
		case expressionEval::ecode_t::EEVALUATION_FAILED_GENERAL:
			ret = "evaluation failed general";
			break;
		case expressionEval::ecode_t::EEVALUATION_FAILED_OVERFLOW:
			ret = "evaluation failed overflow";
			break;
		case expressionEval::ecode_t::EEVALUATION_FAILED_RANGE_ERROR:
			ret = "evaluation range error";
			break;
		case expressionEval::ecode_t::EEVALUATION_FAILED_TOKENS_NOT_CONSUMED:
			ret = "tokens left (not consumed)";
			break;
		case expressionEval::ecode_t::EEVALUATION_FAILED_OPERATORS_NOT_CONSUMED:
			ret = "operators left (not consumed)";
			break;
		case expressionEval::ecode_t::EEVALUATION_FAILED_OPERANDS_NOT_CONSUMED:
			ret = "operands left (not consumed)";
			break;
		case expressionEval::ecode_t::EEVALUATION_FAILED_FUNCTION_NOT_CONSUMED:
			ret = "functions left (not consumed)";
			break;
		default:
			break;
		}
		return ret;
	}
};

} //namespace expressionEval


#endif