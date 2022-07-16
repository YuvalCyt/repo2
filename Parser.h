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
	void AddStatement(std::string statement);
	void EvaluateStatements();
	void set_line(const std::string &line);

	ExpressionPtr EvaluateStatement();

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

	void BuildFunctionsMap();
	Tokenizer m_tokenizer;
	std::vector<std::string> m_statements;
	std::vector<VarEntry> m_vars;
	std::map<std::string, std::function<double(double)>> m_funcs;
};

