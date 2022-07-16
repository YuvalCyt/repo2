#pragma once
#include <string>
#include <memory>

class Parser;

class Expression
{
public:
	Expression();
	virtual ~Expression();

	void SetParser(Parser* parser) { m_parser = parser;}

	virtual double Evaluate() = 0;
protected:
	Parser* m_parser = nullptr;
	
};

using ExpressionPtr = std::shared_ptr<Expression>;

class NumberExpression : public Expression
{
public:
	NumberExpression(double val);
	virtual double Evaluate() override;
private:
	double value;
};

using NumberExpressionPtr = std::shared_ptr<NumberExpression>;

class VariableExpression : public Expression
{
public:
	VariableExpression(Parser *parser, const std::string& var);
	virtual double Evaluate() override;
	std::string GetVariable() const { return variable; }
private:
	std::string variable;
};

class PostfixVariableExpression : public VariableExpression
{
public:
	PostfixVariableExpression(Parser *parser, const std::string& var, bool inc);
	virtual double Evaluate() override;
private:
	bool m_inc;
	bool m_firstIncrement = true;
};

using VariableExpressionPtr = std::shared_ptr<VariableExpression>;

class ArithmeticExpression : public Expression
{
public:
	ArithmeticExpression(const ExpressionPtr &left, const ExpressionPtr &right);
protected:
	virtual bool Validate() const;
	ExpressionPtr m_left;
	ExpressionPtr m_right;
};

class AdditionExpression : public ArithmeticExpression
{
public:
	AdditionExpression(const ExpressionPtr &left, const ExpressionPtr &right);
	virtual double Evaluate() override;
};

class SubstractionExpression : public ArithmeticExpression
{
public:
	SubstractionExpression(const ExpressionPtr &left, const ExpressionPtr &right);
	virtual double Evaluate() override;
};

class MultiplicationExpression : public ArithmeticExpression
{
public:
	MultiplicationExpression(const ExpressionPtr &left, const ExpressionPtr &right);
	virtual double Evaluate() override;	
};

class DivisionExpression : public ArithmeticExpression
{
public:
	DivisionExpression(const ExpressionPtr &left, const ExpressionPtr &right);
	virtual double Evaluate() override;
};

class ModulusExpression : public ArithmeticExpression
{
public:
	ModulusExpression(const ExpressionPtr &left, const ExpressionPtr &right);
	virtual double Evaluate() override;
};

class ExponentiationExpression : public ArithmeticExpression
{
public:
	ExponentiationExpression(const ExpressionPtr &left, const ExpressionPtr &right);
	virtual double Evaluate() override;
};

using ArithmeticExpressionPtr = std::shared_ptr<ArithmeticExpression>;

class AssignmentExpression : public Expression
{
public:
	AssignmentExpression(Parser *parser, const VariableExpressionPtr &var, const ExpressionPtr &value);
	virtual double Evaluate() override;	
private:
	VariableExpressionPtr m_var;
	ExpressionPtr m_value;
};
using AssignmentExpressionPtr = std::shared_ptr<AssignmentExpression>;

class FunctionCallExpression : public Expression
{
public:
	FunctionCallExpression(Parser *parser, const std::string &func, const ExpressionPtr &value);
	virtual double Evaluate() override;	
private:
	std::string m_function_name;
	ExpressionPtr m_value;
};
using FunctionCallExpressionPtr = std::shared_ptr<FunctionCallExpression>;
