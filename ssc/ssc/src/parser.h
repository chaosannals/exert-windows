#pragma once

#include "lexer.h"
#include "scope.h"
#include <stack>

namespace ssc {
	class parser {
        lexer& lex;
        scope global_scope;
        std::stack<scope> local_scopes;
	public:
        parser(lexer& lex) : lex(lex) {}

        void parse();
    private:
        bool match_struct();
        bool match_struct_field();
	};
}
