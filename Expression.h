#pragma once
#include <map>
#include <string>
#include <memory>

class Parser;

class Expression
{
public:
	Expression();
	virtual ~Expression();

	void set_parser(Parser* parser) { m_parser = parser;}

	virtual double evaluate() = 0;
protected:
	Parser* m_parser = nullptr;
	
};

using ExpressionPtr = std::shared_ptr<Expression>;

class NumberExpression : public Expression
{
public:
	NumberExpression(double val);

	virtual double evaluate();
private:
	double value;
};

using NumberExpressionPtr = std::shared_ptr<NumberExpression>;

class VariableExpression : public Expression
{
public:
	VariableExpression(Parser *parser, const std::string& var);

	virtual double evaluate();

	std::string getVar() const { return variable; }
private:
	std::string variable;
};

class PostfixVariableExpression : public VariableExpression
{
public:
	PostfixVariableExpression(Parser *parser, const std::string& var, bool inc);
	virtual double evaluate();
private:
	bool m_inc;
};

using VariableExpressionPtr = std::shared_ptr<VariableExpression>;

class ArithmeticExpression : public Expression
{
public:
	ArithmeticExpression(const ExpressionPtr &left, const ExpressionPtr &right);
protected:
	ExpressionPtr left;
	ExpressionPtr right;
};

class AdditionExpression : public ArithmeticExpression
{
public:
	AdditionExpression(const ExpressionPtr &left, const ExpressionPtr &right);

	virtual double evaluate() override;
};

class SubstractionExpression : public ArithmeticExpression
{
public:
	SubstractionExpression(const ExpressionPtr &left, const ExpressionPtr &right);

	virtual double evaluate();
};

class MultiplicationExpression : public ArithmeticExpression
{
public:
	MultiplicationExpression(const ExpressionPtr &left, const ExpressionPtr &right);

	virtual double evaluate();
};

class DivisionExpression : public ArithmeticExpression
{
public:
	DivisionExpression(const ExpressionPtr &left, const ExpressionPtr &right);

	virtual double evaluate();
};

class ModulusExpression : public ArithmeticExpression
{
public:
	ModulusExpression(const ExpressionPtr &left, const ExpressionPtr &right);

	virtual double evaluate();
};

class ExponentiationExpression : public ArithmeticExpression
{
public:
	ExponentiationExpression(const ExpressionPtr &left, const ExpressionPtr &right);

	virtual double evaluate();
};

using ArithmeticExpressionPtr = std::shared_ptr<ArithmeticExpression>;

class AssignmentExpression : public Expression
{
public:
	AssignmentExpression(Parser *parser, const VariableExpressionPtr &var, const ExpressionPtr &value);

	virtual double evaluate();
private:
	VariableExpressionPtr var;
	ExpressionPtr value;
};

using AssignmentExpressionPtr = std::shared_ptr<AssignmentExpression>;

class FunctionCallExpression : public Expression
{
public:
	FunctionCallExpression(Parser *parser, const std::string &func, const ExpressionPtr &value);

	virtual double evaluate();
private:
	std::string function_name;
	ExpressionPtr value;

};
