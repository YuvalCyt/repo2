#pragma once
#include <string>
#include <memory>

class Parser;

/*
	Expression abstract class
*/
class Expression
{
public:
	Expression();
	virtual ~Expression() = default;

	void SetParser(Parser* parser) { m_parser = parser;}

	virtual double Evaluate() = 0;
protected:
	Parser* m_parser = nullptr;
	
};

using ExpressionPtr = std::shared_ptr<Expression>;

/*
	NumberExpression class represents a number
*/
class NumberExpression : public Expression
{
public:
	NumberExpression(double val);
	virtual double Evaluate() override;
private:
	double value;
};

using NumberExpressionPtr = std::shared_ptr<NumberExpression>;

/*
	VariableExpression class represents a variable
*/
class VariableExpression : public Expression
{
public:
	VariableExpression(Parser *parser, const std::string& var);
	virtual double Evaluate() override;
	std::string GetVariable() const { return variable; }
private:
	std::string variable;
};

using VariableExpressionPtr = std::shared_ptr<VariableExpression>;

/*
	ArithmeticExpression class represents a binary operation of 2 expressions
	e.g. lhs OP rhs
*/
class ArithmeticExpression : public Expression
{
public:
	ArithmeticExpression(const ExpressionPtr &left, const ExpressionPtr &right);
protected:
	virtual bool Validate() const;
	ExpressionPtr m_left;
	ExpressionPtr m_right;
};

using ArithmeticExpressionPtr = std::shared_ptr<ArithmeticExpression>;

/*
	AdditionExpression class represents an addition of 2 expressions
	e.g. lhs + rhs
*/
class AdditionExpression : public ArithmeticExpression
{
public:
	AdditionExpression(const ExpressionPtr &left, const ExpressionPtr &right);
	virtual double Evaluate() override;
};

using AdditionExpressionPtr = std::shared_ptr<AdditionExpression>;


/*
	SubstractionExpression class represents a substraction of 2 expressions
	e.g. lhs - rhs
*/
class SubstractionExpression : public ArithmeticExpression
{
public:
	SubstractionExpression(const ExpressionPtr &left, const ExpressionPtr &right);
	virtual double Evaluate() override;
};

using SubstractionExpressionPtr = std::shared_ptr<SubstractionExpression>;

/*
	MultiplicationExpression class represents a multiplication of 2 expressions
	e.g. lhs * rhs
*/
class MultiplicationExpression : public ArithmeticExpression
{
public:
	MultiplicationExpression(const ExpressionPtr &left, const ExpressionPtr &right);
	virtual double Evaluate() override;	
};

using MultiplicationExpressionPtr = std::shared_ptr<MultiplicationExpression>;

/*
	DivisionExpression class represents a division of 2 expressions
	e.g. lhs / rhs
*/
class DivisionExpression : public ArithmeticExpression
{
public:
	DivisionExpression(const ExpressionPtr &left, const ExpressionPtr &right);
	virtual double Evaluate() override;
};

using DivisionExpressionPtr = std::shared_ptr<DivisionExpression>;

/*
	ModulusExpression class represents a modulo of 2 expressions
	e.g. lhs % rhs
*/
class ModulusExpression : public ArithmeticExpression
{
public:
	ModulusExpression(const ExpressionPtr &left, const ExpressionPtr &right);
	virtual double Evaluate() override;
};

using ModulusExpressionPtr = std::shared_ptr<ModulusExpression>;

/*
	ExponentiationExpression class represents the base lhs and the exponent rhs
	e.g. lhs ^ rhs
*/
class ExponentiationExpression : public ArithmeticExpression
{
public:
	ExponentiationExpression(const ExpressionPtr &left, const ExpressionPtr &right);
	virtual double Evaluate() override;
};

using ExponentiationExpressionPtr = std::shared_ptr<ExponentiationExpression>;

/*
	ArithmeticExpressionPtr class represents assigning an expression to a variable
	e.g. lhs = rhs
	where lhs is a variable and rhs is an expression
*/
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

/*
	ArithmeticExpressionPtr class represents applying an expression on a function
	e.g. f(value)
*/
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
