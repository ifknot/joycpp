#include "joy_operators_io.h"

namespace joy {

    void putch(joy_stack& S, io_device& io) {
        auto ch = static_cast<char>(joy_cast<char>(S.top()));
        auto s = std::string{ ch };
        io.put_string(s);
    }

	void format(joy_stack& S) {
        char  buffer[100];
        std::string control{ "%" };
        joy_stack M;
        auto min_width = joy_cast<int>(S.top());
        S.pop();
        auto max_width = joy_cast<int>(S.top());
        S.pop();
        auto mode = joy_cast<char>(S.top());
        control += mode;
        S.pop();
        switch (S.top().second) {
        case joy_t::char_t: {
            auto c = joy_cast<char>(S.top());
            S.pop();
            if (sprintf_s(buffer, 100, control.c_str(), c, min_width, max_width) == -1) {
                strerror_s(buffer, errno);
            }
            for (auto ch : buffer) {
                if (ch == 0) break;
                M.push(make_token(ch, joy_t::char_t));
            }
            S.push(make_token(M, joy_t::string_t));
            break;
        }
        case joy_t::int_t: {
            auto i = joy_cast<int>(S.top());
            S.pop();
            if (sprintf_s(buffer, 100, control.c_str(), i, min_width, max_width) == -1) {
                strerror_s(buffer, errno);
            }
            for (auto ch : buffer) {
                if (ch == 0) break;
                M.push(make_token(ch, joy_t::char_t));
            }
            S.push(make_token(M, joy_t::string_t));
            break;
        }
        case joy_t::double_t: {
            auto d = joy_cast<double>(S.top());
            S.pop();
            if (sprintf_s(buffer, 100, control.c_str(), d, min_width, max_width) == -1) {
                strerror_s(buffer, errno);
            }
            for (auto ch : buffer) {
                if (ch == 0) break;
                M.push(make_token(ch, joy_t::char_t));
            }
            S.push(make_token(M, joy_t::string_t));
            break;
        }
        default:
            break;
        }
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
