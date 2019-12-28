#pragma once

namespace joy {

	using error_number_t = size_t;

	constexpr size_t DSTACKEMPTY	= 0;
	constexpr size_t DNOCONVERSION	= 1;
	constexpr size_t DWRONGTYPE		= 2;
	constexpr size_t DFILENOTFOUND	= 3;
	constexpr size_t DMANNOTFOUND	= 4;
	constexpr size_t DNOMATCH		= 5;
	constexpr size_t DLESSARGS		= 6;

}