#pragma once

#include "joy_types.h"

namespace joy {

    token_t operator+(const token_t& a, const token_t& b);

    token_t operator-(const token_t& a, const token_t& b);

    token_t operator*(const token_t& a, const token_t& b);

    token_t operator/(const token_t& a, const token_t& b);

}
