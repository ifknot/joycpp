#include "joy_stack.h"

namespace joy {

#ifndef NDEBUG
	void joy_stack::dump() {
		std::cerr << "_\n";
		if (size() != 0) {
			for (int i = size() - 1; i >= 0; --i) {
				const auto [pattern, joy_type] = at(i);
				std::cerr << pattern << " " << to_string(joy_type) << "\n";
			}
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

	void joy_stack::dup() {
		if (!size() == 0) {
			push(top());
		}
	}

	void joy_stack::dupd() {
		if (!size() == 0) {
			dup();
			sat(1) = sat(2);
		}
	}

	void joy_stack::pop() {
		if (!size() == 0) {
			pop_back();
		}
	}

	void joy_stack::popd() {
		if (size() >= 2) {
			sat(1) = sat(0);
			pop();
		}
	}

	void joy_stack::pop2() {
		if (size() >= 2) {
			pop();
			pop();
		}
	}

	void joy_stack::swap() {
		std::swap(sat(0), sat(1));
	}

	void joy_stack::swapd() {
		std::swap(sat(1), sat(2));
	}

	void joy_stack::rotate() {
		std::swap(sat(0), sat(2));
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
		if (!size() == 0) {
			return back();
		}
	}

	base_stack_t::const_reference joy_stack::top() const {
		if (!size() == 0) {
			return back();
		}
	}


}