#include "unittests.h"
#include "Parser.h"

#include <string>
#include <iostream>
#include <map>
#include <math.h>
#include <limits>

using namespace unittests;

struct cout_redirect 
{
    cout_redirect(std::streambuf *new_buffer) 
        : old( std::cout.rdbuf(new_buffer) )
    { }

    ~cout_redirect( ) {
        std::cout.rdbuf(old);
    }

private:
    std::streambuf * old;
};

bool AreSame(double a, double b)
{
    return std::fabs(a - b) < std::numeric_limits<double>::epsilon();
}

bool test_case1()
{
	std::map<std::string, double> expectedValues;
	expectedValues["i"] = 37;
	expectedValues["j"] = 1;
	expectedValues["x"] = 6;
	expectedValues["y"] = 35;
    Parser p;
	std::string s;
	s = std::string("i=0");
	p.AddStatement(s);
	s = std::string("j=++i");
	p.AddStatement(s);
	s = std::string("x=i++ + 5");
	p.AddStatement(s);
	s = std::string("y= 5 + 3 * 10");
	p.AddStatement(s);
	s = std::string("i += y");
	p.AddStatement(s);
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


void UnitTests::RunUnitTests()
{
	test_case1();
}