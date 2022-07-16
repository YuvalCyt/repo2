#include "Expression.h"
#include "Parser.h"
#include "Tokenizer.h"

#include <iostream>


int main()
{
	std::cout << "Enter expressions: ";
	std::string line;
	Parser p;
	while (std::getline (std::cin, line)) {
		if (line.length() == 0)
			break;
		
		p.AddStatement(line);	
	}	

	p.EvaluateStatements();
	p.PrintVariables();

	return 0;
}