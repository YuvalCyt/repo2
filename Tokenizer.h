#pragma once
#include <sstream>
#include <memory>
#include <vector>

class NumberExpression;
class VariableExpression;
class Expression;
class Parser;

class Tokenizer
{
public:
	Tokenizer();
	void SetParser(Parser *parser) { m_parser = parser;}
	void SetStatement(const std::string &statement);
	std::shared_ptr<NumberExpression> EvaluateNumber();
	std::shared_ptr<VariableExpression> EvalutateVariable();
	std::shared_ptr<Expression> EvaluatePrefixFunction();
	std::shared_ptr<Expression> EvaluatePostfixFunction();
	bool EvaluateCharacter(char expected);
	bool EvaluateCharacters(const std::string &expected);
	bool ReachedEnd();

	int GetCurPos();
	void SetCurPos(int mark);
private:
	std::stringstream m_strstrm;
	int m_length = -1;
	Parser* m_parser = nullptr;

	void SkipWhiteSpaces();
	std::string GetCurrentVariableName();
	std::string GetPostPreFixType();
};
