#include "joy_overloads.h"

/**
* preprocessor macros are not ideal but on occasion are the only solution
*/
#define BIN_RELATION(op) \
        switch (b.second) { \
        case joy::joy_t::char_t: \
            return make_token((c op std::any_cast<char>(b.first)) ? true : false, joy_t::bool_t); \
        case joy::joy_t::int_t: \
            return make_token((c op std::any_cast<int>(b.first)) ? true : false, joy_t::bool_t); \
        case joy::joy_t::double_t: \
            return make_token((c op std::any_cast<double>(b.first)) ? true : false, joy_t::bool_t); \
        default: \
            return make_token("", joy_t::undef_t); \
            break; \
        } 

#define BIN_OP(op, type) \
    switch (b.second) { \
    case joy::joy_t::char_t: \
        c op std::any_cast<char>(b.first); \
        return make_token(c, type); \
    case joy::joy_t::int_t: \
        c op std::any_cast<int>(b.first); \
        return make_token(c, type); \
    case joy::joy_t::double_t: \
        c op std::any_cast<double>(b.first); \
        return make_token(c, type); \
    default: \
        return make_token("", joy_t::undef_t); \
        break; \
}

namespace joy {
   
    token_t ord(token_t token) {
        return token_t(static_cast<int>(std::any_cast<char>(token.first)), joy_t::int_t);
    }

    token_t chr(token_t token) {
        return token_t(static_cast<char>(std::any_cast<int>(token.first)), joy_t::char_t);
    }

    token_t operator + (const token_t& a, const token_t& b) {
        switch (a.second) { //switch through all of the Joy03 numeric types for the first argument
        case joy::joy_t::char_t: {
            auto c = std::any_cast<char>(a.first); //cast the first argument
            BIN_OP(+=, joy_t::char_t)
        } //and so on...
        case joy::joy_t::int_t: {
            auto c = std::any_cast<int>(a.first);
            BIN_OP(+=, joy_t::int_t)
        }
        case joy::joy_t::double_t: {
            auto c = std::any_cast<double_t>(a.first);
            BIN_OP(+=, joy_t::double_t)
        }
        default:
            return make_token("", joy_t::undef_t);
            break;
        }
    }

    token_t operator - (const token_t& a, const token_t& b) {
        switch (a.second) {
        case joy::joy_t::char_t: {
            auto c = std::any_cast<char>(a.first);
            BIN_OP(-=, joy_t::char_t)
        }
        case joy::joy_t::int_t: {
            auto c = std::any_cast<int>(a.first);
            BIN_OP(-=, joy_t::int_t)
        }
        case joy::joy_t::double_t: {
            auto c = std::any_cast<double_t>(a.first);
            BIN_OP(-=, joy_t::double_t)
        }
        default:
            return make_token("", joy_t::undef_t);
            break;
        }
    }

    token_t operator * (const token_t& a, const token_t& b) {
        switch (a.second) {
        case joy::joy_t::char_t: {
            auto c = std::any_cast<char>(a.first);
            BIN_OP(*=, joy_t::char_t)
        }
        case joy::joy_t::int_t: {
            auto c = std::any_cast<int>(a.first);
            BIN_OP(*=, joy_t::int_t)
        }
        case joy::joy_t::double_t: {
            auto c = std::any_cast<double_t>(a.first);
            BIN_OP(*=, joy_t::double_t)
        }
        default:
            return make_token("", joy_t::undef_t);
            break;
        }
    }

    token_t operator / (const token_t& a, const token_t& b) {
        switch (a.second) {
        case joy::joy_t::char_t: {
            auto c = std::any_cast<char>(a.first);
            BIN_OP(/=, joy_t::char_t)
        }
        case joy::joy_t::int_t: {
            auto c = std::any_cast<int>(a.first);
            BIN_OP(/=, joy_t::int_t)
        }
        case joy::joy_t::double_t: {
            auto c = std::any_cast<double_t>(a.first);
            BIN_OP(/=, joy_t::double_t)
        }
        default:
            return make_token("", joy_t::undef_t);
            break;
        }
    }

    token_t operator < (const token_t& a, const const token_t& b) {
        switch (a.second) {
        case joy::joy_t::char_t: {
            auto c = std::any_cast<char>(a.first);
            BIN_RELATION(<)
        }
        case joy::joy_t::int_t: {
            auto c = std::any_cast<int>(a.first);
            BIN_RELATION(<)
        }
        case joy::joy_t::double_t: {
            auto c = std::any_cast<double_t>(a.first);
            BIN_RELATION(<)
        }
        default:
            return make_token("", joy_t::undef_t);
            break;
        }
    }

    token_t operator <= (const token_t& a, const token_t& b) {
        switch (a.second) {
        case joy::joy_t::char_t: {
            auto c = std::any_cast<char>(a.first);
            BIN_RELATION(<=)
        }
        case joy::joy_t::int_t: {
            auto c = std::any_cast<int>(a.first);
            BIN_RELATION(<=)
        }
        case joy::joy_t::double_t: {
            auto c = std::any_cast<double_t>(a.first);
            BIN_RELATION(<=)
        }
        default:
            return make_token("", joy_t::undef_t);
            break;
        }
    }

    token_t operator > (const token_t& a, const token_t& b) {
        switch (a.second) {
        case joy::joy_t::char_t: {
            auto c = std::any_cast<char>(a.first);
            BIN_RELATION(>)
        }
        case joy::joy_t::int_t: {
            auto c = std::any_cast<int>(a.first);
            BIN_RELATION(>)
        }
        case joy::joy_t::double_t: {
            auto c = std::any_cast<double_t>(a.first);
            BIN_RELATION(>)
        }
        default:
            return make_token("", joy_t::undef_t);
            break;
        }
    }

    token_t operator >= (const token_t& a, const token_t& b) {
        switch (a.second) {
        case joy::joy_t::char_t: {
            auto c = std::any_cast<char>(a.first);
            BIN_RELATION(>=)
        }
        case joy::joy_t::int_t: {
            auto c = std::any_cast<int>(a.first);
            BIN_RELATION(>=)
        }
        case joy::joy_t::double_t: {
            auto c = std::any_cast<double_t>(a.first);
            BIN_RELATION(>=)
        }
        default:
            return make_token("", joy_t::undef_t);
            break;
        }
    }

    token_t operator != (const token_t& a, const token_t& b) {
        switch (a.second) {
        case joy::joy_t::char_t: {
            auto c = std::any_cast<char>(a.first);
            BIN_RELATION(!=)
        }
        case joy::joy_t::int_t: {
            auto c = std::any_cast<int>(a.first);
            BIN_RELATION(!=)
        }
        case joy::joy_t::double_t: {
            auto c = std::any_cast<double_t>(a.first);
            BIN_RELATION(!=)
        }
        default:
            return make_token("", joy_t::undef_t);
            break;
        }
    }

	token_t operator == (const token_t& a, const token_t& b) {
        switch (a.second) {
        case joy::joy_t::char_t: {
            auto c = std::any_cast<char>(a.first);
            BIN_RELATION(==)
        }
        case joy::joy_t::int_t: {
            auto c = std::any_cast<int>(a.first);
            BIN_RELATION(==)
        }
        case joy::joy_t::double_t: {
            auto c = std::any_cast<double_t>(a.first);
            BIN_RELATION(==)
        }
        default:
            return make_token("", joy_t::undef_t);
            break;
        }
	}

	bool operator == (const token_t& a, const std::string& b) {
        if (a.second == joy_t::lambda_t || a.second == joy_t::undef_t || a.second == joy_t::cmd_t ) {
            return std::any_cast<std::string>(a.first) == b;
        }
        else {
            return false;
        }
	}

}