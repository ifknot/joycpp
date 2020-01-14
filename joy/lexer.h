/**
* Chomsky type 3 regular grammar lexer 
* i.e. can be decided by a finite state automaton
* Uses a map of string operator to lamda function mapping for Joy operators
*/
#pragma once

#include <cassert>

#include "joy_to_string.h"

#include "tokenizer.h"
#include "joy_stack.h"
#include "joy_primitives_minimal.h"

namespace joy {

	class lexer : public tokenizer {

	public:

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
		bool lex(token_t& token);

		/**
		* test if the token is Joy03 regular grammar
		*/
		bool is_lexable(token_t& token);

		/**
		* test stack has the requirments of the initializer list
		*/
		bool has(const std::initializer_list<joy_t>& argt, const joy_stack& stack);

		/**
		* display error then return false
		*/
		bool no_conversion(token_t& token);

	private: 

		/**
		* try and execute token as a regular Joy command by searching the regular expression c++ dictionary
		* return true on succes or call no_conversion
		*/
		bool try_regular(token_t& token);

		//helper member functions

		void load_manual(std::string& path_to_manual);

		//Joy op C++ implementations

		void print_top(const joy_stack& stack);

		void print_stack(const joy_stack& stack);

		/**
		* manual: ->
		* Writes the loaded manual of all Joy primitives to output file.
		*/
		void manual();

		/**
		* helpdetail: [ S1 S2 .. ]
		* Gives brief help on each symbol S in the list.
		* FIX: work with list not just string on top stack
		*/
		void helpdetail(const joy_stack& stack);

		//vars

		bool quit_{ false };

		/**
		* Joy03 (language specs as per Manfred von Thun 16:57.51 on Mar 17 2003)
		* translate Joy regular grammar commands into their c++ lambda equivalents only Joy grammar that either:
		* 1. can not be expressed in Joy grammar
		* 2. offer performance benefit as c++ lambda equivalent
		*/
		dictionary_t regular_translation { 
		//non-standard
		{"?", [&]() { if (has({joy_t::list_t}, root_stack)) { helpdetail(std::any_cast<joy_stack&>(root_stack.top().first)); } }},
		//stack commands
		{".s", [&]() { print_stack(root_stack); }},
		{".", [&]() { if (has({joy_t::any_t}, root_stack)) { print_top(root_stack); } }},
		{"newstack", [&]() { root_stack.newstack(); }},
		{"stack", [&]() { root_stack.stack(); }},
		{"unstack", [&]() { if (has({joy_t::quote_t}, root_stack)) { root_stack.unstack(); } }},
		{"dup", [&]() { if (has({joy_t::any_t}, root_stack)) { root_stack.dup(); } }},
		{"dupd", [&]() { if (has({joy_t::any_t, joy_t::any_t}, root_stack)) { root_stack.dupd(); } }},
		{"pop", [&]() { if (has({joy_t::any_t}, root_stack)) { root_stack.pop(); } }},
		{"popd", [&]() { if (has({joy_t::any_t, joy_t::any_t}, root_stack)) { root_stack.popd(); } }},
		{"pop2", [&]() { if (has({joy_t::any_t, joy_t::any_t}, root_stack)) { root_stack.pop2(); } }},
		{"unit", [&]() { if (has({joy_t::any_t}, root_stack)) { root_stack.unit(); } }},
		{"swap", [&]() { if (has({joy_t::any_t, joy_t::any_t}, root_stack)) { root_stack.swap(); } }},
		{"swapd", [&]() { if (has({joy_t::any_t, joy_t::any_t, joy_t::any_t}, root_stack)) { root_stack.swapd(); } }},
		{"rotate", [&]() { if (has({joy_t::any_t, joy_t::any_t, joy_t::any_t}, root_stack)) { root_stack.rotate(); } }},
		{"rollup", [&]() { if (has({joy_t::any_t, joy_t::any_t, joy_t::any_t}, root_stack)) { root_stack.rollup(); } }},
		{"rolldown", [&]() { if (has({joy_t::any_t, joy_t::any_t, joy_t::any_t}, root_stack)) { root_stack.rolldown(); } }},
		//char
		{"ord", [&]() {
			if (has({joy_t::char_t}, root_stack)) {
				auto c = std::any_cast<char>(root_stack.top().first);
				root_stack.push(make_token(static_cast<int>(c), joy_t::int_t));
				root_stack.popd();
			}
		}},
		{"chr", [&]() {
			if (has({joy_t::int_t}, root_stack)) { 
				auto c = static_cast<char>(std::any_cast<int>(root_stack.top().first));
				root_stack.push(make_token(c, joy_t::char_t));
				root_stack.popd();
			}
		}},
		{"char", [&]() {
			if (has({joy_t::any_t}, root_stack)) {
				root_stack.push(make_token((root_stack.top().second == joy_t::char_t) ? true : false, joy_t::bool_t));
			}
		}},
		//math
		{"+", [&]() {
			if (has({joy_t::numeric_t, joy_t::numeric_t}, root_stack)) {
				root_stack.sat(1) = (root_stack.sat(1) + root_stack.top());
				root_stack.pop();
			}
		}},
		{"-", [&]() {
			if (has({joy_t::numeric_t, joy_t::numeric_t}, root_stack)) {
				root_stack.sat(1) = (root_stack.sat(1) - root_stack.top());
				root_stack.pop();
			}
		}},
		{"*", [&]() {
			if (has({joy_t::numeric_t, joy_t::numeric_t}, root_stack)) {
				root_stack.sat(1) = (root_stack.sat(1) * root_stack.top());
				root_stack.pop();
			}
		}},
		{"/", [&]() {
			if (has({joy_t::numeric_t, joy_t::numeric_t}, root_stack)) {
				root_stack.sat(1) = (root_stack.sat(1) / root_stack.top());
				root_stack.pop();
			}
		}},
		//aggregates
		// TODO:
		{"size", [&]() { 
			if (has({joy_t::aggregate_t}, root_stack)) {
				//s.push(size(s.top()));
			}
		}},
		//environment
		{"manual", [&]() { manual(); }},
		{"quit", [&]() { quit(); io << ". . ."; }}
		};

		/**
		* Joy Manual loaded from file at construction
		*/
		std::map<std::string, std::string> joy_manual;

	};

}