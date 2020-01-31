#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <map>

#include "parse_context_free.h"

// [ [ 1 2 3 ] [ 4 5 6 ] ] [ [ cube ] map ] map .

namespace joy {

	class parse_joy : public parse_context_free {

		enum class joy_state_t{ parse, in, hide, candidate, define, module };
		enum class autoput_state_t { none, top, stack };

	public:

		/**
		* On startup Joy will first determine whether there is a file usrlib.joy in the current directory. 
		* If there is, it is opened for reading. 
		* When the end is reached, or when there is no such file, the terminal or the designated input file is used. 
		* For any particular user this file might look very different:
		* + it might be empty
		* + might contain just a directive to include inilib.joy 
		* + it might contain fewer or it might contain more definitions
		* + it might contain definitions of a very different kind
		*/
		parse_joy(joy_stack& stack, io_device& io, std::string path_to_manual);

		inline bool root_parse(joy_stack& tokens) {
			auto result = parse(tokens, root_stack);
			autoput(root_stack);
			return result;
		}

		inline bool root_parse(joy_stack&& tokens) {
			auto result = parse(tokens, root_stack);
			autoput(root_stack);
			return result;
		}

		using tokenizer::tokenize;

		using parse_context_free::state;

		/**
		* cascade tokens to tokenizer type id the simple type
		* then regular grammar type id
		* then context free grammar type id
		* then joy lambda grammar
		*/
		virtual joy_stack tokenize(joy_stack& tokens) override;

	protected:

		/**
		* string list of all defined symbols
		*/
		virtual std::string help() override;

		/**
		* string list of all private defined symbols
		*/
		std::string _help();

		/**
		* executes Joy operators defined as Joy operators
		* operator matching function and execute if match return true otherwise return false
		*/
		virtual bool call(token_t& token, joy_stack& S) override;

		/**
		* nested parse a quoted program P against a stack S
		*/
		virtual bool parse(token_t& token, joy_stack& S) override;

		virtual bool parse(joy_stack& P, joy_stack& S) override;

	private:

		joy_state_t joy_state{ joy_state_t::parse };
		autoput_state_t autoput_state{ autoput_state_t::top };

		bool abort{ false };
		bool private_access{ false };

		std::string command;
		std::string definition;
		std::string module_name;

		joy_stack tokenize_joy_commands(joy_stack&& tokens);

		bool validate_tokens(joy_stack&& tokens);

		void include(std::string&& path);

		void autoput(joy_stack& S);
		
		//TODO: override definitions

		dictionary_t joy_lambda_map{
			{"help", [&](joy_stack& S) {
				io.colour(YELLOW);
				io << help();			
			}},
			{"include", [&](joy_stack& S) { 
				if (S.has("include", {joy_t::string_t})) {
					auto path = joy_stack::to_string(S.top());
					S.pop();
					include(path.substr(1, path.size() - 2));
				}
			}},
			{"autoput", [&](joy_stack& S) {
				switch (autoput_state) {
				case autoput_state_t::none:
					S.push(make_token(0, joy_t::int_t));
					break;
				case autoput_state_t::top:
					S.push(make_token(1, joy_t::int_t));
					break;
				case autoput_state_t::stack:
					S.push(make_token(2, joy_t::int_t));
					break;
				}
			}},
			{"body", [&](joy_stack& S) {
				auto match = joy_stack::to_string(S.top());
				match = match.substr(1, match.size() - 2);
				S.pop();
				auto jt = public_joy_joy_map.find(match);
				if (jt != public_joy_joy_map.end()) {
					io.colour(BOLDYELLOW);
					io << match + " == " + public_joy_joy_map[match];
				}
				else {
					error(XDEFNREJECTED, match + " not found");
				}
			}},
			{"echo", [&](joy_stack& S) {
				switch (echo_state) {
				case echo_state_t::none:
					S.push(make_token(0, joy_t::int_t));
					break;
				case echo_state_t::echo:
					S.push(make_token(1, joy_t::int_t));
					break;
				case echo_state_t::tab:
					S.push(make_token(2, joy_t::int_t));
					break;
				case echo_state_t::linenumber:
					S.push(make_token(3, joy_t::int_t));
					break;
				}
			}},
			{"setautoput", [&](joy_stack& S) { 
				if (S.has("setautoput", {joy_t::int_t})) { 
					auto state = std::any_cast<int>(S.top().first);
					S.pop();
					switch (state) {
					case 0:
						autoput_state = autoput_state_t::none;
						break;
					case 1:
						autoput_state = autoput_state_t::top;
						break;
					case 2:
						autoput_state = autoput_state_t::stack;
						break;
					default: //invalid input error
						break;
					}
				} 
			}},
			{"setecho", [&](joy_stack& S) { 
				if (S.has("setecho", {joy_t::int_t})) {
					auto state = std::any_cast<int>(S.top().first);
					S.pop();
					switch (state) {
					case 0:
						echo_state = echo_state_t::none;
						break;
					case 1:
						echo_state = echo_state_t::echo;
						break;
					case 2:
						echo_state = echo_state_t::tab;
						break;
					case 3:
						echo_state = echo_state_t::linenumber;
						break;
					default: //invalid input error
						break;
					};
				}
			}},
			{"MODULE", [&](joy_stack& S) { 
				joy_state = joy_state_t::module; 
			}},
			{"PUBLIC", [&](joy_stack& S) { 
				private_access = false; 
			}},
			{"PRIVATE", [&](joy_stack& S) { 
				private_access = true; 
			}},
			{"==", [&](joy_stack& S) { error(XRESERVED, "== (ignored)"); }},
			{";", [&](joy_stack& S) { error(XRESERVED, "; (ignored)"); }},
			{".", [&](joy_stack& S) { }} 
		};

		std::map<std::string, std::string> public_joy_joy_map {
			{"END", " ."},
			//for backwards compatibility
			{"HIDE", " PRIVATE"},
			{"IN", " PUBLIC"},
			{"LIBRA", " PUBLIC"},
			{"DEFINE", " PUBLIC"},
			{"?", "helpdetail"}
		};

		std::map<std::string, std::string> private_joy_joy_map {
			
		};


	};

}