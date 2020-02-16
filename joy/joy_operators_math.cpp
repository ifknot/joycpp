#include "joy_operators_math.h"

namespace joy {

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

}
