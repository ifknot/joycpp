#pragma once

#include <any>
#include <vector>
#include <functional>
#include <map>
#include <stdexcept>
#include <regex>
#include <cassert>
#include <iostream>

#include "colour_codes.h"

/**
* access std::literals::string_literals::operator""s
* to ensure correct std::string type in make_token
*/
using namespace std::string_literals;

/**
* Joy is a functional programming language which is not based on the application of functions to arguments but on the composition of functions.
* From a theoretical perspective the denotation of Joy programs maps a syntactic monoid of program concatenation to a semantic monoid of function composition.
* Instead of lambda abstraction Joy uses program quotation - quoted programs or quotes.
* A quoted program is an aggregate of any joy type - including quoted programs - and any joy command
* Higher order functions can be simulated by first order functions which unquote quoted programs.
* In Joy the quotation of a program is written by enclosing it in square brackets.
* Ultimately all programs are built from atomic programs which are not divisible into parts - at least at the Joy language level.
*/
namespace joy {    

    /**
    * Joy is a typed language, there are 2 main groups:
    * - simple types: boolean, char, integer, double
    * - aggregate types: quote, list, set, string
    * Where a quotation contains only literals, then it is a value of the list type.
    * NB the component literals of a list do not have to be of the same type, and they may include further quotations
    * It is sometimes useful to treat several types together.
    * The numeric types are integers, characters and double, the Boolean types are truth values and sets, and the sequence types are strings and lists.
    * A leaf is anything which is not a list
    * - numeric  (int, char, double)
    * - aggregate (list, quote, set, string)
    * - cmd_t   (any Joy command)
    *
    * joycpp types in order of increasing specificity:
    */
    enum class joy_t { 
        //grouped types
        undef_t,        // an undefined lexeme
        any_t,          // any joy type
        numeric_t,      // int_t, char_t double_t
        group_t,        // quote_t, list_t
        sequence_t,     // quote, list, string
        aggregate_t,    // quote, list, string & set
        nullable_t,     // mumeric_t aggregate_t
        lambda_t,       // joy-lambda command 
        cmd_t,          // joy-joy command
        //aggregate types NB all implemented as a joy_stack
        quote_t,        // aggregate of any joy type and any joy command
        list_t,         // quotation of only literal types
        set_t,          // aggregate of unique unordered integer type 
        string_t,       // ordered sequence of zero or more characters
        //simple types
        bool_t, char_t, int_t, double_t,
    };

    typedef std::any pattern_t;
    typedef std::pair<pattern_t, joy_t> token_t;

    /**
    * Convert a joy type to its string name (boolean, int, char, double, list, quote, set, string)
    */
    std::string to_string(const joy_t match);

    /**
    * Constructs a token object from a type T
    * @note ensure std::string literals are specified using the ""s operator
    * e.g. make_token("genrec"s, joy_t::cmd_t));
    */
    template<typename T>
    inline token_t make_token(T arg, joy_t type) {
        return std::make_pair(arg, type);
    }

    /**
    * cast a token pair's first to a specific type
    */
    template<typename T>
    T joy_cast(token_t& token) {
        try {
            return std::any_cast<T>(token.first);
        }
        catch (const std::bad_any_cast & e) {
            std::cerr << BOLDRED
                << "joy_cast - the typeid of the requested type T does not match the token lexeme type_id!\n"
                << "(possible std::literals::string_literals::operator\"\"s omission?)"
                << RESET;
            throw e;
        }
    }

    /**
    * cast a token pair's first to a specific type
    */
    template<typename T>
    T joy_cast(const token_t& token) {
        try {
            return std::any_cast<T>(token.first);
        }
        catch (const std::bad_any_cast & e) {
            std::cerr << BOLDRED
                << "joy_cast - the typeid of the requested T does not match the token lexeme!\n";
        }
    }

    /**
    * in place conversion bool, char, int, double & special chars '\ '\111
    */
    void convert_simple(token_t& token);

    //string joy primitive matching:

    inline bool jlogical(const std::string& match) {
        return ((match == "true") || (match == "false"));
    }

    inline bool jdouble(const std::string& match) {
        //prevent stod from parsing the joy combinator infra as infinity and daft things like 3] 4} as doubles
        if (std::regex_match(match, std::regex("[-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?"))) {
            try {
                auto x = std::stod(match);
                return true;
            }
            catch (std::invalid_argument) {
                return false;
            }
            catch (std::out_of_range) {
                return false;
            }
        }
        else {
            return false;
        }
    }

    inline bool jdouble(const std::string&& match) {
        //prevent stod from parsing the joy combinator infra as infinity and daft things like 3] 4} as doubles
        if (std::regex_match(match, std::regex("[-+]?[0-9]*\\.?[0-9]+([eE][-+]?[0-9]+)?"))) {
            try {
                auto x = std::stod(match);
                return true;
            }
            catch (std::invalid_argument) {
                return false;
            }
            catch (std::out_of_range) {
                return false;
            }
        }
        else {
            return false;
        }
    }

    inline bool jinteger(const std::string& match) {
        if (jdouble(match)) {
            return match.find_first_not_of("+-0123456789") == std::string::npos;
        }
        else {
            return false;
        }
    }

    inline bool jinteger(const std::string&& match) {
        if (jdouble(match)) {
            return match.find_first_not_of("+-0123456789") == std::string::npos;
        }
        else {
            return false;
        }
    }

    inline bool jchar(const std::string& match) {
        return ((match.size() == 2) && (match[0] == '\''));
    }

    //char special case space
    inline bool jchar_space(const std::string& match) {
        return ((match.size() == 1) && (match[0] == '\''));
    }

    //char escape
    inline bool jchar_escape(const std::string& match) {
        return ((match.size() == 3) && (match[0] == '\'') && (match[1] == '\\'));
    }

    //char decimal
    inline bool jchar_decimal (const std::string& match) {
        return ((match.size() == 5) && (match[0] == '\'') && (match[1] == '\\'));
    }

    //token joy type matching

    inline bool jlogical(const token_t& token) {
        return token.second == joy_t::bool_t;
    }

    inline bool jchar(const token_t& token) {
        return token.second == joy_t::char_t;
    }

    inline bool jdouble(const token_t& token) {
        return token.second == joy_t::double_t;
    }

    inline bool jinteger(const token_t& token) {
        return token.second == joy_t::int_t;
    }

    inline bool jnumeric(const token_t& token) {
        return jchar(token) || jdouble(token) || jinteger(token);
    }

    inline bool jset(const token_t& token) {
        return token.second == joy_t::set_t;
    }

    inline bool jstring(const token_t& token) {
        return token.second == joy_t::string_t;
    }

    inline bool jlist(const token_t& token) {
        return token.second == joy_t::list_t;
    }

    inline bool jquote(const token_t& token) {
        return token.second == joy_t::quote_t;
    }

    inline bool jundef(const token_t& token) {
        return token.second == joy_t::undef_t;
    }

    inline bool jcmd(const token_t& token) {
        return token.second == joy_t::lambda_t;
    }

    inline bool jgroup(const token_t& token) {
        return jlist(token) || jquote(token);
    }

    inline bool jsequence(const token_t& token) {
        return jlist(token) || jquote(token) || jstring(token);
    }

    inline bool jaggregate(const token_t& token) {
        return jsequence(token) || jset(token);
    }

    inline bool jnullable(const token_t& token) {
        return jaggregate(token) || jnumeric(token);
    }

    inline bool jleaf(const token_t& token) {
        return !jaggregate(token);
    }

}
