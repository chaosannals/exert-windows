#include <unordered_map>
#include "lexer.h"

std::unordered_map<std::string, ssc::token> keywords = {
	{ "true", ssc::token::kw_true, },
	{ "false", ssc::token::kw_false, },
	{ "i8", ssc::token::kw_i8, },
	{ "i16", ssc::token::kw_i16,},
	{ "i32", ssc::token::kw_i32,},
	{ "i64", ssc::token::kw_i64,},
	{ "u8", ssc::token::kw_u8,},
	{ "u16", ssc::token::kw_u16,},
	{ "u32", ssc::token::kw_u32,},
	{ "u64", ssc::token::kw_u64,},
	{ "f32", ssc::token::kw_f32,},
	{ "f64", ssc::token::kw_f64,},
	{ "struct", ssc::token::kw_struct,},
	{ "if", ssc::token::kw_if,},
	{ "else", ssc::token::kw_else,},
	{ "switch", ssc::token::kw_switch,},
	{ "case", ssc::token::kw_case,},
	{ "break", ssc::token::kw_break,},
	{ "for", ssc::token::kw_for,},
	{ "continue", ssc::token::kw_continue,},
	{ "until", ssc::token::kw_until,},
	{ "return", ssc::token::kw_return,},
};

void ssc::lexer::next_char() {
	int c = stream.get();
	if (c == '\r') {
		int p = stream.peek();
		if (p == '\n') {
			c = stream.get();
		}
	}
	if (c == '\r' || c == '\n') {
		++row;
		column = 0;
	}
	else {
		++column;
	}
	last_char = c;
}

void ssc::lexer::skip_char() {
	while (std::isspace(last_char)) {
		next_char();
	}
}

ssc::lexeme ssc::lexer::new_lexeme(
	ssc::token token,
	const std::string& text
)
{
	return lexeme(
		row,
		column,
		token,
		text
	);
}

ssc::lexeme ssc::lexer::pop_lexeme() {
	skip_char();
	if (last_char == EOF) {
		return new_lexeme(ssc::token::end_of_file, "[EOF]");
	}
	if (std::isdigit(last_char)) {
		return pop_number();
	}
	if (last_char == '"') {
		return pop_string();
	}
	if (std::ispunct(last_char)) {
		return pop_punct();
	}
	if (std::isalpha(last_char)) {
		return pop_identifier();
	}
	throw lex_error(row, column, last_char);
}

ssc::lexeme ssc::lexer::pop_punct() {
	lexeme r = new_lexeme(token::end_of_file, "[UNKNOWN]");
	int c = last_char;
	next_char();
	switch (c) {
	case '+':
		if (last_char == '+') {
			next_char();
			r.token = token::tk_plus_plus;
			r.text = "++";
		}
		else {
			r.token = token::tk_plus;
			r.text = "+";
		}
		return r;
	case '-':
		if (last_char == '-') {
			next_char();
			r.token = token::tk_minus_minus;
			r.text = "--";
		}
		else if (last_char == '>') {
			next_char();
			r.token = token::tk_arrow;
			r.text = "->";
		}
		else {
			r.token = token::tk_minus;
			r.text = "-";
		}
		return r;
	case '=':
		r.token = token::tk_equal;
		r.text = "=";
		return r;
	case '<':
		if (last_char == '=') {
			next_char();
			r.token = token::tk_less_equal;
			r.text = "<=";
		}
		else {
			r.token = token::tk_less;
			r.text = "<";
		}
		return r;
	case '>':
		if (last_char == '=') {
			next_char();
			r.token = token::tk_greater_equal;
			r.text = ">=";
		}
		else {
			r.token = token::tk_greater;
			r.text = ">";
		}
		return r;
	case '(':
		r.token = token::tk_parenthese_left;
		r.text = "(";
		return r;
	case ')':
		r.token = token::tk_parenthese_right;
		r.text = ")";
		return r;
	case '{':
		r.token = token::tk_brace_left;
		r.text = "{";
		return r;
	case '}':
		r.token = token::tk_brace_right;
		r.text = "}";
		return r;
	case ';':
		r.token = token::tk_semicolon;
		r.text = ";";
		return r;
	case '.':
		r.token = token::tk_dot;
		r.text = ".";
		return r;
	case ',':
		r.token = token::tk_comma;
		r.text = ",";
		return r;
	}
	throw lex_error(row, column, last_char);
}

ssc::lexeme ssc::lexer::pop_number() {
	std::stringstream r;
	while (std::isdigit(last_char)) {
		r << (char)last_char;
		next_char();
	}
	bool is_float = false;
	if (last_char == '.') {
		is_float = true;
		r << (char)last_char;
		next_char();
		while (std::isdigit(last_char)) {
			r << (char)last_char;
			next_char();
		}
	}
	if (last_char == 'f') {
		r << (char)last_char;
		next_char();
		return new_lexeme(
			token::literal_f32,
			r.str()
		);
	}
	if (is_float) {
		return new_lexeme(
			token::literal_f64,
			r.str()
		);
	}
	return new_lexeme(
		token::literal_i32,
		r.str()
	);
}

ssc::lexeme ssc::lexer::pop_string() {
	std::stringstream r;
	next_char();
	while (last_char != '"') {
		if (last_char == '\\') {
			next_char();
			switch (last_char) {
			case '0':
				r << '\0';
				break;
			case 't':
				r << '\t';
				break;
			case 'n':
				r << '\n';
				break;
			case 'r':
				r << '\r';
				break;
			case '\\':
				r << '\\';
				break;
			case '"':
				r << '"';
				break;
			default:
				throw lex_error(row, column, last_char);
			}
		}
		else {
			r << (char)last_char;
		}
		next_char();
	}
	next_char();
	return new_lexeme(
		token::literal_string,
		r.str()
	);
}

ssc::lexeme ssc::lexer::pop_identifier() {
	std::stringstream t;
	lexeme r = new_lexeme(token::end_of_file, "[UNKNOWN]");
	do {
		t << (char)last_char;
		next_char();
	}
	while (std::isalnum(last_char));
	r.text = t.str();
	auto f = keywords.find(r.text);
	if (f != keywords.end()) {
		r.token = f->second;
	}
	else {
		r.token = ssc::token::identifier;
	}
	return r;
}