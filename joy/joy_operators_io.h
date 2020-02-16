#pragma once

#include "joy_stack.h"
#include "io_device.h"

// IO operators core C++ performance 

namespace joy {
	/**
	* N ->
	* N : numeric, writes character whose ASCII is N.
	*/
	void putch(joy_stack& S, io_device& io);

	/**
	* Joy . operator
	* print top of stack S in left:right format to an io device
	*/
	void print_top(const joy_stack& S, io_device& io);

	/**
	* Joy .s operator
	* print stack S size and S contents in top:down format to an io device
	*/
	void print_stack(const joy_stack& S, io_device& io);

	/**
	* Joy get operator
	*/
	void input_stack(joy_stack& S, io_device& io);

}