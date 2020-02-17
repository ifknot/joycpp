#pragma once

#include <random>

#include "joy_stack.h"

// random number operators by compulsion 
// also reveal the influence of C99 on some of the JOY operators 
// C rand is very poor source of entropy but... 
// there is a nice simple replacement for rand() and srand() in the C++11 random library: std::minstd_rand.

namespace joy {

	/**
	* Minimal Standard minstd_rand generator
	* A simple multiplicative congruential pseudo-random number generator (a type of linear_congruential_engine).
	* @note the algorithm restricts the result to between 1 and 231-2, inclusive. 
	* This means the result will always convert safely to a std::int_fast32_t (or to an int if int is at least 32 bits long).
	*/
	static std::minstd_rand __prng;

	/**
	rand:->I
	I is a random integer.
	*/
	void rand(joy_stack& S);

	/**
	srand: I ->
	Sets the random integer seed to integer I .
	*/
	void srand(joy_stack& S);

}