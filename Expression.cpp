#include "Expression.h"
#include "Parser.h"
#include <cmath>
#include <iostream>

Expression::Expression()
{
}

NumberExpression::NumberExpression(double val) 
	: value(val) 
{
}

double NumberExpression::Evaluate()
{
	return value;
}

VariableExpression::VariableExpression(Parser *parser, const std::string& var) 
	: variable(var)
{
	SetParser(parser);
}

double VariableExpression::Evaluate() 
{
	double res(0);
	if (m_parser)
		res = m_parser->LookupVariable(variable);
	return res;
}

ArithmeticExpression::ArithmeticExpression(const ExpressionPtr &l, const ExpressionPtr &r)
	: m_left(l), m_right(r)
{
}

bool 
ArithmeticExpression::Validate() const
{
	return m_left != nullptr && m_right != nullptr;
}

AdditionExpression::AdditionExpression(const ExpressionPtr &left, const ExpressionPtr &right)
	: ArithmeticExpression(left, right)
{
}

double 
AdditionExpression::Evaluate()
{
	if (!Validate())
	{
		std::cout << "AdditionExpression: Could not evaluate expression" << std::endl;
		return 0;
	}
		

	double a = m_left->Evaluate();
	double b = m_right->Evaluate();
	return a+b;
}

SubstractionExpression::SubstractionExpression(const ExpressionPtr &left, const ExpressionPtr &right)
	: ArithmeticExpression(left, right)
{
}

double 
SubstractionExpression::Evaluate()
{
	if (!Validate())
	{
		std::cout << "SubstractionExpression: Could not evaluate expression" << std::endl;
		return 0;
	}

	double a = m_left->Evaluate();
	double b = m_right->Evaluate();
	return a-b;
}

MultiplicationExpression::MultiplicationExpression(const ExpressionPtr &left, const ExpressionPtr &right)
	: ArithmeticExpression(left, right)
{
}

double 
MultiplicationExpression::Evaluate()
{
	if (!Validate())
	{
		std::cout << "MultiplicationExpression: Could not evaluate expression" << std::endl;
		return 0;
	}

	double a = m_left->Evaluate();
	double b = m_right->Evaluate();
	return a*b;
}

DivisionExpression::DivisionExpression(const ExpressionPtr &left, const ExpressionPtr &right)
	: ArithmeticExpression(left, right)
{
}

double 
DivisionExpression::Evaluate()
{
	if (!Validate())
	{
		std::cout << "DivisionExpression: Could not evaluate expression" << std::endl;
		return 0;
	}

	double a = m_left->Evaluate();
	double b = m_right->Evaluate();
	double res = 0;
	if (b == 0.0)
	{
		std::cout << "DivisionExpression: Attempt to divide by zero: " << std::endl;
	}
	else 
		res = a/b;

	return res;
}

ModulusExpression::ModulusExpression(const ExpressionPtr &left, const ExpressionPtr &right)
	: ArithmeticExpression(left, right)
{
}

double 
ModulusExpression::Evaluate()
{
	if (!Validate())
	{
		std::cout << "ModulusExpression: Could not evaluate expression" << std::endl;
		return 0;
	}

	double a = m_left->Evaluate();
	double b = m_right->Evaluate();
	double res = 0;
	if (b == 0.0)
	{
		std::cout << "ModulusExpression: Attempt to apply modulu by zero: " << std::endl;
	}
	else 
		res = std::fmod(a, b);

	return res;
}

ExponentiationExpression::ExponentiationExpression(const ExpressionPtr &left, const ExpressionPtr &right)
	: ArithmeticExpression(left, right)
{
}

double 
ExponentiationExpression::Evaluate()
{
	if (m_left == nullptr || m_right == nullptr)
		return 0;

	double a = m_left->Evaluate();
	double b = m_right->Evaluate();
	return std::pow(a, b);
}

AssignmentExpression::AssignmentExpression(Parser *parser, const VariableExpressionPtr &v, const ExpressionPtr &val)
	: m_var(v), m_value(val)
{
	SetParser(parser);
}


double AssignmentExpression::Evaluate() 
{
	double x = 0;
	if (m_value)
		x = m_value->Evaluate();
	if (m_var) 
		m_parser->RecordVariable(m_var->GetVariable(), x);
	return x;
}

FunctionCallExpression::FunctionCallExpression(Parser *parser, const std::string &func, const ExpressionPtr &val)
	: m_function_name(func), m_value(val)
{
	SetParser(parser);
}

double 
FunctionCallExpression::Evaluate()
{
	double x(0);
	if (m_value)
		x = m_value->Evaluate();
	x = m_parser->EvaluateFunction(m_function_name, x);
	return x;
}
