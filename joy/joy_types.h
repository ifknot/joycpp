/**
* Joy is a functional programming language which is not based on the application of functions to arguments but on the composition of functions. 
* From a theoretical perspective the denotation of Joy programs maps a syntactic monoid of program concatenation to a semantic monoid of function composition.
* Instead of lambda abstraction Joy uses program quotation - quoted programs or quotes.
* A quoted program is an aggregate of any joy type - including quoted programs - and any joy command
* Higher order functions can be simulated by first order functions which dequote quoted programs.
* In Joy the quotation of a program is written by enclosing it in square brackets. 
* Ultimately all programs are built from atomic programs which are not divisible into parts - at least at the Joy language level.
*/
#pragma once

#include <any>
#include <vector>
#include <functional>
#include <map>

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
    * joycpp types in order of increasing precision:
    */
    enum class joy_t { 
        //grouped types
        undef_t,        // an undefined lexeme
        any_t,          // any joy type
        numeric_t,      // int_t, char_t double_t
        cmd_t,          // recognised joy command  
        //aggregate types  
        quote_t,        // aggregate of any joy type and any joy command
        list_t,         // quotation of only literal types
        set_t,          // aggregate of unique unordered integer type 
        string_t,       // ordered sequence of zero or more characters
        //simple types
        bool_t, char_t, int_t, double_t    
    };

    /**
    * joycpp context free parser states
    */
    enum class state_t { parse, quote, list, set, string };

    typedef std::any pattern_t;
    typedef std::pair<pattern_t, joy_t> token_t;
    typedef std::vector<token_t> token_list_t;
    typedef std::function<void()> function_t;
    typedef std::map<pattern_t, function_t> dictionary_t;

}
