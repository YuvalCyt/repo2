#pragma once

#include "Expression.h"
#include "Tokenizer.h"
#include <vector>
#include <map>
#include <functional>

class Expression;

/*
	Parser class to parse, interpret and evaluate CFG statements

	The grammar rules are:
	Statement -> Assignment OR Calculation
	Assignment -> Variable '=' Sum
	Calculation -> Sum
	Sum -> Product (('+' Product)|('-' Product))*
	Product -> Factor (('*' Factor)|('/' Factor))*
	Factor -> Power | Term
	Power -> Function | Term '^' Factor
	Term -> Group | Variable | Number
	Function -> PrefixFunction | PostFixFunction | FunctionCall
	PrefixFunction -> 'Variable' + 1
	PostFixFunction -> 'Variable'
	FunctionCall -> FunctionName '(' Sum ')'
	Group -> '(' Sum ')'
*/
class Parser
{
public:

	/*
		VarEntry utility class to hold data of the stored variables by the order of creation
	*/
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
	~Parser() = default;
	void AddStatement(std::string statement);
	void EvaluateStatements();

	ExpressionPtr EvaluateStatement();

	//Print variables to stdout according to the required format
	void PrintVariables() const;
	double LookupVariable(const std::string& var) const;
	void RecordVariable(const std::string& var, double value);
	std::vector<std::string> GetVariableNames() const;

	double EvaluateFunction(const std::string &function_name, double value) const;

protected:
	ExpressionPtr EvaluateAssignment();
	ExpressionPtr EvaluateCalculation();
	ExpressionPtr EvaluateSum();
	ExpressionPtr EvaluateProduct();
	ExpressionPtr EvaluateFactor();
	ExpressionPtr EvaluatePower();
	ExpressionPtr EvaluateTerm();
	ExpressionPtr EvaluateGroup();
	ExpressionPtr EvaluateFunction();

private:

	//Prepopulate the functions map that can be interpreted
	void BuildFunctionsMap();
	Tokenizer m_tokenizer;
	std::vector<std::string> m_statements;
	std::vector<VarEntry> m_vars;
	std::map<std::string, std::function<double(double)>> m_funcs;
};

