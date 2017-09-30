// expr_eval.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iomanip>

#include "expression.h"


//std::string expr = "";
//std::string expr= "110.+112-111)";
//std::string expr= "110.+112-111;";
//std::string expr= "(110+112-111"; // throws exc
//std::string expr = "11+11+";
//std::string expr = "+";
//std::string expr = "+++";
//std::string expr = "2.1+3.1";
//std::string expr = "3.0/2";
//std::string expr = "cos(0)";
//std::string expr = "1+2.1+3.5+4";
std::string expr = "floor(3)";
//std::string expr = "1/2/3/4/5/6/7/8/9/10.0";
//std::string expr = "2 + 3*cos(0)";
//std::string expr = "2 + 1*log10(10)";

//std::string expr = "1/0";
//std::string expr = "2.3+3*sin(0.5)";
//std::string expr = "2*sin(0.3+4.5)*(2+5)^(3+(2*4-1)))*(1+2)^(3-4)/123.567890";
//std::string expr = "-2*sin(-0.3+4.5)*pow(2+5,(3+5))*pow(1+2,3-4)/123.567890";
//std::string expr = "-2*floor(3.5006)+ceil(0.05)";


/**************************************************************************************
*
* reads user input 
* @returns:
*	read input string
*
***************************************************************************************/
std::string get_user_input()
{
	std::string expression;
	std::cin >> expression;
	//cout << "-> " << expression;
	return expression;
}



/**************************************************************************************
*
* main
* @returns
* - on success EXIT_SUCCESS 
* - on failure EXIT_FAILURE
*
***************************************************************************************/
int _tmain(/*int argc, _TCHAR* argv[]*/)
{
	// TODO:
	// - non-alphanumerics?
	// - on win32 register SetConsoleCtrlHandler & add break to main loop
	// - on unix add signal handler & add break to main loop

	auto retValue = EXIT_SUCCESS;

	// main loop
	while( true )
	{
		auto inputString = get_user_input();

		expressionEval::Expression expression;
		expressionEval::Status operationStatus;
		auto answer = expression.evaluate( inputString, operationStatus );

		// check evaluation status
		if ( operationStatus.getFlag() == expressionEval::ecode_t::EOK )
		{
			if (answer.type == expressionEval::valueType_t::INTEGER )
			{
				std::cout << answer.iValue << std::endl;
				//std::cout << "Answer *** " << answer.iValue << " *** " << std::endl;
			}
			else if(answer.type == expressionEval::valueType_t::FLOATINGPOINT)
			{
				std::cout << std::fixed;
				std::cout << answer.fValue << std::endl;
				//std::cout << "Answer *** " << answer.fValue << " *** " << std::endl;
			}
		}
		else
		{
			// send empty line to std output
			std::cout << std::endl;
			// send error message to err output 
			//cerr << "BRAK PRAWIDLOWEJ ODPOWIEDZI " << endl;
			retValue = EXIT_FAILURE;
		}
	} // main loop

	
	return retValue;
}

