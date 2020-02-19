#include "joy_operators_misc.h"

namespace joy {
//at: A I->X
//X(= A[I]) is the member of A at position I .
    void at(joy_stack& S) {
        auto I = joy_cast<int>(S.top());
        S.pop();
        auto A = joy_cast<joy_stack>(S.top());
        //S.pop();
        S.push(A[I]);
    }

    void manual(std::map<std::string, std::string>& joy_manual, io_device& io) {
        io.colour(YELLOW);
        for (const auto& [cmd, info] : joy_manual) {
            io << cmd << info;
        }
    }

    void helpdetail(const joy_stack& S, std::map<std::string, std::string>& joy_manual, io_device& io) {
        io.colour(YELLOW);
        for (const auto& [command, type] : S) {
            assert(type == joy_t::lambda_t);
            auto match = std::any_cast<std::string>(command);
            auto it = joy_manual.find(match);
            if (it != joy_manual.end()) {
                io << match + joy_manual[match];
            }
        }

    }

}
