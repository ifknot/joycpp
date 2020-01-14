#include "joy_primitives_minimal.h"

namespace joy {

    token_t operator+(const token_t& a, const token_t& b) {
        switch (a.second) { //switch through all of the Joy03 numeric types for the first arguement
        case joy::joy_t::char_t: {
            auto c = std::any_cast<char>(a.first); //cast the the first arguement
            switch (b.second) { //switch through all of the Joy03 numeric types for the second arguement
            case joy::joy_t::char_t: //cast the the second arguement and to the first
                c += std::any_cast<char>(b.first);
                return make_token(c, joy_t::char_t); //return new sum token
            case joy::joy_t::int_t: //ditto
                c += std::any_cast<int>(b.first);
                return make_token(c, joy_t::char_t);
            case joy::joy_t::double_t://ditto
                c += std::any_cast<double>(b.first);
                return make_token(c, joy_t::char_t);
            default:
                return make_token("", joy_t::undef_t);
                break;
            }
        } //and so on...
        case joy::joy_t::int_t: {
            auto c = std::any_cast<int>(a.first);
            switch (b.second) {
            case joy::joy_t::char_t:
                c += std::any_cast<char>(b.first);
                return make_token(c, joy_t::int_t);
            case joy::joy_t::int_t:
                c += std::any_cast<int>(b.first);
                return make_token(c, joy_t::int_t);
            case joy::joy_t::double_t:
                c += std::any_cast<double>(b.first);
                return make_token(c, joy_t::int_t);
            default:
                return make_token("", joy_t::undef_t);
                break;
            }
        }
        case joy::joy_t::double_t: {
            auto c = std::any_cast<double_t>(a.first);
            switch (b.second) {
            case joy::joy_t::char_t:
                c += std::any_cast<char>(b.first);
                return make_token(c, joy_t::double_t);
            case joy::joy_t::int_t:
                c += std::any_cast<int>(b.first);
                return make_token(c, joy_t::double_t);
            case joy::joy_t::double_t:
                c += std::any_cast<double>(b.first);
                return make_token(c, joy_t::double_t);
            default:
                return make_token("", joy_t::undef_t);
                break;
            }
        }
        default:
            return make_token("", joy_t::undef_t);
            break;
        }
    }

    token_t operator-(const token_t& a, const token_t& b) {
        switch (a.second) {
        case joy::joy_t::char_t: {
            auto c = std::any_cast<char>(a.first);
            switch (b.second) {
            case joy::joy_t::char_t:
                c -= std::any_cast<char>(b.first);
                return make_token(c, joy_t::char_t);
            case joy::joy_t::int_t:
                c -= std::any_cast<int>(b.first);
                return make_token(c, joy_t::char_t);
            case joy::joy_t::double_t:
                c -= std::any_cast<double>(b.first);
                return make_token(c, joy_t::char_t);
            default:
                return make_token("", joy_t::undef_t);
                break;
            }
        }
        case joy::joy_t::int_t: {
            auto c = std::any_cast<int>(a.first);
            switch (b.second) {
            case joy::joy_t::char_t:
                c -= std::any_cast<char>(b.first);
                return make_token(c, joy_t::int_t);
            case joy::joy_t::int_t:
                c -= std::any_cast<int>(b.first);
                return make_token(c, joy_t::int_t);
            case joy::joy_t::double_t:
                c -= std::any_cast<double>(b.first);
                return make_token(c, joy_t::int_t);
            default:
                return make_token("", joy_t::undef_t);
                break;
            }
        }
        case joy::joy_t::double_t: {
            auto c = std::any_cast<double_t>(a.first);
            switch (b.second) {
            case joy::joy_t::char_t:
                c -= std::any_cast<char>(b.first);
                return make_token(c, joy_t::double_t);
            case joy::joy_t::int_t:
                c -= std::any_cast<int>(b.first);
                return make_token(c, joy_t::double_t);
            case joy::joy_t::double_t:
                c -= std::any_cast<double>(b.first);
                return make_token(c, joy_t::double_t);
            default:
                return make_token("", joy_t::undef_t);
                break;
            }
        }
        default:
            return make_token("", joy_t::undef_t);
            break;
        }
    }

    token_t operator*(const token_t& a, const token_t& b) {
        switch (a.second) {
        case joy::joy_t::char_t: {
            auto c = std::any_cast<char>(a.first);
            switch (b.second) {
            case joy::joy_t::char_t:
                c *= std::any_cast<char>(b.first);
                return make_token(c, joy_t::char_t);
            case joy::joy_t::int_t:
                c *= std::any_cast<int>(b.first);
                return make_token(c, joy_t::char_t);
            case joy::joy_t::double_t:
                c *= std::any_cast<double>(b.first);
                return make_token(c, joy_t::char_t);
            default:
                return make_token("", joy_t::undef_t);
                break;
            }
        }
        case joy::joy_t::int_t: {
            auto c = std::any_cast<int>(a.first);
            switch (b.second) {
            case joy::joy_t::char_t:
                c *= std::any_cast<char>(b.first);
                return make_token(c, joy_t::int_t);
            case joy::joy_t::int_t:
                c *= std::any_cast<int>(b.first);
                return make_token(c, joy_t::int_t);
            case joy::joy_t::double_t:
                c *= std::any_cast<double>(b.first);
                return make_token(c, joy_t::int_t);
            default:
                return make_token("", joy_t::undef_t);
                break;
            }
        }
        case joy::joy_t::double_t: {
            auto c = std::any_cast<double_t>(a.first);
            switch (b.second) {
            case joy::joy_t::char_t:
                c *= std::any_cast<char>(b.first);
                return make_token(c, joy_t::double_t);
            case joy::joy_t::int_t:
                c *= std::any_cast<int>(b.first);
                return make_token(c, joy_t::double_t);
            case joy::joy_t::double_t:
                c *= std::any_cast<double>(b.first);
                return make_token(c, joy_t::double_t);
            default:
                return make_token("", joy_t::undef_t);
                break;
            }
        }
        default:
            return make_token("", joy_t::undef_t);
            break;
        }
    }

    token_t operator/(const token_t& a, const token_t& b) {
        switch (a.second) {
        case joy::joy_t::char_t: {
            auto c = std::any_cast<char>(a.first);
            switch (b.second) {
            case joy::joy_t::char_t:
                c /= std::any_cast<char>(b.first);
                return make_token(c, joy_t::char_t);
            case joy::joy_t::int_t:
                c /= std::any_cast<int>(b.first);
                return make_token(c, joy_t::char_t);
            case joy::joy_t::double_t:
                c /= std::any_cast<double>(b.first);
                return make_token(c, joy_t::char_t);
            default:
                return make_token("", joy_t::undef_t);
                break;
            }
        }
        case joy::joy_t::int_t: {
            auto c = std::any_cast<int>(a.first);
            switch (b.second) {
            case joy::joy_t::char_t:
                c /= std::any_cast<char>(b.first);
                return make_token(c, joy_t::int_t);
            case joy::joy_t::int_t:
                c /= std::any_cast<int>(b.first);
                return make_token(c, joy_t::int_t);
            case joy::joy_t::double_t:
                c /= std::any_cast<double>(b.first);
                return make_token(c, joy_t::int_t);
            default:
                return make_token("", joy_t::undef_t);
                break;
            }
        }
        case joy::joy_t::double_t: {
            auto c = std::any_cast<double_t>(a.first);
            switch (b.second) {
            case joy::joy_t::char_t:
                c /= std::any_cast<char>(b.first);
                return make_token(c, joy_t::double_t);
            case joy::joy_t::int_t:
                c /= std::any_cast<int>(b.first);
                return make_token(c, joy_t::double_t);
            case joy::joy_t::double_t:
                c /= std::any_cast<double>(b.first);
                return make_token(c, joy_t::double_t);
            default:
                return make_token("", joy_t::undef_t);
                break;
            }
        }
        default:
            return make_token("", joy_t::undef_t);
            break;
        }
    }

}
