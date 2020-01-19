/**
* A Joy stack is not simply a sequence of values but also contains operators and combinators.
* So, strictly speaking the stack is always a quotation and can contain all of the Joy types.
* The stack is also central to arguement conformability checking with the has(...) member function
*
* Stack Operators are those atoms denoting structural functions of the stack component.
* For performance the general unary operators are defined for stacks containing at least one element:
*
*        pop     dup
*  
* The top element does not have to satisfy any particular condition, it can be of any_t. 
* The pop operator removes the top element. 
* The dup operator pushes a duplicate on top, so it replaces the one original by two copies.
* 
* The following binary operators are defined for stacks containing at least two elements:
*
*        swap    popd    popop    dupd
*  
* The swap operator interchanges the top two elements. 
* The popd operator removes the second element. 
* The pop2 operator removes the first and the second element. 
* The dupd operator duplicates the second element.
*
* The following ternary operators are defined for stack stacks containing at least three elements:
*
*       swapd    rollup    rolldown
*  
* The swapd operator interchanges the second and third elements but leaves the first element in place. 
* The rollup operator moves the third and second element into second and third position and moves the first element into third position. 
* The rolldown operator moves the second and first element into third and second position and moves the third element into first position.
*
* Thus, the joy_stack class list of supported operators:
* (Those marked "?" could implemented in Joy but for performance and convenience are implemented as joy stack memeber functions)
*
* supported operators ──┐
*     top of stack ───┐ │
*                     ↓ ↓
*                     a│dup      →                a a│
* ?                 b a│dupd     →              b b a│
*                   b a│pop      →                  b│
* ?                 b a│popd     →                  a│
* ?               c b a│pop2     →                  c│
* ?                   a│unit     →                [a]│
*                   b a│swap     →                a b│
* ?               c b a│swapd    →              b c a│
* ?               c b a│rotate   →              a b c│
* ?               c b a│rollup   →              a c b│
* ?               c b a│rolldown →              b a c│
*
* 3 further stack operators are central to how Joy works:
*
* newstack	- deletes the entire stack and replaces it with a new, empty one
* stack		- pushes onto the stack a list containing all the elements of the stack
* unstack	- expects a quotation on the stack and makes that the new stack
*
*/
#pragma once

#include <vector>
#include <algorithm>
#include <cassert>

#include "joy_types.h"

namespace joy {

	/**
	* The joy stack does most of the heavy lifting in joycpp 
	* Extending std::vector with joy stack manipulators because std::stack hides array access with [] and at ()
	* Also get the vector member types
	* Maintain the std::stack ADT idioms
	*/
	struct joy_stack : public  std::vector<token_t> {

		using vector::vector; // get all the vector constructors for free

		/**
		* argument conformability checking
		* test stack has the requirments presented in the initializer list
		*/
		bool has(const std::string& op, const prerequisite_t& argt);

		/**
		* ..b a│stack    →    │
		* deletes the entire stack and replaces it with a new, empty one
		*/
		void newstack();

		/**
		* ..b a│stack    →    ..b a [a b ..]│
		* pushes onto the stack a quote containing all the elements of the stack
		*/
		void stack();

		/**
		* ..[a b]│unstack  →                b a│
		* expects a quotation on the stack and makes that the new stack
		*/
		void unstack();

		/**
		* a| dup ->		aa|
		* Duplicates item atop stack
		*/
		void dup();

		/**
		* ba| dupd ->	bba|
		* Duplicates an item immediately below the topmost item on the stack.
		*/
		void dupd();

		/**
		* ba| pop ->		b|
		* Removes topmost item from stack
		*/
		void pop();

		/**
		* ba| popd ->	a|
		* Removes the second-to-topmost item from stack
		*/
		void popd();

		/**
		* cba| pop2 ->	c|
		* Removes topmost 2 items from stack
		*/
		void pop2();

		/**
		* a│unit ->  [a]│
		* Quotes top
		*/
		void unit();

		/**
		* ba| swap ->	ab|
		* Swaps the two items at the top of a stack
		*/
		void swap();

		/**
		* cba| swapd ->	bca|
		* Swaps the two items immediately beneath the top of a stack
		*/
		void swapd();

		/**
		* cba| rotate ->	abc|
		* Permutes the order of the top three items on the stack.
		* Swapping the 1st and 3rd items ie 'rotating' them around the 2nd item
		*/
		void rotate();

		/**
		* cba| rollup ->	acb|
		* Permutes the order of the top three items on the stack.
		* Shifting them up and moving the topmost item down below the other two.
		* Equivalent to swap(), swapd()
		*/
		void rollup();

		/**
		* cba| rolldown -> bac|
		* Permutes the order of the top 3 items on the stack.
		* Shifting them down and carrying the deepest item up to the top of the stack.
		* Equivalent to rotate(), swapd()
		*/
		void rolldown();

		/**
		*  Const reference item at a specified position in the stack
		*/
		const_reference sat(size_t i) const;

		/**
		*  Reference item at a specified right to left stack position in the vector
		* (as opposed to at() which specifies position left - right vector position)
		*/
		reference sat(size_t i);

		// expected STL stack interface

		const_reference top() const;

		reference top();

		void push(const value_type& val);

		void push(value_type&& val);

	};
}
