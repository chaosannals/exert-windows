#pragma once

#include <sstream>
#include <fstream>
#include <vector>
#include <cctype>
#include "error.h"
#include "lexeme.h"

namespace ssc {

	class lexer {
		int row;
		int column;
		int last_char;
		std::istream &stream;
	public:
		lexer(std::istream &stream):
			row(1),
			column(0),
			last_char(' '),
			stream(stream)
		{
		}

		void next_char();
		void skip_char();
		lexeme new_lexeme(ssc::token token, const std::string& text);

		lexeme pop_lexeme();
		lexeme pop_punct();
		lexeme pop_number();
		lexeme pop_string();
		lexeme pop_identifier();
	};
}