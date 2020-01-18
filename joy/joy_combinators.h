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
#pragma once

#include "joy_stack.h"

namespace joy {

	

}
