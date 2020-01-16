/**
* Chomsky type 3 regular grammar lexer 
* i.e. can be decided by a finite state automaton
* Uses a map of string operator to lamda function mapping for Joy operators
*/
#pragma once

#include <cassert>

#include "to_string.h"

#include "tokenizer.h"
#include "joy_stack.h"
#include "primitives_minimal.h"

namespace joy {

	class lexer : public tokenizer {

		using prerequisites = std::initializer_list<joy_t>;

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
		bool regular(token_t& token);

		/**
		* test if the token is Joy03 regular grammar
		*/
		bool is_regular(token_t& token);

		/**
		* argument conformability checking
		* test stack has the requirments of the initializer list
		*/
		bool require(const std::string& op, const prerequisites& argt);

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

		/**
		* Joy03 (language specs as per Manfred von Thun 16:57.51 on Mar 17 2003)
		* translate Joy regular grammar commands into their c++ lambda equivalents only Joy grammar that either:
		* 1. can not be expressed in Joy grammar
		* 2. offer performance benefit as c++ lambda equivalent
		*/
		dictionary_t regular_translation { 
		//non-standard
		{"?", [&]() { if (require("?", {joy_t::quote_t})) { helpdetail(std::any_cast<joy_stack&>(root_stack.top().first)); } }},
		//stack commands
		{".s", [&]() { print_stack(root_stack); }},
		{".", [&]() { if (require(".", {joy_t::any_t})) { print_top(root_stack); } }},
		{"newstack", [&]() { root_stack.newstack(); }},
		{"stack", [&]() { root_stack.stack(); }},
		//change has to use a lookup map depending on the op eg args("popd") {...}
		{"unstack", [&]() { if (require("unstack", {joy_t::group_t})) { root_stack.unstack(); } }},
		{"dup", [&]() { if (require("dup", {joy_t::any_t})) { root_stack.dup(); } }},
		{"dupd", [&]() { if (require("dupd", {joy_t::any_t, joy_t::any_t})) { root_stack.dupd(); } }},
		{"pop", [&]() { if (require("pop", {joy_t::any_t})) { root_stack.pop(); } }},
		{"popd", [&]() { if (require("popd", {joy_t::any_t, joy_t::any_t})) { root_stack.popd(); } }},
		{"pop2", [&]() { if (require("pop2", {joy_t::any_t, joy_t::any_t})) { root_stack.pop2(); } }},
		{"unit", [&]() { if (require("unit", {joy_t::any_t})) { root_stack.unit(); } }},
		{"swap", [&]() { if (require("swap", {joy_t::any_t, joy_t::any_t})) { root_stack.swap(); } }},
		{"swapd", [&]() { if (require("swapd", {joy_t::any_t, joy_t::any_t, joy_t::any_t})) { root_stack.swapd(); } }},
		{"rotate", [&]() { if (require("rotate", {joy_t::any_t, joy_t::any_t, joy_t::any_t})) { root_stack.rotate(); } }},
		{"rollup", [&]() { if (require("rollup", {joy_t::any_t, joy_t::any_t, joy_t::any_t})) { root_stack.rollup(); } }},
		{"rolldown", [&]() { if (require("rolldown", {joy_t::any_t, joy_t::any_t, joy_t::any_t})) { root_stack.rolldown(); } }},
		//char
		{"ord", [&]() {
			if (require("ord", {joy_t::char_t})) {
				root_stack.push(make_token(static_cast<int>(std::any_cast<char>(root_stack.top().first)), joy_t::int_t));
				root_stack.popd();
			}
		}},
		{"chr", [&]() {
			if (require("chr", {joy_t::int_t})) { 
				root_stack.push(make_token(static_cast<char>(std::any_cast<int>(root_stack.top().first)), joy_t::char_t));
				root_stack.popd();
			}
		}},
		{"char", [&]() {
			if (require("char", {joy_t::any_t})) {
				root_stack.push(make_token((root_stack.top().second == joy_t::char_t) ? true : false, joy_t::bool_t));
			}
		}},
		//math
		{"+", [&]() {
			if (require("+", {joy_t::numeric_t, joy_t::numeric_t})) {
				root_stack.sat(1) = (root_stack.sat(1) + root_stack.top());
				root_stack.pop();
			}
		}},
		{"-", [&]() {
			if (require("-", {joy_t::numeric_t, joy_t::numeric_t})) {
				root_stack.sat(1) = (root_stack.sat(1) - root_stack.top());
				root_stack.pop();
			}
		}},
		{"*", [&]() {
			if (require("*", {joy_t::numeric_t, joy_t::numeric_t})) {
				root_stack.sat(1) = (root_stack.sat(1) * root_stack.top());
				root_stack.pop();
			}
		}},
		{"/", [&]() {
			if (require("/", {joy_t::numeric_t, joy_t::numeric_t})) {
				root_stack.sat(1) = (root_stack.sat(1) / root_stack.top());
				root_stack.pop();
			}
		}},
		//aggregates
		// size  ==  [ pop 1 ]  map  sum
		{"size", [&]() { 
			if (require("size", {joy_t::aggregate_t})) {
				root_stack.push(make_token(static_cast<int>(std::any_cast<joy_stack>(root_stack.top().first).size()), joy_t::int_t));
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