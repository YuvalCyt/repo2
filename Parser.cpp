#include "Parser.h"
#include "Expression.h"
#include "Tokenizer.h"

#include <cmath>
#include <iostream>

Parser::Parser()
{
	m_tokenizer.SetParser(this);
	BuildFunctionsMap();
}

Parser::~Parser()
{
}

void
Parser::AddStatement(std::string &statement)
{
	m_statements.emplace_back(std::move(statement));
}

void
Parser::EvaluateStatements()
{
	for (const std::string &statement : m_statements)
	{
		m_tokenizer.SetStatement(statement); 
		ExpressionPtr exp = this->EvaluateStatement();
		if (exp)
		{
			exp->Evaluate();
		}
		else
		{
			//error?
		}
	}
}

//todo: remove function
void
Parser::set_line(const std::string &line)
{
	m_tokenizer.SetStatement(line);
}

void
Parser::BuildFunctionsMap()
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
Parser::LookupVariable(const std::string& var) const
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
Parser::GetVariableNames() const
{
	std::vector<std::string> names;
	names.reserve(m_vars.size());
	for (const auto &varEntry : m_vars)
		names.push_back(varEntry.m_name);

	std::sort(names.begin(), names.end(), 
		[] (const std::string& first, const std::string& second){
        	return first.size() > second.size();
    });

	return names;
}

void 
Parser::PrintVariables() const
{
	std::cout << "(";
	bool first = true;
	for (const auto &varEntry: m_vars) 
	{
   		if (first) 
			first = false; 
		else 
			std::cout << ","; 
			
		std::cout << varEntry.m_name << "=" << varEntry.m_value; 
	}

	std::cout << ")" << std::endl;
}

void 
Parser::RecordVariable(const std::string& var, double value)
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
Parser::EvaluateFunction(const std::string &function_name, double value) const
{
	double res(0);
	auto find = m_funcs.find(function_name);
	if (find != m_funcs.end())
		res = find->second(value);

	return res;
}

ExpressionPtr Parser::EvaluateStatement() {
	ExpressionPtr exp(EvaluateAssignment());
	if (!exp)
		exp = EvaluateCalculation();
	return exp;
}

ExpressionPtr Parser::EvaluateAssignment()
{
	int curPos = m_tokenizer.GetCurPos();
	ExpressionPtr exp;

	std::shared_ptr<VariableExpression> var(m_tokenizer.EvalutateVariable());
	ExpressionPtr rhs;
	if (var)
	{
		if (m_tokenizer.EvaluateCharacter('=') && (rhs=EvaluateSum()) && m_tokenizer.ReachedEnd()) 
			exp = std::make_shared<AssignmentExpression>(this, var, rhs);
		else
		{
			ExpressionPtr newRhs;
			bool varExist(false);
			for (const auto &varEntry : m_vars)
				if (varEntry.m_name == var->GetVariable())
				{
					varExist = true;
					break;
				}

			if (varExist)
			{
				//can do such operation for defined variables only
				if (m_tokenizer.EvaluateCharacters("+=") && (rhs=EvaluateSum()) && m_tokenizer.ReachedEnd())
				{
					newRhs = std::make_shared<AdditionExpression>(var, rhs);
				}
				else if (m_tokenizer.EvaluateCharacters("-=") && (rhs=EvaluateSum()) && m_tokenizer.ReachedEnd())
				{
					newRhs = std::make_shared<SubstractionExpression>(var, rhs);
				}
				else if (m_tokenizer.EvaluateCharacters("*=") && (rhs=EvaluateSum()) && m_tokenizer.ReachedEnd())
				{
					newRhs = std::make_shared<MultiplicationExpression>(var, rhs);
				}
				else if (m_tokenizer.EvaluateCharacters("/=") && (rhs=EvaluateSum()) && m_tokenizer.ReachedEnd())
				{
					newRhs = std::make_shared<DivisionExpression>(var, rhs);
				}
				else if (m_tokenizer.EvaluateCharacters("^=") && (rhs=EvaluateSum()) && m_tokenizer.ReachedEnd())
				{
					newRhs = std::make_shared<ExponentiationExpression>(var, rhs);
				}
				else if (m_tokenizer.EvaluateCharacters("%=") && (rhs=EvaluateSum()) && m_tokenizer.ReachedEnd())
				{
					newRhs = std::make_shared<ModulusExpression>(var, rhs);
				}
			}
		
			if (newRhs)
				exp = std::make_shared<AssignmentExpression>(this, var, newRhs);	
		}
		
	}
	if (!exp)
		m_tokenizer.SetCurPos(curPos);
	return exp;
}

ExpressionPtr Parser::EvaluateCalculation()
{
	ExpressionPtr result;
	if ((result=EvaluateSum()) && m_tokenizer.ReachedEnd())
		return result;
	if(result) 
		result = nullptr;
	return result;
}

ExpressionPtr Parser::EvaluateSum()
{
	int curPos = m_tokenizer.GetCurPos();
	ExpressionPtr lhs = EvaluateProduct();
	ExpressionPtr rhs;

	while (lhs) 
	{
		if (m_tokenizer.EvaluateCharacter('+')) {
			rhs=EvaluateProduct();
			if (rhs)
				lhs = std::make_shared<AdditionExpression>(lhs, rhs);
			else 
				lhs = nullptr;
		}
		else if (m_tokenizer.EvaluateCharacter('-')) 
		{
			rhs=EvaluateProduct();
			if (rhs)
				lhs = std::make_shared<SubstractionExpression>(lhs, rhs);
			else 
				lhs = nullptr;
		}
		else
			break;
	}

	if (lhs == nullptr)
		m_tokenizer.SetCurPos(curPos);
	return lhs;
}

ExpressionPtr Parser::EvaluateProduct()
{
	int curPos = m_tokenizer.GetCurPos();
	ExpressionPtr lhs = EvaluateFactor();
	ExpressionPtr rhs;
	while (lhs) 
	{
		if (m_tokenizer.EvaluateCharacter('*')) {
			if (rhs=EvaluateFactor())
				lhs = std::make_shared<MultiplicationExpression>(lhs, rhs);
			else 
				lhs = nullptr;
		}
		else if (m_tokenizer.EvaluateCharacter('/')) {
			if (rhs=EvaluateFactor())
				lhs = std::make_shared<DivisionExpression>(lhs, rhs);
			else 
				lhs = nullptr;
		}
		else if (m_tokenizer.EvaluateCharacter('%')){
			if (rhs=EvaluateFactor())
				lhs = std::make_shared<ModulusExpression>(lhs, rhs);
			else 
				lhs = nullptr;
		}
		else
			break;
	}
	if (!lhs)
		m_tokenizer.SetCurPos(curPos); 
	return lhs;
}

ExpressionPtr Parser::EvaluateFactor() {
	ExpressionPtr exp = nullptr;
	if ((exp=EvaluatePower()) || (exp=EvaluateTerm()))
		;
	return exp;
}

ExpressionPtr Parser::EvaluatePower()
{
	int curPos = m_tokenizer.GetCurPos();
	ExpressionPtr exp;
	if (exp=EvaluateFunction())
		return exp;
	ExpressionPtr lhs;
	ExpressionPtr rhs;
	if ((lhs=EvaluateTerm()) && m_tokenizer.EvaluateCharacter('^') && (rhs=EvaluateFactor()))
		exp = std::make_shared<ExponentiationExpression>(lhs, rhs);
	else
		m_tokenizer.SetCurPos(curPos);
	return exp;
}

ExpressionPtr Parser::EvaluateTerm()
{
	ExpressionPtr exp;
	if ((exp=EvaluateGroup()) || (exp=EvaluateFunction()) || (exp=m_tokenizer.EvalutateVariable()) || (exp=m_tokenizer.EvaluateNumber()))
		;
	return exp;
}

ExpressionPtr Parser::EvaluateGroup() {
	int curPos = m_tokenizer.GetCurPos();
	ExpressionPtr exp;
	if (m_tokenizer.EvaluateCharacter('(') && (exp=EvaluateSum()) && (m_tokenizer.EvaluateCharacter(')')))
		return exp;
	else
	{
		exp = nullptr;
		m_tokenizer.SetCurPos(curPos);
	}

	return exp;
}

ExpressionPtr Parser::EvaluateFunction()
{
	int curPos = m_tokenizer.GetCurPos();
	ExpressionPtr func_exp, exp;
	if ((exp=m_tokenizer.EvaluatePrefixFunction()) || (exp=m_tokenizer.EvaluatePostfixFunction()))
		return exp;
	std::string func_name;
	for (const auto &func_pair : m_funcs)
	{
		if (m_tokenizer.EvaluateCharacters(func_pair.first))
		{
			func_name = func_pair.first;
			break;
		}
	}

	if (!func_name.empty() && m_tokenizer.EvaluateCharacter('(') && (exp=EvaluateSum()) && (m_tokenizer.EvaluateCharacter(')')))
	{
		func_exp = std::make_shared<FunctionCallExpression>(this, func_name, exp);
	}
	else
	{
		m_tokenizer.SetCurPos(curPos);
	}

	return func_exp;
}
