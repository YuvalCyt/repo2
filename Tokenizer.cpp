#include "Tokenizer.h"
#include "Expression.h"
#include "Parser.h"
#include <cctype>

Tokenizer::Tokenizer() {
}

void 
Tokenizer::SetStatement(const std::string &statement)
{
	str.str(statement);
	m_length = statement.length();
	SetCurPos(0);
}

std::shared_ptr<NumberExpression> 
Tokenizer::EvaluateNumber()
{
	SkipWhiteSpaces();
	NumberExpressionPtr numExp;
	char ch = str.peek();
	if (std::isdigit(ch)) 
	{
		double x;
		str >> x;
		numExp = std::make_shared<NumberExpression>(x);
	}
	return numExp;
}

std::shared_ptr<VariableExpression> 
Tokenizer::EvalutateVariable() {
	SkipWhiteSpaces();
	VariableExpressionPtr valExp;
	char ch = str.peek();
	std::string s;
	bool bFirstChar=true;
	while (std::isalpha(ch) || (!bFirstChar && !isspace(ch) && isalnum(ch)))  //todo: what about underscore?
	{
		bFirstChar = false;
		ch = str.get();
		s += ch;
		ch = str.peek();
	}
	if (s.length() > 0)
	{
		valExp = std::make_shared<VariableExpression>(m_parser, s);	
	}

	return valExp;
}

ExpressionPtr 
Tokenizer::EvaluatePrefixFunction()
{
	int curPos = str.tellg();
	SkipWhiteSpaces();
	ExpressionPtr exp;
	std::string prefix(GetPostPreFixType());
	if (!prefix.empty())
	{
		std::string var_name(GetCurrentVariableName());
		if (!var_name.empty())
		{
			double addedValue = prefix == "++" ? 1 : -1;
			m_parser->RecordVariable(var_name, m_parser->LookupVariable(var_name) + addedValue);
			exp = std::make_shared<VariableExpression>(m_parser, var_name);	
			curPos += var_name.length() + prefix.length() + 1;
		}
	}

	SetCurPos(curPos);

	return exp;
}


ExpressionPtr 
Tokenizer::EvaluatePostfixFunction()
{
	int curPos = str.tellg();
	SkipWhiteSpaces();
	ExpressionPtr exp;
	std::string var_name(GetCurrentVariableName());
	if (!var_name.empty())
	{
		std::string postfix = GetPostPreFixType();
		if (!postfix.empty())
		{
			exp = std::make_shared<PostfixVariableExpression>(m_parser, var_name, postfix == "++");	
			curPos += var_name.length() + postfix.length() + 1;
		}
	}

	SetCurPos(curPos);

	return exp;
}

bool
Tokenizer::EvaluateCharacter(char expected) {
	SkipWhiteSpaces();
	bool expectedChar = false;
	char ch = str.peek();
	if (ch == expected) 
	{
		ch = str.get();
		expectedChar = true;
	}

	return expectedChar;
}

bool 
Tokenizer::EvaluateCharacters(const std::string &expected) {
	SkipWhiteSpaces();
	
	bool expectedChars = false;
	if (!ReachedEnd())
	{
		std::string s = str.str().substr(GetCurPos());
		if (s.find(expected) == 0)
		{
			str.seekg(GetCurPos() + expected.length());
			expectedChars = true;
		}
	}

	return expectedChars;
}

int 
Tokenizer::GetCurPos() {
	return str.tellg();
}

bool 
Tokenizer::ReachedEnd() {
	return str.eof();
}

void 
Tokenizer::SetCurPos(int curPos) {
	str.clear();
	str.seekg(std::min(m_length, curPos));
}

void 
Tokenizer::SkipWhiteSpaces() {
	char ch = str.peek();
	while (isspace(ch)) {
		ch = str.get();
		ch = str.peek();
	}
}

std::string 
Tokenizer::GetCurrentVariableName()
{
	std::string var_name;
	std::vector<std::string> names(m_parser->GetVariableNames());
	for (std::string &name : names)
	{
		if (EvaluateCharacters(name))
		{
			var_name = std::move(name);
			break;
		}
	}

	return var_name;
}

std::string
Tokenizer::GetPostPreFixType()
{
	std::string postfix;
	for (const std::string &postfix_type : {std::string("++"), std::string("--")})
	{
		if (EvaluateCharacters(postfix_type))
		{
			postfix = postfix_type;
			break;
		}
	}

	return postfix;
}
