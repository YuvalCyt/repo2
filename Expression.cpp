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

double NumberExpression::Evaluate() {
	return value;
}

VariableExpression::VariableExpression(Parser *parser, const std::string& var) 
	: variable(var)
{
	SetParser(parser);
}

double VariableExpression::Evaluate() {
	double res(0);
	if (m_parser)
		res = m_parser->LookupVariable(variable);
	return res;
}

PostfixVariableExpression::PostfixVariableExpression(Parser *parser, const std::string& var, bool inc) 
	: VariableExpression(parser, var)
	, m_inc(inc)
{
}

double PostfixVariableExpression::Evaluate() {
	double res(0);
	if (m_parser)
	{
		res = m_parser->LookupVariable(GetVariable());
		m_parser->RecordVariable(GetVariable(), m_inc ? res + 1 : res - 1); //flag for doing this once?
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
AdditionExpression::Evaluate()
{
	if (left == nullptr || right == nullptr)
		return 0;

	double a = left->Evaluate();
	double b = right->Evaluate();
	return a+b;
}

SubstractionExpression::SubstractionExpression(const ExpressionPtr &left, const ExpressionPtr &right)
	: ArithmeticExpression(left, right)
{

}

double 
SubstractionExpression::Evaluate()
{
	if (left == nullptr || right == nullptr)
		return 0;

	double a = left->Evaluate();
	double b = right->Evaluate();
	return a-b;
}

MultiplicationExpression::MultiplicationExpression(const ExpressionPtr &left, const ExpressionPtr &right)
	: ArithmeticExpression(left, right)
{

}

double 
MultiplicationExpression::Evaluate()
{
	if (left == nullptr || right == nullptr)
		return 0;

	double a = left->Evaluate();
	double b = right->Evaluate();
	return a*b;
}

DivisionExpression::DivisionExpression(const ExpressionPtr &left, const ExpressionPtr &right)
	: ArithmeticExpression(left, right)
{

}

double 
DivisionExpression::Evaluate()
{
	if (left == nullptr || right == nullptr)
		return 0;

	double a = left->Evaluate();
	double b = right->Evaluate();
	//todo: check b?
	return a/b;
}

ModulusExpression::ModulusExpression(const ExpressionPtr &left, const ExpressionPtr &right)
	: ArithmeticExpression(left, right)
{

}

double 
ModulusExpression::Evaluate()
{
	if (left == nullptr || right == nullptr)
		return 0;

	double a = left->Evaluate();
	double b = right->Evaluate();
	//todo: check b?
	return std::fmod(a, b);
}

ExponentiationExpression::ExponentiationExpression(const ExpressionPtr &left, const ExpressionPtr &right)
	: ArithmeticExpression(left, right)
{

}

double 
ExponentiationExpression::Evaluate()
{
	if (left == nullptr || right == nullptr)
		return 0;

	double a = left->Evaluate();
	double b = right->Evaluate();
	return std::pow(a, b);
}

AssignmentExpression::AssignmentExpression(Parser *parser, const VariableExpressionPtr &v, const ExpressionPtr &val)
	: var(v), value(val)
{
	SetParser(parser);
}


double AssignmentExpression::Evaluate() {
	double x = 0;
	if (value)
		x = value->Evaluate();
	if (var) 
		m_parser->RecordVariable(var->GetVariable(), x);
	return x;
}

FunctionCallExpression::FunctionCallExpression(Parser *parser, const std::string &func, const ExpressionPtr &val)
	: function_name(func), value(val)
{
	SetParser(parser);
}

double 
FunctionCallExpression::Evaluate()
{
	double x(0);
	if (value)
		x = value->Evaluate();
	x = m_parser->EvaluateFunction(function_name, x);
	return x;
}
