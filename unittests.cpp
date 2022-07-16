#include "unittests.h"
#include "Parser.h"

#include <string>
#include <iostream>
#include <map>
#include <math.h>
#include <limits>

using namespace unittests;

bool AreSame(double a, double b)
{
    return std::fabs(a - b) < std::numeric_limits<double>::epsilon();
}

bool evaluate_and_compare(const std::vector<std::string> &statements, std::map<std::string, double> &expectedValues)
{
	Parser p;	
	for (const std::string &statement : statements)
		p.AddStatement(statement);

	p.EvaluateStatements();

	bool bRes = true;
	for (const std::map<std::string, double>::value_type &vt : expectedValues)
	{
		double storedVariableValue = p.LookupVariable(vt.first);
		if (!AreSame(storedVariableValue, vt.second))
		{
			bRes = false;
			break;
		}
	}

	return bRes;
}

bool test_case0()
{
	std::vector<std::string> statements {
		std::string("i=0"), std::string("j=++i"), std::string("x=i++ + 5"), 
		std::string("y= 5 + 3 * 10"), std::string("i += y")
	};
	std::map<std::string, double> expectedValues {
		{std::string("i"), 37}, {std::string("j"), 1}, {std::string("x"), 6},
		{std::string("y"), 35},
	};

	return evaluate_and_compare(statements, expectedValues);;
}

bool test_case1()
{
	std::vector<std::string> statements {
		std::string("a=2+3"), std::string("b=2*3"), std::string("c=2-3"), 
		std::string("d=2^3"), std::string("e=5%3"), std::string("f=4/2")
	};
	std::map<std::string, double> expectedValues {
		{std::string("a"), 5}, {std::string("b"), 6}, {std::string("c"), -1},
		{std::string("d"), 8}, {std::string("e"), 2}, {std::string("f"), 2}
	};

	return evaluate_and_compare(statements, expectedValues);
}

bool test_case2()
{
	std::vector<std::string> statements {
		std::string("a=(5 + 2*3 - 1 + 7 * 8)"), std::string("b=(67 + 2 * 3 - 67 + 2/1 - 7)"), std::string("c=a+b"), 
		std::string("d=b+(2) + (17*2-30) * (5)+2 - (8/2)*4"), std::string("e=4*2.5 + 8.5+1.5 / 3.0"), std::string("f=a+b+c+d+e")
	};

	std::map<std::string, double> expectedValues {
		{std::string("a"), 66}, {std::string("b"), 1}, {std::string("c"), 67},
		{std::string("d"), 9}, {std::string("e"), 19}, {std::string("f"), 162}
	};

	return evaluate_and_compare(statements, expectedValues);
}

bool test_case3()
{
	std::vector<std::string> statements {
		std::string("a=sin(60)"), std::string("b=cos(60)"), std::string("c=tan(60)"), 
		std::string("d=ceil(5.5)"), std::string("e=floor(5.5)")
	};

	std::map<std::string, double> expectedValues {
		{std::string("a"), sin(60)}, {std::string("b"), cos(60)}, {std::string("c"), tan(60)},
		{std::string("d"), ceil(5.5)}, {std::string("e"), floor(5.5)}
	};

	return evaluate_and_compare(statements, expectedValues);
}

bool test_case4()
{
	std::vector<std::string> statements {
		std::string("a=5"), std::string("a+=15"), std::string("b=a"), 
		std::string("b-=15"), std::string("a*=b")
	};

	std::map<std::string, double> expectedValues {
		{std::string("a"), 100}, {std::string("b"), 5}
	};

	return evaluate_and_compare(statements, expectedValues);
}

bool test_case5()
{
	std::vector<std::string> statements {
		std::string("abcde=1"), std::string("qwer123aq=2"), std::string("QWEAZcde=3"), 
		std::string("ertTYU=4"), std::string("abcde=qwer123aq")
	};

	std::map<std::string, double> expectedValues {
		{std::string("abcde"), 2}, {std::string("qwer123aq"), 2}, {std::string("QWEAZcde"), 3},
		{std::string("ertTYU"), 4}
	};

	return evaluate_and_compare(statements, expectedValues);
}

bool test_case6()
{
	std::vector<std::string> statements {
		std::string("abc=   12        -  8   "), std::string("a=142        -9   "), std::string("ab=72+  15"), 
		std::string("aaa= 12*  4"), std::string("xxx=abc"), std::string("xxxx=xxx")
	};

	std::map<std::string, double> expectedValues {
		{std::string("abc"), 4}, {std::string("a"), 133}, {std::string("ab"), 87},
		{std::string("aaa"), 48}, {std::string("xxxx"), 4}, {std::string("xxx"), 4}
	};

	return evaluate_and_compare(statements, expectedValues);
}


void UnitTests::RunUnitTests()
{
	std::cout << "Running unit tests:" << std::endl;
	size_t passed(0), failed(0);
	test_case0() ? ++passed : ++failed;
	test_case1() ? ++passed : ++failed;
	test_case2() ? ++passed : ++failed;
	test_case3() ? ++passed : ++failed;
	test_case4() ? ++passed : ++failed;
	test_case5() ? ++passed : ++failed;
	test_case6() ? ++passed : ++failed;

	std::cout << "Ran  " << passed+failed <<" tests" << std::endl;
	std::cout << passed << " tests passed" << std::endl;
	std::cout << failed << " tests failed" << std::endl;
}
