#include "parser.h"
#include <iostream>

void ssc::parser::parse() {
    ssc::lexeme lexeme = lex.pop_lexeme();
    while (lexeme.token != ssc::token::end_of_file) {
        std::cout << lexeme.to_string() << std::endl;
        lexeme = lex.pop_lexeme();
    }
}
