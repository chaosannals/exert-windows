#include "lexer.h"
#include <iostream>

int main() {
	try {
		std::fstream file("res/demo.ssc");
		ssc::lexer lexer(file);
		ssc::lexeme lexeme = lexer.pop_lexeme();
		while (lexeme.token != ssc::token::end_of_file) {
			std::cout << lexeme.to_string() << std::endl;
			lexeme = lexer.pop_lexeme();
		}
	}
	catch (ssc::lex_error e) {
		std::cout << e.to_string() << std::endl;
	}
	std::getchar();
	return 0;
}