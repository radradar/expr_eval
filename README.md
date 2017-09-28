# **Expression Evaluator**

Parses input string and tries to calculate numerical value of input expression.

**Available operators:**

-	'+' sum of two operands,
-	'-' difference of two operands,
-	'*' multiplication of two operands,
-	'/' divide of two operands,
-	'%' modulo of two operands (calculates remainder),
-	'^' power of two operands.

	also:
-	'-' negative value of next operand,
-	'+' positive value of next operand.
	

**Available functions:**

-	"abs" - absolute value of an argument,
-	"sqrt" - squere root of an argument,
-	"sin" - sine of an argument,
-	"cos" - cosine of an argument,
-	"tan" or "tg" - tangent of an argument,
-	"ctn" or "ctg" - cotangent of an argument,
-	"log" or "log10" - logarithm (base-10) of an argument,
-	"ln" - natural logarithm ( base-e) of an argument,
-	"floor" - rounds downward, largest integral value lower or equal to an argument,
-	"ceil" - rounds upward, smallest integral greater or equal to an argument

**Valid expression examples:**

- single digit/number e.g.:
>		1 
>		123
>		1.23456
- any combination of numbers and operators e.g.:
>		1+2
>		1.0-2
>		-1.1+2+4-8*5
- any combination of numbers, operators and functions e.g.:
>		1+2*sin(0.0)
>		1.0-2+floor(3.5)
>		-1.1+2^5/tan(0.1+0.2)
- grouping in parentheses e.g.:
>		4*(2+3)
>		(1.0-2)*pow(2.0,3.1)
>		(1+2)*(3+4)
	

**Invalid expression examples:** 

- single digit/number without fraction part e.g:
>		1.
>		123.
- missing closing/opening parenthese e.g.:
>		sin(2.0+1.0
>		2.8*floor1.4)
- using unavailable funtion or misspelling function name e.g.:
>		2.0*gamma(1.0)
>		3.0*sim(0.0)
- using ',' (comma) as a separator in floating points numbers e.g.:
>		2,5
	
**Extra rules:**

When expression containes only numbers that are representation of integer values all calculations are made with integers, not floating point. For example dividing 5 by 2 ("5/2") gives "2" as a result. If at least one of the operators is floating point number then calculations will preserve fraction part of calculation i.e. "5/2.0" gives "2.5" as a result (the same for expression as "5.0/2").





