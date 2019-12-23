/**
* Joy stack is not simply a sequence of values but also contains operators and combinators. 
* So, strictly speaking the stack is always a quotation and can contain all of the Joy types.
*
* The joylang stack list of supported operators: 
* (Those marked "?" could implemented in Joy)
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
* 3 further stack operators seem central to how Joy works:
*
* newstack	- deletes the entire stack and replaces it with a new, empty one
* stack		- pushes onto the stack a list containing all the elements of the stack
* unstack	- expects a quotation on the stack and makes that the new stack
*/
#pragma once

namespace joy {

	typedef joy_stack joy_stack_t;

	class joy_stack {



	};
}

