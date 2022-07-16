#include "Expression.h"
#include "Parser.h"
#include <cmath>
#include <functional>

Expression::Expression()
{
}
Expression::~Expression() {}

NumberExpression::NumberExpression(double val) 
	: value(val) 
{
}

double NumberExpression::evaluate() {
	return value;
}

VariableExpression::VariableExpression(Parser *parser, const std::string& var) 
	: variable(var)
{
	set_parser(parser);
}

double VariableExpression::evaluate() {
	double res(0);
	if (m_parser)
		res = m_parser->lookup_variable(variable);
	return res;
}

PostfixVariableExpression::PostfixVariableExpression(Parser *parser, const std::string& var, bool inc) 
	: VariableExpression(parser, var)
	, m_inc(inc)
{
}

double PostfixVariableExpression::evaluate() {
	double res(0);
	if (m_parser)
	{
		res = m_parser->lookup_variable(getVar());
		m_parser->record_variable(getVar(), m_inc ? res + 1 : res - 1); //flag for doing this once?
	}
		
	return res;
}

ArithmeticExpression::ArithmeticExpression(const ExpressionPtr &l, const ExpressionPtr &r)
	: left(l), right(r)
{
}

AdditionExpression::AdditionExpression(const ExpressionPtr &left, const ExpressionPtr &right)
	: ArithmeticExpression(left, right)
{

}

double 
AdditionExpression::evaluate()
{
	if (left == nullptr || right == nullptr)
		return 0;

	double a = left->evaluate();
	double b = right->evaluate();
	return a+b;
}

SubstractionExpression::SubstractionExpression(const ExpressionPtr &left, const ExpressionPtr &right)
	: ArithmeticExpression(left, right)
{

}

double 
SubstractionExpression::evaluate()
{
	if (left == nullptr || right == nullptr)
		return 0;

	double a = left->evaluate();
	double b = right->evaluate();
	return a-b;
}

MultiplicationExpression::MultiplicationExpression(const ExpressionPtr &left, const ExpressionPtr &right)
	: ArithmeticExpression(left, right)
{

}

double 
MultiplicationExpression::evaluate()
{
	if (left == nullptr || right == nullptr)
		return 0;

	double a = left->evaluate();
	double b = right->evaluate();
	return a*b;
}

DivisionExpression::DivisionExpression(const ExpressionPtr &left, const ExpressionPtr &right)
	: ArithmeticExpression(left, right)
{

}

double 
DivisionExpression::evaluate()
{
	if (left == nullptr || right == nullptr)
		return 0;

	double a = left->evaluate();
	double b = right->evaluate();
	//todo: check b?
	return a/b;
}

ModulusExpression::ModulusExpression(const ExpressionPtr &left, const ExpressionPtr &right)
	: ArithmeticExpression(left, right)
{

}

double 
ModulusExpression::evaluate()
{
	if (left == nullptr || right == nullptr)
		return 0;

	double a = left->evaluate();
	double b = right->evaluate();
	//todo: check b?
	return std::fmod(a, b);
}

ExponentiationExpression::ExponentiationExpression(const ExpressionPtr &left, const ExpressionPtr &right)
	: ArithmeticExpression(left, right)
{

}

double 
ExponentiationExpression::evaluate()
{
	if (left == nullptr || right == nullptr)
		return 0;

	double a = left->evaluate();
	double b = right->evaluate();
	return std::pow(a, b);
}

AssignmentExpression::AssignmentExpression(Parser *parser, const VariableExpressionPtr &v, const ExpressionPtr &val)
	: var(v), value(val)
{
	set_parser(parser);
}


double AssignmentExpression::evaluate() {
	double x = 0;
	if (value)
		x = value->evaluate();
	if (var) 
		m_parser->record_variable(var->getVar(), x);
	return x;
}

FunctionCallExpression::FunctionCallExpression(Parser *parser, const std::string &func, const ExpressionPtr &val)
	: function_name(func), value(val)
{
	set_parser(parser);
}

double 
FunctionCallExpression::evaluate()
{
	double x(0);
	if (value)
		x = value->evaluate();
	x = m_parser->evaluate_function(function_name, x);
	return x;
}
