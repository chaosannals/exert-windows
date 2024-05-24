#pragma once

#include <string>
#include <format>
#include "token.h"

namespace ssc {
	struct lexeme {
		int row;
		int column;
		token token;
		std::string text;

		lexeme(
			int row,
			int column,
			ssc::token token,
			const std::string &text):
			row(row),
			column(column),
			token(token),
			text(text)
		{

		}

		lexeme &operator=(const lexeme& other){
			row = other.row;
			column = other.column;
			token = other.token;
			text = other.text;
			return *this;
		}

		std::string to_string() const {
			return std::format("[{}, {}] {}", row, column, text);
		}
	};
}
