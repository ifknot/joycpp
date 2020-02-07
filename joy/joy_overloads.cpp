#include "joy_overloads.h"

/**
* preprocessor macros are not ideal but on occasion are the only solution
*/
#define BIN_RELATION(op) \
        switch (b.second) { \
        case joy::joy_t::char_t: \
            return make_token((c op joy_cast<char>(b)), joy_t::bool_t); \
        case joy::joy_t::int_t: \
            return make_token((c op joy_cast<int>(b)), joy_t::bool_t); \
        case joy::joy_t::double_t: \
            return make_token((c op joy_cast<double>(b)), joy_t::bool_t); \
        default: \
            return make_token("", joy_t::undef_t); \
            break; \
        } 

#define BIN_OP(op, type) \
    switch (b.second) { \
    case joy::joy_t::char_t: \
        c op joy_cast<char>(b); \
        return make_token(c, type); \
    case joy::joy_t::int_t: \
        c op joy_cast<int>(b); \
        return make_token(c, type); \
    case joy::joy_t::double_t: \
        c op joy_cast<double>(b); \
        return make_token(c, type); \
    default: \
        return make_token("", joy_t::undef_t); \
        break; \
}

namespace joy {
   
    token_t ord(token_t token) {
        return token_t(static_cast<int>(joy_cast<char>(token)), joy_t::int_t);
    }

    token_t chr(token_t token) {
        return token_t(static_cast<char>(joy_cast<int>(token)), joy_t::char_t);
    }

    token_t operator + (const token_t& a, const token_t& b) {
        switch (a.second) { //switch through all of the Joy03 numeric types for the first argument
        case joy::joy_t::char_t: {
            auto c = joy_cast<char>(a); //cast the first argument
            BIN_OP(+=, joy_t::char_t)
        } //and so on...
        case joy::joy_t::int_t: {
            auto c = joy_cast<int>(a);
            BIN_OP(+=, joy_t::int_t)
        }
        case joy::joy_t::double_t: {
            auto c = joy_cast<double_t>(a);
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
            auto c = joy_cast<char>(a);
            BIN_OP(-=, joy_t::char_t)
        }
        case joy::joy_t::int_t: {
            auto c = joy_cast<int>(a);
            BIN_OP(-=, joy_t::int_t)
        }
        case joy::joy_t::double_t: {
            auto c = joy_cast<double_t>(a);
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
            auto c = joy_cast<char>(a);
            BIN_OP(*=, joy_t::char_t)
        }
        case joy::joy_t::int_t: {
            auto c = joy_cast<int>(a);
            BIN_OP(*=, joy_t::int_t)
        }
        case joy::joy_t::double_t: {
            auto c = joy_cast<double_t>(a);
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
            auto c = joy_cast<char>(a);
            BIN_OP(/=, joy_t::char_t)
        }
        case joy::joy_t::int_t: {
            auto c = joy_cast<int>(a);
            BIN_OP(/=, joy_t::int_t)
        }
        case joy::joy_t::double_t: {
            auto c = joy_cast<double_t>(a);
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
            auto c = joy_cast<char>(a);
            BIN_RELATION(<)
        }
        case joy::joy_t::int_t: {
            auto c = joy_cast<int>(a);
            BIN_RELATION(<)
        }
        case joy::joy_t::double_t: {
            auto c = joy_cast<double_t>(a);
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
            auto c = joy_cast<char>(a);
            BIN_RELATION(<=)
        }
        case joy::joy_t::int_t: {
            auto c = joy_cast<int>(a);
            BIN_RELATION(<=)
        }
        case joy::joy_t::double_t: {
            auto c = joy_cast<double_t>(a);
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
            auto c = joy_cast<char>(a);
            BIN_RELATION(>)
        }
        case joy::joy_t::int_t: {
            auto c = joy_cast<int>(a);
            BIN_RELATION(>)
        }
        case joy::joy_t::double_t: {
            auto c = joy_cast<double_t>(a);
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
            auto c = joy_cast<char>(a);
            BIN_RELATION(>=)
        }
        case joy::joy_t::int_t: {
            auto c = joy_cast<int>(a);
            BIN_RELATION(>=)
        }
        case joy::joy_t::double_t: {
            auto c = joy_cast<double_t>(a);
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
            auto c = joy_cast<char>(a);
            BIN_RELATION(!=)
        }
        case joy::joy_t::int_t: {
            auto c = joy_cast<int>(a);
            BIN_RELATION(!=)
        }
        case joy::joy_t::double_t: {
            auto c = joy_cast<double_t>(a);
            BIN_RELATION(!=)
        }
        default:
            return make_token("", joy_t::undef_t);
            break;
        }
    }

	token_t operator == (const token_t& a, const token_t& b) {
        switch (a.second) {
        case joy_t::bool_t: {
            auto c = joy_cast<bool>(a);
            switch (b.second) {
            case joy_t::bool_t:
                return make_token((c == joy_cast<bool>(b)), joy_t::bool_t);
            default:
                return make_token("", joy_t::undef_t);
                break;
            }
        }
        case joy::joy_t::char_t: {
            auto c = joy_cast<char>(a);
            BIN_RELATION(==)
        }
        case joy::joy_t::int_t: {
            auto c = joy_cast<int>(a);
            BIN_RELATION(==)
        }
        case joy::joy_t::double_t: {
            auto c = joy_cast<double_t>(a);
            BIN_RELATION(==)
        }
        default:
            return make_token("", joy_t::undef_t);
            break;
        }
	}

	bool operator == (const token_t& a, const std::string& b) {
        if (a.second == joy_t::lambda_t || a.second == joy_t::undef_t || a.second == joy_t::cmd_t ) {
            return joy_cast<std::string>(a) == b;
        }
        else {
            return false;
        }
	}

}