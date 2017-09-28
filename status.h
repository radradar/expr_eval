#ifndef _STATUS_CLASS_H_
#define _STATUS_CLASS_H_


namespace expressionEval
{

// class enum?
typedef enum
{
	EOK,
	EFAIL,

	EPARSING_FAILED_GENERAL,
	EPARSING_FAILED_UNKNOWN_OPERATOR,
	EPARSING_FAILED_UNKNOWN_FUNCTION,
	EPARSING_FAILED_NO_CLOSING_PARENTHESIS,

	EEVALUATION_FAILED_GENERAL,
	EEVALUATION_FAILED_OVERFLOW,
	EEVALUATION_FAILED_RANGE_ERROR,
	EEVALUATION_FAILED_TOKENS_NOT_CONSUMED,
	EEVALUATION_FAILED_OPERATORS_NOT_CONSUMED,
	EEVALUATION_FAILED_OPERANDS_NOT_CONSUMED,
	EEVALUATION_FAILED_FUNCTION_NOT_CONSUMED
	
}code_t;


class Status
{
private:
	code_t	flag_;
	//size_t	position_;
	double	value_;

public:
	Status( code_t flag=EOK, /*size_t position=0,*/ double value=0.0f): 
	flag_(flag),
	//position_(position),
	value_(value)
	{}

	//setters
	void setFlag( code_t flag ) { flag_ = flag; } 
	//void setPosition( size_t position ) { position_ = position; }
	void setValue( double value ) { value_ = value; }

	// getters
	code_t	getFlag(void) const { return flag_; } 
	//size_t	getPosition(void) const { return position_; }
	double	getValue(void)  const { return value_; }

};

typedef Status status_t;

} //namespace expressionEval


#endif