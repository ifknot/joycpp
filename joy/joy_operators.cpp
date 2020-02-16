#include "joy_operators.h"

namespace joy {

    void cons(joy_stack& S) {
        S.swap();
        auto X = S.top();
        S.pop();
        auto& A = joy_cast<joy_stack&>(S.top());
        A.insert(A.begin(), X);
    }

    void reverse(joy_stack& stack) {
        auto& A = joy_cast<joy_stack&>(stack.top());
        std::reverse(A.begin(), A.end());
    }

    void size(joy_stack& S) {
        if (S.has("size", { joy_t::aggregate_t })) {
            S.push(make_token(size(S.top()), joy_t::int_t));
        }
    }

    void small(joy_stack& S) {
        const auto [lexeme, type] = S.top();
        switch (type) {
        case joy_t::char_t: 
            S.push(token_t{ std::any_cast<char>(lexeme) == 0 || std::any_cast<char>(lexeme) == 1, joy_t::bool_t });
            break;
        case joy_t::int_t:
            S.push(token_t{ std::any_cast<int>(lexeme) == 0 || std::any_cast<int>(lexeme) == 1, joy_t::bool_t });
            break;
        case joy_t::double_t:
            S.push(token_t{ std::any_cast<double>(lexeme) == 0 || std::any_cast<double>(lexeme) == 1, joy_t::bool_t });
            break;
        case joy_t::quote_t:
        case joy_t::list_t:
            S.push(token_t{ std::any_cast<joy_stack>(lexeme).size() == 0 || std::any_cast<joy_stack>(lexeme).size() == 1, joy_t::bool_t });
            break;
        default:
            break;
        }
    }

    bool null(token_t& token) {
        const auto& [pattern, type] = token;
        switch (type) {
        case joy_t::char_t:
            return std::any_cast<char>(pattern) == 0;
        case joy_t::int_t:
            return std::any_cast<int>(pattern) == 0;
        case joy_t::double_t:
            return std::any_cast<double>(pattern) == 0;
        case joy_t::list_t:
            return std::any_cast<joy_stack>(pattern).size() == 0;
        case joy_t::quote_t:
            return std::any_cast<joy_stack>(pattern).size() == 0;
        default:
            return false;
            break;
        }
	}

}
