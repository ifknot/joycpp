/**
* Chomsky type 3 regular grammar lexer 
* i.e. can be decided by a finite state automaton
* Uses a map of string operator to lamda function mapping for Joy operators
*/
#pragma once

#include <cassert>

#include "error.h"
#include "to_string.h"
#include "tokenizer.h"
#include "joy_overloads.h"
#include "joy_operators.h"

namespace joy {

	class lexer : public tokenizer {

	public:

		typedef std::function<void(joy_stack& S)> function_t;
		typedef std::map<std::string, function_t> dictionary_t;

		lexer(joy_stack& stack, io_device& io, std::string path_to_manual);

		bool is_quit();

		void quit();

	protected:

		/**
		* root stack
		*/
		joy_stack& root_stack;	

		/**
		* root type
		*/
		joy_t root_type{ joy_t::quote_t };

		/**
		* try to map token to a regular grammar C++ lamda implementation of a regular grammar Joy operator
		*/
		bool regular(token_t& token, joy_stack& S);

		/**
		* test if the token is Joy03 regular grammar
		*/
		bool is_regular(token_t& token);

		/**
		* display error then return false
		*/
		bool no_conversion(token_t& token);

	private: 

		/**
		* internal quit flag
		*/
		bool quit_{ false };

		/**
		* try and execute token as a regular Joy command by searching the regular expression c++ dictionary
		* return true on succes or call no_conversion
		*/
		bool try_regular(token_t& token, joy_stack& S);

		//helper member functions

		void load_manual(std::string& path_to_manual);

		//Joy op C++ implementations

		//void print_top(const joy_stack& S);

		//void print_stack(const joy_stack& S);

		/**
		* FIX: this prob needs to be nested joy(parse(lex))) to print all symbols
		* help : ->
		* Lists all defined symbols, including those from library files.
		* Then lists all primitives of raw Joy.
		*/
		//void help()

		/**
		* Joy03 (language specs as per Manfred von Thun 16:57.51 on Mar 17 2003)
		* translate Joy regular grammar commands into their c++ lambda equivalents only Joy grammar that either:
		* 1. can not be expressed in Joy grammar
		* 2. offer performance benefit as c++ lambda equivalent
		*/
		dictionary_t regular_translation { 
		//non-standard
		{"?", [&](joy_stack& S) { if (S.has("?", {joy_t::quote_t})) { helpdetail(std::any_cast<joy_stack&>(S.top().first), joy_manual, io); } }},
		//stack commands
		{".s", [&](joy_stack& S) { print_stack(S, io); }},
		{".", [&](joy_stack& S) { if (S.has(".", {joy_t::any_t})) { print_top(S, io); } }},
		{"newstack", [&](joy_stack& S) { S.newstack(); }},
		{"stack", [&](joy_stack& S) { S.stack(); }},
		//change has to use a lookup map depending on the op eg args("popd") {...}
		{"unstack", [&](joy_stack& S) { S.unstack(); }},
		{"dup", [&](joy_stack& S) { S.dup(); }},
		{"dupd", [&](joy_stack& S) { S.dupd(); }},
		{"pop", [&](joy_stack& S) { S.pop(); }},
		{"popd", [&](joy_stack& S) { S.popd(); }},
		{"pop2", [&](joy_stack& S) { S.pop2(); }},
		{"unit", [&](joy_stack& S) { S.unit(); }},
		{"swap", [&](joy_stack& S) { S.swap(); }},
		{"swapd", [&](joy_stack& S) { S.swapd(); }},
		{"rotate", [&](joy_stack& S) { S.rotate(); }},
		{"rollup", [&](joy_stack& S) { S.rollup(); }},
		{"rolldown", [&](joy_stack& S) { S.rolldown(); }},
		//char
		{"ord", [&](joy_stack& S) { if (S.has("ord", {joy_t::char_t})) { S.push(ord(S.top())); } }},
		{"chr", [&](joy_stack& S) { if (S.has("chr", {joy_t::int_t})) { S.push(chr(S.top())); } }},
		{"char", [&](joy_stack& S) { if (S.has("char", {joy_t::any_t})) { S.push(is_char(S.top())); } }},
		//math
		{"+", [&](joy_stack& S) {
			if (S.has("+", {joy_t::numeric_t, joy_t::numeric_t})) {
				S.sat(1) = (S.sat(1) + S.top());
				S.pop();
			}
		}},
		{"-", [&](joy_stack& S) {
			if (S.has("-", {joy_t::numeric_t, joy_t::numeric_t})) {
				S.sat(1) = (S.sat(1) - S.top());
				S.pop();
			}
		}},
		{"*", [&](joy_stack& S) {
			if (S.has("*", {joy_t::numeric_t, joy_t::numeric_t})) {
				S.sat(1) = (S.sat(1) * S.top());
				S.pop();
			}
		}},
		{"/", [&](joy_stack& S) {
			if (S.has("/", {joy_t::numeric_t, joy_t::numeric_t})) {
				S.sat(1) = (S.sat(1) / S.top());
				S.pop();
			}
		}},
		//aggregates
		// size  ==  [ pop 1 ]  map  sum
		{"size", [&](joy_stack& S) { size(S); }},
		//environment
		{"manual", [&](joy_stack& S) { manual(joy_manual, io); }},
		{"quit", [&](joy_stack& S) { quit(); io << ". . ."; }}
		};

		/**
		* Joy Manual loaded from file at construction
		*/
		std::map<std::string, std::string> joy_manual;

	};

}