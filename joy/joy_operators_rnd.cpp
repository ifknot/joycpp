#include "joy_operators_rnd.h"

namespace joy { 

	void rand(joy_stack& S) {
		S.push(make_token(static_cast<int>(__prng()), joy_t::int_t));
	}

	void srand(joy_stack& S) {
		__prng.seed(joy_cast<int>(S.top()));
		S.pop();
	}

}
