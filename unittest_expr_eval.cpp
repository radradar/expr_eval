#include "stdafx.h"

#include <string>
#include <cmath>

#include "../expr_eval/expression.h"
#include "gtest/gtest.h"
 

#define TEST_CASE_EQ(arg)  			EXPECT_EQ( arg, expression.evaluate(#arg, operationStatus) );
#define TEST_CASE_DOUBLE_EQ(arg)  	EXPECT_DOUBLE_EQ( arg, expression.evaluate(#arg, operationStatus) );

#define TEST_CASE_NE(arg)  			EXPECT_NE( arg, expression.evaluate(#arg, operationStatus) );


#pragma region TestOperators

TEST (ExpressionEvaluateTestOperators, Sum )
{
	// INTEGERS
	status_t operationStatus;
	Expression expression;
	//TEST_CASE_EQ( 3+2);
	//TEST_CASE_EQ( -3+2);
	//TEST_CASE_EQ( -2+3);
	//TEST_CASE_EQ( 1+2+3+4+5+6+7+8+10);
	//TEST_CASE_EQ( 1+2+3+4+5+6+7+8+10);
	//TEST_CASE_EQ( -1+1 );


	//// FLOATING POINT
	//TEST_CASE_DOUBLE_EQ( 2.1+3.5 );
	//TEST_CASE_DOUBLE_EQ( 3.5+2.1);
	//TEST_CASE_DOUBLE_EQ( -3.5+2.1);
	//TEST_CASE_DOUBLE_EQ( -2.1+3.5 );
	TEST_CASE_DOUBLE_EQ( 1+2.1+3.5+4+5+6+7+8+10 );
	TEST_CASE_DOUBLE_EQ( 10-9-8-7-6-5-4-3.5-2.1-1 );
	TEST_CASE_DOUBLE_EQ( -1.0+1.0 );

}

TEST (ExpressionEvaluateTestOperators, Sub )
{
	status_t operationStatus;
	Expression expression;
	TEST_CASE_EQ( 2-3 );
	TEST_CASE_EQ( 3-2 );
	TEST_CASE_EQ( -3-2 );
	TEST_CASE_EQ( -2-3 );
	TEST_CASE_EQ( 1-2-3-4-5-6-7-8-10 );

}


TEST (ExpressionEvaluateTestOperators, Mul )
{
	status_t operationStatus;
	Expression expression;
	TEST_CASE_EQ( 2*3 );
	TEST_CASE_EQ( 3*2 );
	TEST_CASE_EQ( -3*2 );
	TEST_CASE_EQ( -2*3 );
	TEST_CASE_EQ( 1*2*3*4*5*6*7*8*9*10 );
}

TEST (ExpressionEvaluateTestOperators, Div )
{
	status_t operationStatus;
	Expression expression;
	TEST_CASE_EQ( 2/3 );
	TEST_CASE_EQ( 3/2 );
	TEST_CASE_EQ( -3/2 );
	TEST_CASE_EQ( -2/3 );
	TEST_CASE_EQ( 1/2/3 );
}

TEST (ExpressionEvaluateTestOperators, Modulo )
{
	status_t operationStatus;
	Expression expression;
	TEST_CASE_EQ( 2%3 );
	TEST_CASE_EQ( 3%2 );
	TEST_CASE_EQ( 100%67 );
	TEST_CASE_EQ( 100000000%5 );
	TEST_CASE_EQ( 100000001%5 );

	//TEST_CASE_NE( 1%0 );
	//TEST_CASE_NE( -5%2 );

	//ASSERT_THROW(operationStatus, exc_type)
	//ASSERT_ANY_THROW(operationStatus)
	//ASSERT_NO_THROW(operationStatus)
}


TEST (ExpressionEvaluateTestOperators, Power )
{
	status_t operationStatus;
	Expression expression;

	EXPECT_EQ( (__int64)pow(2,3), expression.evaluate("2^3", operationStatus) );
	EXPECT_EQ( (__int64)pow(3,2), expression.evaluate("3^2", operationStatus) );
	EXPECT_EQ( (__int64)pow(3,-2), expression.evaluate("3^(-2)", operationStatus) );
	EXPECT_EQ( (__int64)pow(-3,2), expression.evaluate("(-3)^2", operationStatus) );

	EXPECT_EQ( pow(2,3), expression.evaluate("2^3.0", operationStatus) );
	EXPECT_EQ( pow(3,2), expression.evaluate("3^2.0", operationStatus) );
	EXPECT_EQ( pow(3,-2), expression.evaluate("3^(-2.0)", operationStatus) );
	EXPECT_EQ( pow(-3,2), expression.evaluate("(-3)^(2.0)", operationStatus) );

}

TEST (ExpressionEvaluateTestOperators, OperatorPrecedence )
{
	status_t operationStatus;
	Expression expression;

	TEST_CASE_DOUBLE_EQ(2+3*4);
	TEST_CASE_DOUBLE_EQ((2+3)*4);
	TEST_CASE_DOUBLE_EQ(2+(3*4));
	TEST_CASE_DOUBLE_EQ((2+3)*4);
	
	TEST_CASE_DOUBLE_EQ(2*5+3*4);
	TEST_CASE_DOUBLE_EQ(2*(5+3)*4);

	TEST_CASE_DOUBLE_EQ(2*5+3*4);
	TEST_CASE_DOUBLE_EQ(2*((5+3)*4));
	TEST_CASE_DOUBLE_EQ(2*5+3*4);
	TEST_CASE_DOUBLE_EQ(2+(5+3)*4);
	TEST_CASE_DOUBLE_EQ(2*(5+3)-4);

}

#pragma endregion

#pragma region TestFunctions

TEST (ExpressionEvaluateTestFunctions, Functions )
{
	status_t operationStatus;
	Expression expression;

	TEST_CASE_DOUBLE_EQ( sin(0.0) );
	TEST_CASE_DOUBLE_EQ( cos(0.0) );
	TEST_CASE_DOUBLE_EQ( tan(0.0) );
	EXPECT_DOUBLE_EQ( 1./tan(0.0), expression.evaluate("ctg(0.0)", operationStatus) );
	TEST_CASE_DOUBLE_EQ( log(1.0) );
	TEST_CASE_DOUBLE_EQ( log10(1.0) );
	TEST_CASE_DOUBLE_EQ( log(1.0) );

	TEST_CASE_DOUBLE_EQ( floor(3.5) );
	TEST_CASE_DOUBLE_EQ( floor(3.999999) );
	TEST_CASE_DOUBLE_EQ( floor(3.000000) );
	TEST_CASE_DOUBLE_EQ( floor(3) );

	TEST_CASE_DOUBLE_EQ( ceil(3.5) );
	TEST_CASE_DOUBLE_EQ( ceil(3.999999) );
	TEST_CASE_DOUBLE_EQ( ceil(3.000000) );
	TEST_CASE_DOUBLE_EQ( ceil(3) );
}


TEST (ExpressionEvaluateTestFunctions, FunctionsOperators )
{
	status_t operationStatus;
	Expression expression;

	TEST_CASE_DOUBLE_EQ( sin(1.0)*sin(0.0));
	TEST_CASE_DOUBLE_EQ( cos(1.0)*cos(0.0));
	TEST_CASE_DOUBLE_EQ( sin(1.0)*tan(0.0));
	TEST_CASE_DOUBLE_EQ( floor(3.5)+ceil(0.10));
	TEST_CASE_DOUBLE_EQ( sin(1.0)*sin(0.0));
	TEST_CASE_DOUBLE_EQ( sin(1.0)*sin(0.0));
	TEST_CASE_DOUBLE_EQ( sin(1.0)*sin(0.0));

}

#pragma endregion

TEST (ExpressionEvaluateTestInvalidExpression, InvalidExpressions )
{
	status_t operationStatus;
	Expression expression;

	//ASSERT_NO_THROW( expression.evaluate( "" ,operationStatus) );
	//ASSERT_NO_THROW( expression.evaluate( "x" ,operationStatus) );
	//ASSERT_NO_THROW( expression.evaluate( "xXx" ,operationStatus) );
	//ASSERT_NO_THROW( expression.evaluate( "(" ,operationStatus) );
	//ASSERT_NO_THROW( expression.evaluate( ")" ,operationStatus) );
	//ASSERT_NO_THROW( expression.evaluate( "-" ,operationStatus) );
	//ASSERT_NO_THROW( expression.evaluate( "+" ,operationStatus) );
	//ASSERT_NO_THROW( expression.evaluate( "*" ,operationStatus) );
	//ASSERT_NO_THROW( expression.evaluate( "/" ,operationStatus) );
	//ASSERT_NO_THROW( expression.evaluate( "-" ,operationStatus) );
	//ASSERT_NO_THROW( expression.evaluate( "+" ,operationStatus) );


	//EXPECT_NEQ( operationStatus.flag
}


int main(int argc, char **argv) 
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}