#pragma once
#include <sstream>
#include <memory>
#include <vector>

class NumberExpression;
class VariableExpression;
class Expression;
class Parser;
/*
	Tokenizer class to evaluate expressions from a statement
*/
class Tokenizer
{
public:
	Tokenizer();
	void SetParser(Parser *parser) { m_parser = parser;}

	//Set the statement to evaluate
	void SetStatement(const std::string &statement);

	//Evaluate a number from an expression, e.g. "13.37"
	std::shared_ptr<NumberExpression> EvaluateNumber();

	//Evaluate a variable from an expression, e.g. "x123"
	std::shared_ptr<VariableExpression> EvalutateVariable();

	//Evaluate a prefix from an expression, e.g. "++i"
	std::shared_ptr<Expression> EvaluatePrefixFunction();

	//Evaluate a postfix from an expression, e.g. "j++"
	std::shared_ptr<Expression> EvaluatePostfixFunction();

	//Evaluate a single character from an expression, e.g. "x"
	bool EvaluateCharacter(char expected);

	//Evaluate a string from an expression, e.g. "xyz123"
	bool EvaluateCharacters(const std::string &expected);

	//Flag wheter we've read the whole statement
	bool ReachedEnd() const;

	//Get the current position of what was being read from the statement
	int GetCurrentPosition();

	//Set the current position of what was being read from the statement
	void SetCurrenPosition(int mark);
private:
	std::stringstream m_strstrm;
	int m_length = -1;
	Parser* m_parser = nullptr;

	void SkipWhiteSpaces();

	//Get the variable expression (if any) is in the current position
	std::string GetCurrentVariableName();

	//Get the current expression is prefix or postfix is in the current position
	std::string GetPostPreFixType();
};
