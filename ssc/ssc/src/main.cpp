#include "lexer.h"
#include "parser.h"
#include <iostream>

int main() {
	try {
		std::fstream file("res/demo.ssc");
		ssc::lexer lexer(file);
        ssc::parser parser(lexer);
        parser.parse();
	}
	catch (ssc::lex_error e) {
		std::cout << e.to_string() << std::endl;
	}
	std::getchar();
	return 0;
}
