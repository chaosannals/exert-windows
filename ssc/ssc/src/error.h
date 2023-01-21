#pragma once

#include <string>
#include <format>

namespace ssc {
	struct compile_error {
		const int row;
		const int column;
		compile_error(int row, int column) :
			row(row),
			column(column)
		{

		}
		virtual ~compile_error() {}
		virtual std::string to_string() const = 0;
	};

	class lex_error : compile_error {
		int last_char;
	public:
		lex_error(int row, int column, int last_char) :
			compile_error(row, column),
			last_char(last_char)
		{

		}
		virtual std::string to_string() const {
			return std::format("lex error: [{}, {}] unknown char: ({}){} \n", row, column, last_char, (char)last_char);
		}
	};

	class parse_error : compile_error {
	public:
		parse_error(int row, int column) :
			compile_error(row, column)
		{

		}
		virtual std::string to_string() const {
			return std::format("parse error: [{}, {}]", row, column);
		}
	};
}