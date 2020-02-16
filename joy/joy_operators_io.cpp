#include "joy_operators_io.h"

namespace joy {

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

}
