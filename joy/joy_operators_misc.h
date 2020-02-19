#pragma once

#include "joy_stack.h"
#include "io_device.h"

namespace joy {

    void at(joy_stack& S);

    /**
    * manual: ->
    * Writes the loaded manual of all Joy primitives to an io device.
    */
    void manual(std::map<std::string, std::string>& joy_manual, io_device& io);

    /**
    * helpdetail: [ S1 S2 .. ]
    * Gives brief help on each symbol S in the list.
    */
    void helpdetail(const joy_stack& S, std::map<std::string, std::string>& joy_manual, io_device& io);

}