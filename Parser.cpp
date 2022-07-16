#include "Parser.h"
#include "Expression.h"
#include "Tokenizer.h"

#include <cmath>
#include <iostream>

Parser::Parser()
{
	tokens.set_parser(this);
	build_functions_map();
}

Parser::~Parser()
{
}

void
Parser::set_line(const std::string &line)
{
	tokens.set_line(line);
}

void
Parser::build_functions_map()
{
	typedef double (*DoubleFuncPtr)(double);
	m_funcs.insert(std::make_pair("sin", static_cast<DoubleFuncPtr>(std::sin)));
	m_funcs.insert(std::make_pair("asin", static_cast<DoubleFuncPtr>(std::asin)));
	m_funcs.insert(std::make_pair("cos", static_cast<DoubleFuncPtr>(std::cos)));
	m_funcs.insert(std::make_pair("acos", static_cast<DoubleFuncPtr>(std::acos)));
	m_funcs.insert(std::make_pair("tan", static_cast<DoubleFuncPtr>(std::tan)));
	m_funcs.insert(std::make_pair("atan", static_cast<DoubleFuncPtr>(std::atan)));
	m_funcs.insert(std::make_pair("atan", static_cast<DoubleFuncPtr>(std::atan)));
	m_funcs.insert(std::make_pair("ceil", static_cast<DoubleFuncPtr>(std::ceil)));
	m_funcs.insert(std::make_pair("floor", static_cast<DoubleFuncPtr>(std::floor)));
}

double 
Parser::lookup_variable(const std::string& var) const
{
	double res(0);
	for (const auto &varEntry : m_vars)
		if (varEntry.m_name == var)
		{
			res = varEntry.m_value;
			break;
		}
	return res;
}

std::vector<std::string>
Parser::get_variable_names() const
{
	std::vector<std::string> names;
	names.reserve(m_vars.size());
	for (const auto &varEntry : m_vars)
		names.push_back(varEntry.m_name);

	std::sort(names.begin(), names.end(), [] (const std::string& first, const std::string& second){
        return first.size() > second.size();
    });

	return names;
}

void 
Parser::print_variables() const
{
	std::cout << "(";
	bool first = true;
	for (const auto &varEntry: m_vars) 
	{
   		if (first) 
		{ 
			first = false; 
		} else 
		{ 
			std::cout << ","; 
		}
		std::cout << varEntry.m_name << "=" << varEntry.m_value; 
	}

	std::cout << ")" << std::endl;
	
}

void 
Parser::record_variable(const std::string& var, double value)
{
	for (auto &varEntry: m_vars) 
	{
		if (varEntry.m_name == var)
		{
			varEntry.m_value = value;
			return;
		}
	}

	m_vars.push_back(VarEntry(var, value));
}

double 
Parser::evaluate_function(const std::string &function_name, double value) const
{
	double res(0);
	auto find = m_funcs.find(function_name);
	if (find != m_funcs.end())
		res = find->second(value);

	return res;
}

ExpressionPtr Parser::statement() {
	ExpressionPtr exp(assignment());
	if (!exp)
		exp = calculation();
	return exp;
}

ExpressionPtr Parser::assignment()
{
	int mark = tokens.mark();
	ExpressionPtr exp;

	std::shared_ptr<VariableExpression> var(tokens.variable());
	ExpressionPtr rhs;
	if (var)
	{
		if (tokens.character('=') && (rhs=sum()) && tokens.atEnd()) 
			exp = std::make_shared<AssignmentExpression>(this, var, rhs);
		else
		{
			ExpressionPtr newRhs;
			bool varExist(false);
			for (const auto &varEntry : m_vars)
				if (varEntry.m_name == var->getVar())
				{
					varExist = true;
					break;
				}

			if (varExist)
			{
				//can do such operation for defined variables only
				if (tokens.characters("+=") && (rhs=sum()) && tokens.atEnd())
				{
					newRhs = std::make_shared<AdditionExpression>(var, rhs);
				}
				else if (tokens.characters("-=") && (rhs=sum()) && tokens.atEnd())
				{
					newRhs = std::make_shared<SubstractionExpression>(var, rhs);
				}
				else if (tokens.characters("*=") && (rhs=sum()) && tokens.atEnd())
				{
					newRhs = std::make_shared<MultiplicationExpression>(var, rhs);
				}
				else if (tokens.characters("/=") && (rhs=sum()) && tokens.atEnd())
				{
					newRhs = std::make_shared<DivisionExpression>(var, rhs);
				}
				else if (tokens.characters("^=") && (rhs=sum()) && tokens.atEnd())
				{
					newRhs = std::make_shared<ExponentiationExpression>(var, rhs);
				}
				else if (tokens.characters("%=") && (rhs=sum()) && tokens.atEnd())
				{
					newRhs = std::make_shared<ModulusExpression>(var, rhs);
				}
			}
		
			if (newRhs)
				exp = std::make_shared<AssignmentExpression>(this, var, newRhs);	
		}
		
	}
	if (!exp)
		tokens.reset(mark);
	return exp;
}

ExpressionPtr Parser::calculation()
{
	ExpressionPtr result;
	if ((result=sum()) && tokens.atEnd())
		return result;
	if(result) 
		result = nullptr;
	return result;
}

ExpressionPtr Parser::sum()
{
	int mark = tokens.mark();
	ExpressionPtr lhs = product();
	ExpressionPtr rhs;

	while (lhs) 
	{
		if (tokens.character('+')) {
			rhs=product();
			if (rhs)
				lhs = std::make_shared<AdditionExpression>(lhs, rhs);
			else 
				lhs = nullptr;
		}
		else if (tokens.character('-')) 
		{
			rhs=product();
			if (rhs)
				lhs = std::make_shared<SubstractionExpression>(lhs, rhs);
			else 
				lhs = nullptr;
		}
		else
			break;
	}

	if (lhs == nullptr)
		tokens.reset(mark);
	return lhs;
}

ExpressionPtr Parser::product()
{
	int mark = tokens.mark();
	ExpressionPtr lhs = factor();
	ExpressionPtr rhs;
	while (lhs) 
	{
		if (tokens.character('*')) {
			if (rhs=factor())
				lhs = std::make_shared<MultiplicationExpression>(lhs, rhs);
			else 
				lhs = nullptr;
		}
		else if (tokens.character('/')) {
			if (rhs=factor())
				lhs = std::make_shared<DivisionExpression>(lhs, rhs);
			else 
				lhs = nullptr;
		}
		else if (tokens.character('%')){
			if (rhs=factor())
				lhs = std::make_shared<ModulusExpression>(lhs, rhs);
			else 
				lhs = nullptr;
		}
		else
			break;
	}
	if (!lhs)
		tokens.reset(mark); 
	return lhs;
}

ExpressionPtr Parser::factor() {
	ExpressionPtr exp = nullptr;
	if ((exp=power()) || (exp=term()))
		;
	return exp;
}

ExpressionPtr Parser::power()
{
	int mark = tokens.mark();
	ExpressionPtr exp;
	if (exp=function())
		return exp;
	ExpressionPtr lhs;
	ExpressionPtr rhs;
	if ((lhs=term()) && tokens.character('^') && (rhs=factor()))
		exp = std::make_shared<ExponentiationExpression>(lhs, rhs);
	else
		tokens.reset(mark);
	return exp;
}

ExpressionPtr Parser::term()
{
	ExpressionPtr exp;
	if ((exp=group()) || (exp=function()) || (exp=tokens.variable()) || (exp=tokens.number()))
		;
	return exp;
}

ExpressionPtr Parser::group() {
	int mark = tokens.mark();
	ExpressionPtr exp;
	if (tokens.character('(') && (exp=sum()) && (tokens.character(')')))
		return exp;
	else
	{
		exp = nullptr;
		tokens.reset(mark);
	}

	return exp;
}

ExpressionPtr Parser::function()
{
	int mark = tokens.mark();
	ExpressionPtr func_exp, exp;
	if ((exp=tokens.prefix_function()) || (exp=tokens.postfix_function()))
		return exp;
	std::string func_name;
	for (const auto &func_pair : m_funcs)
	{
		if (tokens.characters(func_pair.first))
		{
			func_name = func_pair.first;
			break;
		}
	}

	if (!func_name.empty() && tokens.character('(') && (exp=sum()) && (tokens.character(')')))
	{
		func_exp = std::make_shared<FunctionCallExpression>(this, func_name, exp);
	}
	else
	{
		tokens.reset(mark);
	}

	return func_exp;
}
