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
*                   m n│<        →                m<n│
*                   m n│<=       →                m≤n│
*                   m n│=        →                m=n│
*                   m n│>=       →                m≥n│
*                   m n│>        →                m>n│
*                   m n│!=       →                m≠n│
*/
namespace joy {

    //arithmetic token overloads

    token_t operator+(const token_t& a, const token_t& b);

    token_t operator-(const token_t& a, const token_t& b);

    token_t operator*(const token_t& a, const token_t& b);

    token_t operator/(const token_t& a, const token_t& b);

    // relational token operators

    token_t operator < (const token_t& a, const token_t& b);

    token_t operator <= (const token_t& a, const token_t& b);

    token_t operator > (const token_t& a, const token_t& b);

    token_t operator >= (const token_t& a, const token_t& b);

    token_t operator != (const token_t& a, const token_t& b);

    token_t operator == (const token_t& a, const token_t& b);

    /**
    * syntactic sugar compare a string to the pattern part of a token 
    */
    bool operator == (const token_t& a, const std::string& b);

}
