#pragma once

#include "joy_stack.h"

/**
* Joy03 Combinators Library
*
* It is useful to classify Joy atomic programs into categories depending on what kind of function they denote.
* A coarse classification distinguishes just three:
* 1. literals
* 2. the operators
* 3. the combinators.
*
* Combinator atoms require quoted programs atop the stack.
* Combinators also denote functions which are defined only on states having the appropriate number of quoted programs on top of the stack.
* They yield as values another state which depends on the argument state, including the quoted programs, and on the combinator itself.
*/
namespace joy {

	/**
	* Joy has certain devices called combinators which cause the execution of quoted programs that are on top of the stack.
	* One of the simplest is the i combinator.
	* Its effect is to execute a single program on top of the stack, and nothing else.
	* Syntactically speaking, its effect is to remove the quoting square brackets and thus to expose the quoted program for execution.
	*
	* [a]│i   →  │a
	* i: [P] -> ...
	* Executes P. So, [P] i == P
	*/
	template<typename parser_t>
	void i(joy_stack& S, parser_t& parse) {
		auto P = joy_cast<joy_stack&>(S.top());
		S.pop();
		parse(P, S);
	}

	/**
	* The dip combinator expects a quotation parameter (which it will consume), and below that one more element.
	* It saves that element away, executes the quotation on whatever of the stack is left, and then restores the saved element.
	* So 2 3 4 [+] dip is the same as 5 4.
	* This single combinator was inspired by several special purpose optimizing combinators S', B' and C' in the combinatory calculus, see Peyton Jones (1987, sections 16.2.5 and 16.2.6).
	*
	* a [f]│dip      →       │f a
	* dip: X [P] -> ... X
	* Saves X, executes P, pushes X back onto stack.
	*/
	template<typename parser_t>
	void dip(joy_stack& S, parser_t& parse) {
		S.swap(); //flip [P] and X
		const auto X = S.top(); //save token
		S.pop();
		i(S, parse); //execute [P]
		S.push(X); //restore X
	}

	/**
	* The times unary combinator expects a numeric value below its quotation parameter and executes its quotation parameter as many times as indicated by the numeric value.
	* If the value is zero or less, then the quotation is not executed at all.
	*
	* n [ops]│times    →              ops⍣n│
	* times : N[P] ->...
	* N times executes P.
	*/
	template<typename parser_t>
	void times(joy_stack& S, parser_t& parse) {
		auto P = joy_cast<joy_stack&>(S.top()); //get the program
		S.pop();
		auto n = joy_cast<int>(S.top()); //get n
		S.pop();
		while (n--) {
			parse(P, S);
		}
	}

	/**
	* The step combinator can be used to access all elements of an aggregate in sequence.
	* For strings and lists this means the order of their occurrence, for sets it means the underlying order.
	* The following will step through the members of the second list and swons them into the initially empty first list.
	* []  [2 8 3 6 5]  [swons]  step
	* The effect is to reverse the non-empty list, yielding [5 6 3 8 2].
	* 
	* [a b ...] [p]│step     →     │a p b p ...
	* step: A [P] -> ...
	* Sequentially putting members of aggregate A onto a stack M
	* parsing M that executes P for each member of A stack the result
	*/
	template<typename parser_t>
	void step(joy_stack& S, parser_t& parse) {
		auto P = joy_cast<joy_stack&>(S.top()); //get the program
		S.pop();
		auto A = joy_cast<joy_stack&>(S.top()); //get the aggregate
		S.pop();
		joy_stack M;
		for (auto a : A) {
			M.push(a);
			for (auto p : P) {
				M.push(p);
			}
		}
		parse(M, S);
	}

	/**
	* The map combinator expects an aggregate value on top of the stack, and it yields another aggregate of the same size.
	* The elements of the new aggregate are computed by applying the quoted program to each element of the original aggregate.
	* [a b ...] [f]│map     →       [f¨a b ...]│
	* map: A [P] -> B
	* Executes P on each member of aggregate A, collects results in same type aggregate B.
	*/
	template<typename parser_t>
	void map(joy_stack& S, parser_t& parse) {
		auto sz = size(S.sat(1)); //aggregate size
		auto type = S.sat(1).second; //get the return aggregate type
		step(S, parse);
		joy_stack M; //fresh stack to work with
		while (sz--) { //copy off the results of step
			M.push(S.top());
			S.pop();
		}
		M.stack(); //convert the stack to a list [N]
		M.top().second = type;
		S.push(M.top());  //push [N] as the result
	}

	/**
	* The genrec combinator takes four program parameters in addition to whatever data parameters it needs. 
	* Fourth from the top is an if-part, followed by a then-part. 
	* If the if-part yields true, then the then-part is executed and the combinator terminates. 
	* The other two parameters are the rec1-part and the rec2part. 
	* If the if-part yields false, the rec1-part is executed. 
	* Following that the four program parameters and the combinator are again pushed onto the stack bundled up in a quoted form. 
	* Then the rec2-part is executed, where it will find the bundled form. 
	* Typically it will then execute the bundled form, either with i or with app2, or some other combinator.
	*
	* genrec : [B][T][R1][R2] ->...
	* Executes B, if that yields true executes T. Else executes R1 and then [[B][T][R1][R2] genrec] R2.
	*/
	template<typename parser_t>
	void genrec(joy_stack& S, parser_t& parse) {
		auto R2 = S.top();
		S.pop();
		auto R1 = S.top();
		S.pop();
		auto T = S.top();
		S.pop();
		auto B = S.top();
		i(S, parse);			// Executes B,
		assert(S.top().second == joy_t::bool_t);
		if (!joy_cast<bool>((S.top()))) {
			S.pop();
			S.push(R1);			// Else executes R1 and then...
			i(S, parse);  
			{					// [[B][T][R1][R2] genrec]
				joy_stack M;	// temporary stack to build [[B][T][R1][R2] genrec]
				M.push(make_token("genrec"s, joy_t::cmd_t));
				M.push(R2);
				M.push(R1);
				M.push(T);
				M.push(B);
				M.stack(); 
				S.push(M.top()); // push quoted form
			}
			S.push(R2);
			i(S, parse);		// R2
		}
		else {					// If B yields true,  
			S.pop();			//
			S.push(T);			//
			i(S, parse);		// executes T and terminates.
		}
	}

	/**
	* The linrec combinator also takes four program parameters and is otherwise very similar to the genrec combinator. 
	* The essential difference is that the bundled up quotation is immediately called before the rec2-part. 
	* Consequently it can only be used for linear recursion e.g.
	*
	* l-fac == [null][succ][dup pred][*] linrec;
	*
	* linrec : [B][T][R1][R2] ->...
	* Executes B. If that yields true, executes T. Else executes R1, recurses, executes R2.
	*/
	template<typename parser_t>
	void linrec(joy_stack& S, parser_t& parse) {
		auto R2 = S.top();
		S.pop();
		auto R1 = S.top();
		S.pop();
		auto T = S.top();
		S.pop();
		auto B = S.top();
		i(S, parse);			// Executes B.
		assert(S.top().second == joy_t::bool_t);
		if (!joy_cast<bool>((S.top()))) {
			S.pop();
			S.push(R1);
			i(S, parse);		// Else executes R1,
			S.push(B);
			S.push(T);
			S.push(R1);
			S.push(R2);
			linrec(S, parse);	// recurses,
			S.push(R2);			//
			i(S, parse);		// executes R2
		}
		else {					// If B yields true,   
			S.pop();			//
			S.push(T);			//
			i(S, parse);		// executes T and terminates.
		}
	}

	/**
	* The binrec combinator is again similar, but it applies the bundled quotation twice.
	* Once to each of the two top values which the R1 part has left on the stack. 
	* It implements binary recursion e.g.
	*
	* b-fib == [small][id][pred dup pred][+] binrec;
	*
	* binrec : [B][T][R1][R2] ->...
	* Executes B. If that yields true, executes T. 
	* Else uses R1 to produce two intermediates, recurses on both, 
	* then executes R2 to combine their results.
	*/
	template<typename parser_t>
	void binrec(joy_stack& S, parser_t& parse) {
		auto R2 = S.top();
		S.pop();
		auto R1 = S.top();
		S.pop();
		auto T = S.top();
		S.pop();
		auto B = S.top();
		i(S, parse);			// Executes B.
		assert(S.top().second == joy_t::bool_t);
		if (!joy_cast<bool>((S.top()))) {
			S.pop();
			S.push(R1);
			i(S, parse);		// Else uses R1 to produce two intermediates,
			S.swap();
			S.push(B);
			S.push(T);
			S.push(R1);
			S.push(R2);
			binrec(S, parse);	// recurse on first item left by R1
			S.swap();
			S.push(B);
			S.push(T);
			S.push(R1);
			S.push(R2);
			binrec(S, parse);	// recurse on second item left by R1
			S.swap();
			S.push(R2);			
			i(S, parse);		// then executes R2 to combine their results.
		}
		else {					// If B yields true,   
			S.pop();			//
			S.push(T);			//
			i(S, parse);		// executes T and terminates.
		}
	}

	/**
	* The tailrec combinator is similar to the linrec combinator, except that it does not have an R2 part. 
	* It can only be used for tail recursion, such as in the program below which returns the last element of an aggregate.
    * 
	* t-last == [small] [first] [rest] tailrec;
	*
	* tailrec : [B][T][R1] ->...
	* Executes B. If that yields true, executes T. Else executes R1, recurses.
	*/
	template<typename parser_t>
	void tailrec(joy_stack& S, parser_t& parse) {
		auto R1 = S.top();
		S.pop();
		auto T = S.top();
		S.pop();
		auto B = S.top();
		i(S, parse);			// Executes B.
		assert(S.top().second == joy_t::bool_t);
		if (!joy_cast<bool>((S.top()))) {
			S.pop();
			S.push(R1);
			i(S, parse);		// Else uses R1,
			S.push(B);
			S.push(T);
			S.push(R1);
			tailrec(S, parse);	// recurse.
		}
		else {					// If B yields true,   
			S.pop();			//
			S.push(T);			//
			i(S, parse);		// executes T and terminates.
		}
	}
}
