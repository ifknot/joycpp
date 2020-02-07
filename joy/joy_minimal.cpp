#include "joy_minimal.h"
#include <iterator>

namespace joy {

    void uncons(joy_stack& S) {
        auto A = joy_cast<joy_stack>(S.top());
        if (A.size()) {
            auto t = S.top().second;
            S.pop();
            joy_stack R(A.begin() + 1, A.end());
            auto F = *A.begin();
            S.push(F);
            S.push(make_token(R, t));
        }
    }

    void choice(joy_stack& S) {
        auto F = S.top();
        S.pop();
        auto T = S.top();
        S.pop();
        auto B = S.top();
        S.pop();
        if (std::any_cast<bool>(B)) {
            S.push(T);
        }
        else {
            S.push(F);
        }
    }

}
