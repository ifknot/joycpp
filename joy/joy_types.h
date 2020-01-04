/**
* Joy is a typed language, there are 2 main groups:
* - simple types: boolean, char, integer, double
* - aggregate types: quote, list, set, string
*  Where a quotation contains only literals, then it is a value of the list type.
* NB the component literals of a list do not have to be of the same type, and they may include further quotations
* It is sometimes useful to treat several types together. 
* The numeric types are integers, characters and double, the Boolean types are truth values and sets, and the sequence types are strings and lists. 
* A leaf is anything which is not a list
* - numeric  (int, char, double)
* - aggregate (list, quote, set, string)
* - cmd_t   (any Joy command)
*/
#pragma once

#include <any>
#include <vector>
#include <functional>
#include <map>

namespace joy {

    /**
    * joycpp types in order of increasing precision
    */
    enum class joy_t { 
        any_t, numeric_t,                   //grouped types
        quote_t, list_t, set_t, string_t,   //aggregate types  
        bool_t, char_t, int_t, double_t     //simple types
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
