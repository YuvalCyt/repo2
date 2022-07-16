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
		
		p.set_line(line);
		ExpressionPtr e = p.statement();
		if (e) 
		{
			std::cout << e->evaluate() << std::endl;
		}
		else
			std::cout << "Error" << std::endl;		
	}
	
	p.print_variables();
	return 0;
}