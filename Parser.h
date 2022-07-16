#pragma once

#include "Expression.h"
#include "Tokenizer.h"
#include <vector>
#include <map>
#include <functional>

class Expression;
class Parser
{
public:

	struct VarEntry{
		VarEntry(const std::string &varName, double varValue)
			: m_name(varName)
			, m_value(varValue)
		{

		}

		std::string m_name;
		double m_value;
	};
	Parser();
	~Parser();
	void set_line(const std::string &line);

	ExpressionPtr statement();

	void print_variables() const;
	double lookup_variable(const std::string& var) const;
	void record_variable(const std::string& var, double value);
	std::vector<std::string> get_variable_names() const;

	double evaluate_function(const std::string &function_name, double value) const;

protected:
	ExpressionPtr assignment();
	ExpressionPtr calculation();
	ExpressionPtr sum();
	ExpressionPtr product();
	ExpressionPtr factor();
	ExpressionPtr power();
	ExpressionPtr term();
	ExpressionPtr group();
	ExpressionPtr function();
	//ExpressionPtr prefix_function();
	//ExpressionPtr postfix_function();

private:

	void build_functions_map();
	Tokenizer tokens;

	std::vector<VarEntry> m_vars;
	std::map<std::string, std::function<double(double)>> m_funcs;
};

