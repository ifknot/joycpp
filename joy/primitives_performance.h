/**
* For many operators an implementation can choose whether to make it a Joy03 primitive or define it in a library.
* Apart from execution speed, to the user it makes no difference as to which choice has been made.
* Those benefiting from performance improvements are defined here.
*
* performance operators ──┐
*       top of stack ───┐ │
*                       ↓ ↓
*           [a b ...] [f]│step     →                   │a f b f ...
*			   	  [a b c]│reverse  →            [c b a]│
*                     [a]│i        →                   │a
*/
#pragma once

#include "joy_types.h"
#include "joy_stack.h"

namespace joy {

    /**
    * [a b c]│reverse  →            [c b a]│
    */
    void reverse(joy_stack& stack);

}