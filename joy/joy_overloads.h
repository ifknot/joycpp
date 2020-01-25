#pragma once

#include "joy_types.h"

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
namespace joy {

    //unary token overloads

    /**
    * ord : C -> I
    * Integer I is the ASCII value of character C( or logical or integer).
    * TODO: logical [] integer []
    */
    token_t ord(token_t token);

    /**
    * chr : I -> C
    * C is the character whose ASCII value is integer I( or logical or character).
    * TODO: logical [] integer []
    */
    token_t chr(token_t token);

    /**
    * char : X -> B
    * Tests whether X is a character.
    */
    token_t is_char(token_t token);

    //arithmetic token overloads

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

    // TODO: fix for booleans
    token_t operator==(const token_t& a, const token_t& b);

    bool operator==(const token_t& a, const std::string& b);

    // !=



}
