#include "parser.h"

namespace joy {



	parser::parser(joy_stack& stack, io_device& io, std::string path_to_manual) :
		lexer(stack, io, path_to_manual)
	{
		state_stack.push(state_t::parse);
	}

	void parser::parse(std::string line) {
		auto tokens = tokenizer::tokenize(line);
		parse(tokens);
	}

	void parser::parse(joy_stack& stack) {
		assert(state_stack.top() == state_t::parse);
		for (auto& token : stack) {
			if (!parse(token)) {
				break;
			}
		}
	}

	void parser::parse(joy_stack&& stack) {
		assert(state_stack.top() == state_t::parse);
		for (auto& token : stack) {
			if (!parse(token)) {
				break;
			}
		}
	}

	bool parser::parse(token_t& token) {
		if (jundef(token)) {
			if (state_change(token)) {
				return true;
			}
			if (is_context_free(token) || is_regular(token)) {
				token.second = joy_t::cmd_t;
			}
			else {
				unwind();
				return lexer::no_conversion(token);
			}
		}
		switch (state_stack.top()) {
		case joy::state_t::parse:
			if (jcmd(token)) {
				return state_change(token) || context_free(token) || regular(token);
			} 
			else {
				root_stack.push(token);
				return true;
			}
		case joy::state_t::list:
		case joy::state_t::quote:
			nest_token(token, root_stack, root_type, list_depth);
			return true;
		case joy::state_t::set:
			// TODO:
			return false;
			break;
		}
	}

	bool parser::is_context_free(token_t& token) {
		assert(token.second == joy_t::undef_t);
		auto it = context_free_atoms.find(std::any_cast<std::string>(token.first));
		if (it != context_free_atoms.end()) {
			return true;
		}
		return false;
	}

	bool parser::state_change(token_t& token) {
		auto it = state_change_atoms.find(std::any_cast<std::string>(token.first));
		if (it != state_change_atoms.end()) {
			(it->second)();
			return true;
		}
		return false;
	}

	bool parser::context_free(token_t& token) {
		auto it = context_free_atoms.find(std::any_cast<std::string>(token.first));
		if (it != context_free_atoms.end()) {
			(it->second)();
			return true;
		}
		return false;
	}

	void parser::nest_list(joy_stack& stack, size_t depth) {
		if (depth == 0) {
			stack.emplace_back(joy_stack{}, joy_t::list_t); //default to a list type...
		}
		else {
			assert(jgroup(stack.top()));
			nest_list(std::any_cast<joy_stack&>(stack.top().first), depth - 1);
		}
	}

	void parser::nest_token(token_t& token, joy_stack& stack, joy_t& type, size_t depth) {
		if (depth == 0) {
			stack.push(token);
			if (token.second == joy_t::cmd_t) {
				type = joy_t::quote_t; //...unless a joy command is added
			}
		}
		else {
			assert(jgroup(stack.top())); 
			nest_token(token, std::any_cast<joy_stack&>(stack.top().first), stack.top().second, depth - 1);
		}
	}

	void parser::unwind() {
		while (list_depth) {
			state_stack.pop();
			--list_depth;
		}
	}

	void parser::infra(joy_stack& S) {
		auto P = S.top(); // get the program
		S.pop();
		joy_stack M; //fresh stack to work with
		M.push(S.top()); //get the list
		S.pop();
		M.unstack(); //make the list the stack
		M.push(P); // push the program 
		auto T = root_stack; //temporarily discard remainder of the root stack
		root_stack = M; // set the new root stack
		i(root_stack); //execute the program
		root_stack.stack(); //convert the stack to a list [N]
		auto N = root_stack.top(); //get the result of P M as N
		root_stack = T; //restore the stack
		root_stack.push(N); //push [N] onto original stack as the result
	}

	token_t parser::map(joy_stack& S) {
		/*
		auto P = std::any_cast<joy_stack>(S.top().first); // get the program
		if (P.size()) {
			S.pop();
			auto A = std::any_cast<joy_stack>(S.top().first); // get the aggregate
		}
		*/
		
		auto size = std::any_cast<joy_stack&>(S.sat(1).first).size(); //aggregate size
		auto type = S.sat(1).second; //get the return aggregate type
		step(S);
		joy_stack M;
		while (size--) {
			M.push(root_stack.top());
			root_stack.pop();
		}
		std::reverse(M.begin(), M.end()); 
		return make_token(M, type);
		
	}

	void parser::dip(joy_stack& S) {
		auto P = std::any_cast<joy_stack&>(S.top().first);
		const auto X = root_stack.sat(1);
		root_stack.pop2();
		parse(P);
		S.push(X);
	}

	void parser::i(joy_stack& S) {
		auto P = std::any_cast<joy_stack&>(S.top().first);
		S.pop();
		parse(P);
	}

	void parser::step(joy_stack& S) {	
		auto P = std::any_cast<joy_stack&>(S.top().first); //get the program
		S.pop();
		auto A = std::any_cast<joy_stack&>(S.top().first); //get the aggregate
		S.pop();
		joy_stack M; 
		for (auto a : A) {
			M.push(a);
			for (auto p : P) {
				M.push(p);
			}
		}
		parse(M);
	}

}
