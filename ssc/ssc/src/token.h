#pragma once

namespace ssc {
	enum token {
		end_of_file = -1,
		kw_void = 0,
		kw_true,
		kw_false,
		kw_i8,
		kw_i16,
		kw_i32,
		kw_i64,
		kw_u8,
		kw_u16,
		kw_u32,
		kw_u64,
		kw_f32,
		kw_f64,
		kw_struct,
		kw_if,
		kw_else,
		kw_switch,
		kw_case,
		kw_break,
		kw_for,
		kw_continue,
		kw_until,
		kw_return,

		tk_plus,			// +
		tk_plus_plus,		// ++
		tk_minus,			// -
		tk_minus_minus,		// --
		tk_star,			// *
		tk_slash,			// /
		tk_ampersand,		// &
		tk_vertical,		// |
		tk_tilde,			// ~
		tk_equal,			// =
		tk_dot,				// .
		tk_comma,			// ,
		tk_colon,			// :
		tk_semicolon,		// ;
		tk_parenthese_left,	// (
		tk_parenthese_right,// )
		tk_brace_left,		// {
		tk_brace_right,		// }
		tk_bracket_left,	// [
		tk_bracket_right,	// ]
		tk_less,			// <
		tk_less_equal,		// <=
		tk_greater,			// >
		tk_greater_equal,	// >=
		tk_exclamation,		// !
		tk_and,				// &&
		tk_or,				// ||
		tk_arrow,			// ->

		identifier,			// 标识符
		literal_f32,		// 32位浮点字面量
		literal_f64,		// 64位浮点字面量
		literal_i32,		// 32位整数字面量
		literal_string,		// 字符串字面量
	};
}