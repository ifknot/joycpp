#pragma once

#include <string>
#include <sstream>
#include <fstream>
#include <map>

#include "parse_context_free.h"

// [ [ 1 2 3 ] [ 4 5 6 ] ] [ [ cube ] map ] map .

namespace joy {

	/**
	* The parse_joy class is the final layer of the joycpp interpreter providing:
	* + Chomsky type 2 context free Joy grammar parser dual Joy-Joy and Joy-C++
	* + the ability to load libraries of Joy code
	* + the ability to define Joy grammar from a mixture Joy C++ primitives and Joy definitions
	* + the ability to build modules of public and private Joy code that use both pure Joy and Joy C++ primitives to build the Joy language
	* + the ability to extend the language with user definitions.
	*
	* joycpp uses an OOP hierarchy to add layers of functionality:
	* + tokenizer class 
	* + regular grammar parser Joy primitives to core C++ implementations
	* + context free grammar parser Joy primitives to core C++ implementations
	* + context free grammar parser Joy to Joy primitives
	* 
	* The Joy language can be concatenate built from a very minimal set of Joy primitives expressed in C++.
	* joycpp extends that minimal set of C++ primitives where it makes sense from a performance and OS perspective.
	*
	*/
	class parse_joy : public parse_context_free {

		/**
		* The Joy programming language is a concatenation functional stack language and by these virtues
		* does not need to employ a parse tree for its execution - rather it can be parsed using stacks.
		* Joy consists of parts with different rules of parsing: 
		* + keywords like PUBLIC or include don’t make sense inside strings
		* + strings may contain backslash-escaped symbols like \n 
		* + comments usually don’t contain anything interesting except the end of the comment
		*
		* In joycpp such parts are represented as states where each state consists of:
		* + starting condition
		* + state actions
		* + state changing condition
		* + ending condition
		*/
		enum class joy_state_t{ abort,
								parse,	/// map Joy keywords to Joy and C++ implementations
								in,		/// build public definition
								hide,	/// build private definition
								candidate,	/// potential keyword 
								define,		/// build the definition for the confirmed keyword
								module		/// place definitions inside a module name
							};

		/**
		* Joy has an autoput feature that can select automatic output options when an interpret cycle has completed:
		*/
		enum class autoput_state_t {	none,	/// no output
										top,	/// the item of the top of stack is printed to the iostream
										stack	/// the enitre stack is printed to the iostream
									};

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

		joy_state_t joy_state{ joy_state_t::parse }; /// start out in parse state
		autoput_state_t autoput_state{ autoput_state_t::top }; /// default Joy is to autoput top of stack

		bool private_access{ false };
		
		std::string command;		/// current keyword 
		std::string definition;		/// current definition for keyword
		std::string module_name;	/// current module name (if any)

		joy_stack tokenize_joy_commands(joy_stack&& tokens);

		bool validate_tokens(joy_stack&& tokens);

		/* Joy Keyword primitives C++ definitions */

		void include(std::string&& path);

		void autoput(joy_stack& S);

		virtual std::string help() override;

		std::string _help();

		/* Joy Keyword C++ lambda definitions */

		dictionary_t joy_lambda_map {
			//{"END", [&](joy_stack& S) { module_name.clear(); }},
			{"abort", [&](joy_stack& S) {
				if (S.has("abort", {joy_t::bool_t})) {
					if (!joy_cast<bool>(S.top())) {
						joy_state = joy_state_t::abort;
					}
				}
			}},
			{"help", [&](joy_stack& S) {
				io.colour(YELLOW);
				io << help();			
			}},
			{"include", [&](joy_stack& S) { 
				if (S.has("include", {joy_t::string_t})) {
					auto path = to_string(S.top());
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
				auto match = to_string(S.top());
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
					auto state = joy_cast<int>(S.top());
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
					auto state = joy_cast<int>(S.top());
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
		};

		std::map<std::string, std::string> private_joy_joy_map {
			
		};


	};

}