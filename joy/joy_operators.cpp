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

    void pow(joy_stack& S) {
        double base{ 0 }, exponent{ 0 };
        token_t H;
        auto G = S.top();
        S.pop();
        auto F = S.top();
        S.pop();
        switch (G.second) {
        case joy_t::char_t: 
            exponent = joy_cast<char>(G);
            break;
        case joy_t::int_t: 
            exponent = joy_cast<int>(G);
            break;
        case joy_t::double_t: 
            exponent = joy_cast<double>(G);
            break;
        default:
            break;
        }
        switch (F.second) {
        case joy_t::char_t: 
            base = joy_cast<char>(F);
            break;
        case joy_t::int_t: 
            base = joy_cast<int>(F);
            break;
        case joy_t::double_t: 
            base = joy_cast<double>(F);
            break;
        default:
            break;
        }
        auto result = std::pow(base, exponent);
        switch (F.second) {
        case joy_t::char_t:
            H.first = static_cast<char>(result);
            H.second = joy_t::char_t;
            break;
        case joy_t::int_t:
            H.first = static_cast<int>(result);
            H.second = joy_t::int_t;
            break;
        case joy_t::double_t:
            H.first = static_cast<double>(result);
            H.second = joy_t::double_t;
            break;
        default:
            break;
        }
        S.push(H);
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

	void putch(joy_stack& S, io_device& io) {
        auto ch = static_cast<char>(joy_cast<char>(S.top()));
        auto s = std::string{ ch };
        io.put_string(s);
    }

    void print_top(const joy_stack& S, io_device& io) {
        io.colour(GREEN);
        io << to_string(S.top());
    }

    void print_stack(const joy_stack& S, io_device& io) {
        io.colour(GREEN);
        std::string dump{ ">" + std::to_string(S.size()) + "<\n" };
        for (size_t i{ 0 }; i < S.size(); ++i) {
            const auto& t = S.sat(i);
            dump += to_string(t) + "\n";
        }
        io << dump;
    }

	void input_stack(joy_stack& S, io_device& io) {
        io.colour(BOLDYELLOW);
        io.put_string("? ");
        S.push(make_token(io.input(), joy_t::undef_t));
    }

    void manual(std::map<std::string, std::string>& joy_manual, io_device& io) {
        io.colour(YELLOW);
        for (const auto& [cmd, info] : joy_manual) {
            io << cmd << info;
        }
    }

    void helpdetail(const joy_stack& S, std::map<std::string, std::string>& joy_manual, io_device& io) {
        io.colour(YELLOW);
        for (const auto& [command, type] : S) {
            assert(type == joy_t::lambda_t);
            auto match = std::any_cast<std::string>(command);
            auto it = joy_manual.find(match);
            if (it != joy_manual.end()) {
                io << match + joy_manual[match];
            }
        }

    }

}
