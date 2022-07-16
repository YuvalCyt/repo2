#include "Tokenizer.h"
#include "Expression.h"
#include "Parser.h"
#include <cctype>

Tokenizer::Tokenizer() {
}

void 
Tokenizer::set_line(const std::string &line)
{
	str.str(line);
	reset(0);
}

std::shared_ptr<NumberExpression> Tokenizer::number()
{
	skipWhiteSpace();
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

std::shared_ptr<VariableExpression> Tokenizer::variable() {
	skipWhiteSpace();
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

std::shared_ptr<Expression> Tokenizer::prefix_function()
{
	int mark = str.tellg();
	skipWhiteSpace();
	ExpressionPtr exp;
	std::string prefix(get_post_pre_fix_type());
	if (!prefix.empty())
	{
		std::string var_name(get_current_variable_name());
		if (!var_name.empty())
		{
			double addedValue = prefix == "++" ? 1 : -1;
			m_parser->record_variable(var_name, m_parser->lookup_variable(var_name) + addedValue);
			exp = std::make_shared<VariableExpression>(m_parser, var_name);	
			mark += var_name.length() + prefix.length() + 1;
		}
	}

	reset(mark);

	return exp;
}


ExpressionPtr Tokenizer::postfix_function()
{
	int mark = str.tellg();
	skipWhiteSpace();
	ExpressionPtr exp;
	std::string var_name(get_current_variable_name());
	if (!var_name.empty())
	{
		std::string postfix = get_post_pre_fix_type();
		if (!postfix.empty())
		{
			exp = std::make_shared<PostfixVariableExpression>(m_parser, var_name, postfix == "++");	
			mark += var_name.length() + postfix.length() + 1;
		}
	}

	reset(mark);

	return exp;
}

bool Tokenizer::character(char expected) {
	skipWhiteSpace();
	bool expectedChar = false;
	char ch = str.peek();
	if (ch == expected) 
	{
		ch = str.get();
		expectedChar = true;
	}

	return expectedChar;
}

bool Tokenizer::characters(const std::string &expected) {
	skipWhiteSpace();
	
	bool expectedChars = false;
	if (!atEnd())
	{
		std::string s = str.str().substr(mark());
		if (s.find(expected) == 0)
		{
			str.seekg(mark() + expected.length());
			expectedChars = true;
		}
	}

	return expectedChars;
}

int Tokenizer::mark() {
	return str.tellg();
}

bool Tokenizer::atEnd() {
	return str.eof();
}

void Tokenizer::reset(int mark) {
	str.clear();
	str.seekg(mark);
}

void Tokenizer::skipWhiteSpace() {
	char ch = str.peek();
	while (isspace(ch)) {
		ch = str.get();
		ch = str.peek();
	}
}

std::string 
Tokenizer::get_current_variable_name()
{
	std::string var_name;
	std::vector<std::string> names(m_parser->get_variable_names());
	for (std::string &name : names)
	{
		if (characters(name))
		{
			var_name = std::move(name);
			break;
		}
	}

	return var_name;
}

std::string
Tokenizer::get_post_pre_fix_type()
{
	std::string postfix;
	for (const std::string &postfix_type : {std::string("++"), std::string("--")})
	{
		if (characters(postfix_type))
		{
			postfix = postfix_type;
			break;
		}
	}

	return postfix;
}
