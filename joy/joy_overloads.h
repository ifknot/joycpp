/**
* Joy token to C++ arithmetic operator overloading
*
* supported operators ──┐
*     top of stack ───┐ │          token overloads ──┐
*                     ↓ ↓                            ↓
*                   m n│+        →                m+n│
*                   m n│-        →                m-n│
*                   m n│*        →                m×n│
*                   m n│/        →                m÷n│
*                   m n│rem      →               ⌊m÷n│
*                   m n│div      →              0 n⊤m│
*                   m n│<        →                m<n│
*                   m n│<=       →                m≤n│
*                   m n│=        →                m=n│
*                   m n│>=       →                m≥n│
*                   m n│>        →                m>n│
*                   m n│!=       →                m≠n│
*                     b│not      →               {!⍵}│
*/
#pragma once

#include "joy_types.h"

namespace joy {

    //arithmetic token operators

    token_t operator+(const token_t& a, const token_t& b);

    token_t operator-(const token_t& a, const token_t& b);

    token_t operator*(const token_t& a, const token_t& b);

    token_t operator/(const token_t& a, const token_t& b);

    // rem  #include <math.h>

    // div #include <math.h>

    // relational token operators

    // <
    // <=
    // >
    // >=
    // == 
    // !=



}
