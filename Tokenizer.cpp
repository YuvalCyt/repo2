#include "Tokenizer.h"
#include "Expression.h"
#include "Parser.h"

#include <cctype>

Tokenizer::Tokenizer() 
{
}

void 
Tokenizer::SetStatement(const std::string &statement)
{
	m_strstrm.str(statement);
	m_length = statement.length();
	SetCurrenPosition(0);
}

std::shared_ptr<NumberExpression> 
Tokenizer::EvaluateNumber()
{
	SkipWhiteSpaces();
	NumberExpressionPtr numExp;
	char curChar = m_strstrm.peek();
	if (std::isdigit(curChar)) 
	{
		double x(0.0);
		m_strstrm >> x;
		numExp = std::make_shared<NumberExpression>(x);
	}
	return numExp;
}

std::shared_ptr<VariableExpression> 
Tokenizer::EvalutateVariable() 
{
	SkipWhiteSpaces();
	VariableExpressionPtr valExp;
	char curChar = m_strstrm.peek();
	std::string s;
	bool bFirstChar=true;
	while (std::isalpha(curChar) || (!bFirstChar && !isspace(curChar) && isalnum(curChar)))  //todo: what about underscore?
	{
		bFirstChar = false;
		curChar = m_strstrm.get();
		s += curChar;
		curChar = m_strstrm.peek();
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
	int curPos = GetCurrentPosition();
	SkipWhiteSpaces();
	ExpressionPtr exp;
	std::string prefix(GetPostPreFixType());
	if (!prefix.empty())
	{
		std::string var_name(GetCurrentVariableName());
		if (!var_name.empty())
		{
			double addedValue = prefix == "++" ? 1 : -1;
			double varValue = m_parser->LookupVariable(var_name);
			m_parser->RecordVariable(var_name, varValue + addedValue);
			exp = std::make_shared<NumberExpression>(varValue + addedValue);
			curPos = GetCurrentPosition();
		}
	}

	SetCurrenPosition(curPos);

	return exp;
}


ExpressionPtr 
Tokenizer::EvaluatePostfixFunction()
{
	int curPos = GetCurrentPosition();
	SkipWhiteSpaces();
	ExpressionPtr exp;
	std::string var_name(GetCurrentVariableName());
	if (!var_name.empty())
	{
		std::string postfix = GetPostPreFixType();
		if (!postfix.empty())
		{
			double addedValue = postfix == "++" ? 1 : -1;
			double varValue = m_parser->LookupVariable(var_name);
			m_parser->RecordVariable(var_name, varValue + addedValue);
			exp = std::make_shared<NumberExpression>(varValue);
			curPos = GetCurrentPosition();
		}
	}

	SetCurrenPosition(curPos);

	return exp;
}

bool
Tokenizer::EvaluateCharacter(char expected)
{
	SkipWhiteSpaces();
	bool expectedChar = false;
	char ch = m_strstrm.peek();
	if (ch == expected) 
	{
		ch = m_strstrm.get();
		expectedChar = true;
	}

	return expectedChar;
}

bool 
Tokenizer::EvaluateCharacters(const std::string &expected) 
{
	int curPos = GetCurrentPosition();
	SkipWhiteSpaces();
	
	bool expectedChars = false;
	if (!ReachedEnd())
	{
		std::string s = m_strstrm.str().substr(GetCurrentPosition());
		if (s.find(expected) == 0)
		{
			m_strstrm.seekg(GetCurrentPosition() + expected.length());
			expectedChars = true;
		}
	}
	if (!expectedChars)
		SetCurrenPosition(curPos);

	return expectedChars;
}

int 
Tokenizer::GetCurrentPosition() 
{
	return m_strstrm.tellg();
}

bool 
Tokenizer::ReachedEnd() const
{
	return m_strstrm.eof();
}

void 
Tokenizer::SetCurrenPosition(int curPos) 
{
	m_strstrm.clear();
	m_strstrm.seekg(std::min(m_length, curPos));
}

void 
Tokenizer::SkipWhiteSpaces() 
{
	char curChar = m_strstrm.peek();
	while (isspace(curChar)) {
		curChar = m_strstrm.get();
		curChar = m_strstrm.peek();
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
