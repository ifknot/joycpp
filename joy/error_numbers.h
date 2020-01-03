#pragma once

namespace joy {

	using error_number_t = size_t;

	constexpr size_t DJOYERROR		= 0;
	constexpr size_t DSTACKEMPTY	= 1;
	constexpr size_t DNOCONVERSION	= 2;
	constexpr size_t DWRONGTYPE		= 3;
	constexpr size_t DFILENOTFOUND	= 4;
	constexpr size_t DMANNOTFOUND	= 5;
	constexpr size_t DNOMATCH		= 6;
	constexpr size_t DLESSARGS		= 7;
	constexpr size_t DAGGNOOPEN		= 8;
	constexpr size_t DQUOTESET		= 9;
	constexpr size_t DPRECISION		= 10;

}