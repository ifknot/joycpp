#pragma once

/**
* It is useful to classify Joy atomic programs into categories depending on what kind of function they denote.
* A coarse classification distinguishes just three :
* 1. literals
* 2. the operators
* 3. the combinators.
*/

#include "error.h"

#include "joy_types.h"

#include "joy_stack.h"
#include "joy_overloads.h"
#include "joy_operators.h"
#include "joy_operators_io.h"
#include "joy_operators_math.h"
#include "joy_operators_misc.h"

#include "joy_minimal.h"
#include "joy_combinators.h"