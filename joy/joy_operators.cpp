#include "joy_operators.h"

namespace joy {

    void reverse(joy_stack& stack) {
        auto& A = std::any_cast<joy_stack&>(stack.top().first);
        std::reverse(A.begin(), A.end());
    }

    void size(joy_stack& S) {
        if (S.has("size", { joy_t::aggregate_t })) {
            S.push(make_token(size(S.top()), joy_t::int_t));
        }
    }

    void print_top(const joy_stack& S, io_device& io) {
        io.colour(GREEN);
        io << joy_stack::to_string(S.top());
    }

    void print_stack(const joy_stack& S, io_device& io) {
        io.colour(GREEN);
        std::string dump{ ">" + std::to_string(S.size()) + "<\n" };
        for (size_t i{ 0 }; i < S.size(); ++i) {
            const auto& t = S.sat(i);
            dump += joy_stack::to_string(t) + "\n";
        }
        io << dump;
    }

	void input_stack(joy_stack& S, io_device& io) {
        io.putch('?');
        io.putch(' ');
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
        for (const auto [command, type] : S) {
            assert(type == joy_t::lambda_t);
            auto match = std::any_cast<std::string>(command);
            auto it = joy_manual.find(match);
            if (it != joy_manual.end()) {
                io << match + joy_manual[match];
            }
        }

    }

}
