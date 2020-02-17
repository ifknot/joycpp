#pragma once

#include "joy_stack.h"
#include "io_device.h"

// IO operators by compulsion also reveal the influence of C99 on some of the JOY operators 

namespace joy {

	/**
	* N ->
	* N : numeric, writes character whose ASCII is N.
	*/
	void putch(joy_stack& S, io_device& io);
	
	/**
	* strtol: S I->J
	* String S is converted to the integer J using base I.If I = 0, assumes base 10, but leading "0" means base 8 and leading "0x" means base 16.
	*/
	//void strol(joy_stack& S, io_device& io);

	/**
	* strtod : S->R
	* String S is converted to the float R .
	*/
	//void strtod(joy_stack& S, io_device& io);

	/**
	* format : N C I J->S
	* S is the formatted version of N in mode C('d or ' i = decimal, 'o = octal, ' x or 'X = hex with lower or upper case letters) with maximum width I and minimum width J.
	*/
	//void format(joy_stack& S, io_device& io);

	/**
	* formatf : F C I J->S
	* S is the formatted version of F in mode C('e or ' E = exponential, 'f = fractional, ' g or G = general with lower or upper case letters) with maximum width Iand precision J.
	*/
	//void formatf(joy_stack& S, io_device& io);

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