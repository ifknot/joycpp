#include "joy_stack.h"
#include "error.h"

namespace joy {
	
	void joy_stack::newstack() {
		clear();
	}

	void joy_stack::stack() {
		joy_stack S = *this;
		std::reverse(S.begin(), S.end());
		push(make_token(S, joy_t::quote_t));
	}

	void joy_stack::unstack() {
		if(has("unstack", { joy_t::sequence_t })) {
		auto S = std::any_cast<joy_stack>(top().first);
		std::reverse(S.begin(), S.end());
		*this = S;
	}
	}

	void joy_stack::dup() {
		if (has("dup", { joy_t::any_t })) {
			assert(size());
			push(top());
		}
	}

	void joy_stack::dupd() {
		if (has("dupd", { joy_t::any_t, joy_t::any_t })) {
			assert(size() > 1);
			dup();
			sat(1) = sat(2);
		}
	}

	void joy_stack::pop() {
		if (has("pop", { joy_t::any_t })) {
			assert(size());
			pop_back();
		}
	}

	void joy_stack::popd() {
		if (has("popd", { joy_t::any_t, joy_t::any_t })) {
			assert(size() > 1);
			sat(1) = sat(0);
			pop();
		}
	}

	void joy_stack::pop2() {
		if (has("pop2", { joy_t::any_t, joy_t::any_t })) {
			assert(size() > 1);
			pop();
			pop();
		}
	}

	void joy_stack::unit() {
		if (has("unit", { joy_t::any_t })) {
			joy_stack S;
			S.push(top());
			top() = make_token(S, joy_t::quote_t);
		}
	}

	void joy_stack::swap() {
		if (has("swap", { joy_t::any_t, joy_t::any_t })) {
			assert(size() > 1);
			std::swap(sat(0), sat(1));
		}
	}

	void joy_stack::swapd() {
		if (has("swapd", { joy_t::any_t, joy_t::any_t, joy_t::any_t })) {
			assert(size() > 1);
			std::swap(sat(1), sat(2));
		}
	}

	void joy_stack::rotate() {
		if (has("rotate", { joy_t::any_t, joy_t::any_t, joy_t::any_t })) {
			assert(size() > 2);
			std::swap(sat(0), sat(2));
		}
	}

	void joy_stack::rollup() {
		if (has("rollup", { joy_t::any_t, joy_t::any_t, joy_t::any_t })) {
			swap();
			swapd();
		}
	}

	void joy_stack::rolldown() {
		if (has("rolldown", { joy_t::any_t, joy_t::any_t, joy_t::any_t })) {
			rotate();
			swapd();
		}
	}

	joy_stack::const_reference joy_stack::sat(size_t i) const {
		assert(i < size());
		return at(size() - 1 - i);
	}

	joy_stack::reference joy_stack::sat(size_t i) {
		assert(i < size());
		return at(size() - 1 - i);
	}

	joy_stack::reference joy_stack::top() {
		assert(size());
		return back();
	}

	joy_stack::const_reference joy_stack::top() const {
		assert(size());
		return back();
	}

	void joy_stack::push(const value_type& val) {
		push_back(val);
	}

	void joy_stack::push(value_type&& val) {
		push_back(val);
	}

	bool joy_stack::has(const std::string& op, const prerequisite_t& argt) {
		if (size() >= argt.size()) {
			size_t i{ 0 };
			for (const auto& t : argt) { //for each of the joy types in the initializer list
				switch (t) {
				case joy_t::bool_t:
				case joy_t::int_t:
				case joy_t::char_t:
				case joy_t::double_t:
				case joy_t::quote_t:
				case joy_t::list_t:
				case joy_t::set_t:
				case joy_t::string_t:
				case joy_t::lamda_t:
					if (sat(i).second != t) {
						return error(XTYPEMISMATCH, op + " argument at stack[" + std::to_string(i) + "] expected: " + joy::to_string(t) + " found: " + joy::to_string(sat(i).second));
					}
					break;
				case joy_t::numeric_t:
					if (jnumeric(sat(i))) {
						break;
					}
					else {
						return error(XTYPEMISMATCH, op + " argument at stack[" + std::to_string(i) + "] expected: " + joy::to_string(t) + " found: " + joy::to_string(sat(i).second));
					}
					return false;
				case joy_t::group_t:
					if (jgroup(sat(i))) {
						break;
					}
					else {
						return error(XTYPEMISMATCH, op + " argument at stack[" + std::to_string(i) + "] expected: " + joy::to_string(t) + " found: " + joy::to_string(sat(i).second));
					}
					return false;
				case joy_t::sequence_t:
					if (jsequence(sat(i))) {
						break;
					}
					else {
						return error(XTYPEMISMATCH, op + " argument at stack[" + std::to_string(i) + "] expected: " + joy::to_string(t) + " found: " + joy::to_string(sat(i).second));
					}
					return false;
				case joy_t::aggregate_t:
					if (jaggregate(sat(i))) {
						break;
					}
					else {
						return error(XTYPEMISMATCH, op + " argument at stack[" + std::to_string(i) + "] expected: " + joy::to_string(t) + " found: " + joy::to_string(sat(i).second));
					}
					return false;
				case joy_t::any_t:
					break;
				case joy_t::undef_t:
				default:
					error(XNOCONVERSION, op + " argument conformance checking");
					return false;
				}
				++i;
			}
			return true;
		}
		else {
			return error(XARGC, op + " expected: " + std::to_string(argt.size()) + " found: " + std::to_string(size()));
		}
	}

	std::string joy_stack::to_string(const token_t& token) {
		std::string result;
		switch (token.second) {
		case joy::joy_t::list_t:
		case joy::joy_t::quote_t:
			result += "[ " + joy_stack::to_string(std::any_cast<joy_stack>(token.first)) + "]";
			break;
		case joy::joy_t::set_t:
			break;
		case joy::joy_t::string_t: {
			result += "\"";
			const auto tokens = std::any_cast<joy_stack>(token.first);
			for (auto t : tokens) {
				result.push_back(std::any_cast<char>(t.first));
			}
			result += "\"";
			break;
		}
		case joy::joy_t::bool_t:
			result += (std::any_cast<bool>(token.first)) ? "true" : "false";
			break;
		case joy::joy_t::char_t:
			result += "'" + std::string{ std::any_cast<char>(token.first) };
			break;
		case joy::joy_t::int_t:
			result += std::to_string(std::any_cast<int>(token.first));
			break;
		case joy::joy_t::double_t:
			result += std::to_string(std::any_cast<double>(token.first));
			break;
		default:
			result += std::any_cast<std::string>(token.first);
			break;
		}
		return result;// +" " + joy::to_string(token.second);
	}

	std::string joy_stack::to_string(const joy_stack& stack) {
		std::string result;
		for (const auto& token : stack) {
			result += to_string(token) + " ";
		}
		return result;
	}

}