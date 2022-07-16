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
	void set_parser(Parser *parser) { m_parser = parser;}
	void set_line(const std::string &line);
	std::shared_ptr<NumberExpression> number();
	std::shared_ptr<VariableExpression> variable();
	std::shared_ptr<Expression> prefix_function();
	std::shared_ptr<Expression> postfix_function();
	bool character(char expected);
	bool characters(const std::string &expected);
	bool atEnd();

	int mark();
	void reset(int mark);
private:
	std::stringstream str;
	Parser* m_parser = nullptr;

	void skipWhiteSpace();
	std::string get_current_variable_name();
	std::string get_post_pre_fix_type();
};
