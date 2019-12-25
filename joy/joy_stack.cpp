#include "joy_stack.h"

namespace joy {

#ifndef NDEBUG
	void joy_stack::dump() {
		std::cerr << "_\n";
		for (size_t i{ 0 }; i < size(); ++i) {
			const auto [pattern, joy_type] = sat(i);
			std::cerr << pattern << " " << to_string(joy_type) << "\n";
		}
	}
#endif

	base_stack_t::const_reference joy_stack::sat(int i) const {
		assert(i < size());
		return at(size() - 1 - i);
	}

	base_stack_t::reference joy_stack::sat(int i) {
		assert(i < size());
		return at(size() - 1 - i);
	}

	void joy_stack::push(const value_type& val) {
		push_back(val);
	}

	void joy_stack::push(value_type&& val) {
		push_back(val);
	}

	void joy_stack::newstack() {
		clear();
	}

	void joy_stack::stack() {
	}

	void joy_stack::dup() {
		if (size()) {
			push(top());
		}
	}

	void joy_stack::dupd() {
		if (size()) {
			dup();
			sat(1) = sat(2);
		}
	}

	void joy_stack::pop() {
		if (size()) {
			pop_back();
		}
	}

	void joy_stack::popd() {
		if (size() > 1) {
			sat(1) = sat(0);
			pop();
		}
	}

	void joy_stack::pop2() {
		if (size() > 1) {
			pop();
			pop();
		}
	}

	void joy_stack::unit() {
		top() = make_quoted_token(top().first);
	}

	void joy_stack::swap() {
		if (size() > 1) {
			std::swap(sat(0), sat(1));
		}
	}

	void joy_stack::swapd() {
		if (size() > 1) {
			std::swap(sat(1), sat(2));
		}
	}

	void joy_stack::rotate() {
		if (size() > 2) {
			std::swap(sat(0), sat(2));
		}
	}

	void joy_stack::rollup() {
		swap();
		swapd();
	}

	void joy_stack::rolldown() {
		rotate();
		swapd();
	}

	base_stack_t::reference joy_stack::top() {
		if (size()) {
			return back();
		}
		else {
			throw std::runtime_error(NULL_STACK);
		}
	}

	base_stack_t::const_reference joy_stack::top() const {
		if (size()) {
			return back();
		}
		else {
			throw std::runtime_error(NULL_STACK);
		}
	}


}