# joycpp
Joy Interpreter written in C++ and Joy (Please see the [Wiki](https://github.com/ifknot/joycpp/wiki))

### Joy Language 2003 core - joycpp implemention table:
### inilib.joy

|        |	|	|	|	|	|
|----------------	|---------------	|--------------	|---------------	|----------------	|-------------------	|
| [x] false      	| [x] rollupd   	| [x] cons     	| [x] =         	| [x] app2       	| [x] map           	|
| [x] true       	| [x] rolldownd 	| [x] swons    	| [ ] equal     	| [x] app3       	| [x] times         	|
| [ ] maxint     	| [x] rotated   	| [x] first    	| [ ] has       	| [ ] app4       	| [x] infra         	|
| [ ] setsize    	| [x] pop       	| [x] rest     	| [ ] in        	| [ ] binary     	| [ ] filter        	|
| [ ] conts      	| [x] choice    	| [ ] compare  	| [x] integer   	| [ ] ternary    	| [ ] split         	|
| [x] autoput    	| [x] or        	| [x] at       	| [x] char      	| [x] cleave     	| [ ] some          	|
| [ ] undeferror 	| [x] xor       	| [x] of       	| [x] logical   	| [x] branch     	| [ ] all           	|
| [ ] undefs     	| [x] and       	| [x] size     	| [ ] set       	| [x] ifte       	| [x] help          	|
| [x] echo       	| [x] not       	| [ ] opcase   	| [x] string    	| [ ] ifinteger  	| [x] helpdetail    	|
| [ ] clock      	| [x] +         	| [ ] case     	| [x] list      	| [ ] ifchar     	| [x] manual        	|
| [x] time       	| [x] -         	| [x] uncons   	| [x] leaf      	| [ ] iflogical  	| [x] setautoput    	|
| [ ] rand       	| [x] *         	| [x] unswons  	| [ ] user      	| [ ] ifset      	| [ ] setundeferror 	|
| [ ] stdin      	| [x] /         	| [x] drop     	| [x] float     	| [ ] ifstring   	| [x] setecho       	|
| [ ] stdout     	| [x] ord       	| [x] take     	| [ ] file      	| [ ] iflist     	| [ ] gc            	|
| [ ] stderr     	| [x] chr       	| [x] concat   	| [x] i         	| [ ] iffloat    	| [ ] system        	|
| [x] id         	| [ ] localtime 	| [x] enconcat 	| [x] x         	| [ ] iffile     	| [ ] getenv        	|
| [x] stack      	| [ ] gmtime    	| [ ] name     	| [x] dip       	| [ ] cond       	| [ ] argv          	|
| [x] unstack    	| [ ] mktime    	| [ ] intern   	| [x] app1      	| [ ] while      	| [ ] argc          	|
| [x] dup        	| [ ] strftime  	| [x] body     	| [ ] app11     	| [x] primrec    	| [x] get           	|
| [x] swap       	| [ ] strtol    	| [x] null     	| [ ] app12     	| [x] linrec     	| [x] put           	|
| [x] rollup     	| [ ] strtod    	| [x] small    	| [ ] construct 	| [x] tailrec    	| [x] putch         	|
| [x] rolldown   	| [ ] format    	| [x] >=       	| [x] nullary   	| [x] binrec     	| [x] putchars      	|
| [x] rotate     	| [ ] formatf   	| [x] >        	| [x] unary     	| [x] genrec     	| [x] include       	|
| [x] popd       	| [ ] srand     	| [x] <=       	| [ ] unary2    	| [ ] condlinrec 	| [x] abort         	|
| [x] dupd       	| [x] pred      	| [x] <        	| [ ] unary3    	| [x] step       	| [x] quit          	|
| [x] swapd      	| [x] succ      	| [x] !=       	| [ ] unary4    	| [x] fold       	|                   	|

### mathlib.joy
|	|	|	|	|	|	|
|----------	|-----------	|-----------	|----------	|-----------	|----------	|
| [ ] rem  	| [ ] asin  	| [ ] cosh  	| [ ] modf 	| [ ] tan   	| [x] sign 	|
| [ ] div  	| [ ] atan  	| [ ] exp   	| [x] pow  	| [ ] tanh  	| [x] exp  	|
| [ ] neg  	| [ ] atan2 	| [ ] floor 	| [ ] sin  	| [ ] trunc 	| [x] fib  	|
| [ ] abs  	| [ ] ceil  	| [ ] log   	| [ ] sinh 	| [x] max   	|          	|
| [ ] acos 	| [ ] cos   	| [ ] log10 	| [x] sqrt 	| [x] min   	|          	|

### filelib.joy
|	|	|	|	|	|	|
|------------	|------------	|------------	|-------------	|----------------	|-----------	|
| [ ] fclose 	| [ ] fflush 	| [ ] fopen  	| [ ] fremove 	| [ ] fputch     	| [ ] fseek 	|
| [ ] feof   	| [ ] fgetch 	| [ ] fread  	| [ ] frename 	| [ ] fputchars  	| [ ] ftell 	|
| [ ] ferror 	| [ ] fgets  	| [ ] fwrite 	| [ ] fput    	| [ ] fputstring 	|           	|

## 08/02/20

TODO:
+ helpdetail fix
+ PRIVATE fix

## 31/01/20

It works! testlib.joy and tests work! 

## 30/01/20

inilib.joy works!

## 12/01/20

Nested map works!

map a quoted cube function quote to a list of lists:

```forth
(* comment *)
ok
[ [ 1 2 3 ] [ 4 5 6 ] ] [ [ dup dup * * ] map ] map    # comment
ok
.
[ [ 1 8 27 ] [ 64 125 216 ] ]
ok
```
Next...

Joy commands written in Joy

## 03/01/20

std::any

There needs to be a more flexible approach to tokens rather than simply storing them as a string and doing a lot of string conversions!

Maintaining the current type hierarchy: (?add cmd_t for Joy commands)
```cpp
enum class joy_t {
        undef_t, lexeme_t, any_t,           //abstract types
        numeric_t, aggregate_t ,             //group types
        bool_t, char_t, int_t, double_t,    //simple types
        string_t, set_t, list_t, quote_t    //aggregate types  
    };
```
Modifying the token structure:
```cpp
typedef std::any pattern_t;
typedef std::pair<pattern_t, joy_t> token_t;
```
There can now be a meanigful distinction between a list (a vector of simple types) and a quote (a vector of token_t)
tokenizing, lexing and parsing will have to be smarter so that \[ 1 2 3 \] is recognised as a vector of ints whereas \[ 1 2 3.14 \] and \[ 2.1 dup \* \] is recognised as a vector of token_t 

Its also time to parse string input properly.

## 31/12/19
```cpp
std::initializer_list 
std::function
```

There needs to be a more structured approach to the core C++ operator translations and unify the arguement checking, the recurring pattern:

* operator name string (it would be nice to do this as redefinable tokens but this a decoupling todo for later)
* a definition map c++ lamdas (decouple and permit plurality dictionaries because some operators have more than one set of args eg boolean ops and, or, not act on both bolls and sets)
* arguement type preconditions none {} specific {int_t, int_t, list_t} number {number_t, (int, char, double)} aggregate {aggregate_t} any{any_t} 
* arg conformance or error and stack and state unwind
* stack reference and the c++ lamda function over that stack

With an operator matching function function *match* given an operator string and map<string, function> as arguements points to a map entry for the evaluation function *f* that takes std::initializer_list, std::function

e.g. for the *+* operator 
```cpp
eval("+", regular_dictionary) 
```
map entry:
```cpp
{"+", [](){ f({number_t, number_t}, joy_add() } },

f(std::initializer_list std::function) {...}
```

e.g. for the *and* operator has complex args can be set : set, set : bool, bool : set, or bool : bool so it needs an extra eval layer of its own that manages these combinations


regular dictionary entry:
```cpp
{"and", [](){ f_and(...)} },
```
## 27/12/19

Regular expression lexer v0.1 

With a working joy stack and basic lexer it's time to:
* fix a version of the Joy language to implement and access the manual from within Joy (as per Thun)
* translate a minimal set of Joy regular grammar commands into their c++ lambda equivalents

The language specifications as per Manfred von Thun 16:57.51 on Mar 17 2003, henceforth Joy03, are available at [Kevin Albrecht's](http://www.kevinalbrecht.com/)  [mirror of Thun's original pages](http://www.kevinalbrecht.com/code/joy-mirror/joy.html) from his alma mater [La Trobe University](https://www.latrobe.edu.au/) who - for some inexplicable reason - have deleted Thun's original work!

### TODO:
Translate Joy regular grammar commands into their c++ lambda equivalents *but* only Joy grammar that either:

* 1. can not be expressed in Joy grammar
* 2. offers performance benefits as c++ lambda equivalents

## 23/12/19
The most important thing to recognise about the Joy Programming Language (joylang) is that most of it can be written in Joy. 

The Joy interpeter [original](https://github.com/ifknot/joycpp_old)
 quick and dirty version got me interested in the Joy Programming Language and after some time spent with the writings of Manfred von Thun and others it is time do this beautiful language justice. Time for a better joycpp.

A joycpp where only the most essential parts are written in C++ and the remainder is written in Joy.

Original version repository renamed [joycpp_old](https://github.com/ifknot/joycpp_old) this is the new version.

### Goals:

* simple fast modular
* minimal number of joylang atomic grammar expressed in C++ 
* remainder joylang expressed in Joy

### Implementation
* joylang is a stack based string re-writing system. Therefore, and to avoid using a garbage collector (GC), use a C++ stack of type std::string but embelished with class member functions that map the joylang stack atomics.

* joylang is made up of both context free and regular grammar. Therefore, modularise the C++ inbuilt atomics on this basis with a joylang top layer:
```cpp
joy langguage     : joy
 ↓
joy context free  : c++
 ↓
joy regular       : c++
 ↓
joy stack         : c++
 ↓
input/output      : c++
```
### ToDo:
1. ~~input output~~
2. ~~joy_stack_t~~
3. ~~joy stack layer~~
4. ~~bdd test joy_stack~~
5. ~~joy type handling functions and make helpers~~
6. ~~bdd test joy types and tools~~
7. ~~lexer layer (regular grammar)~~
8. ~~parser layer (context free)~~
9. ~~joy layer~~
10. finish joycpp using joy
