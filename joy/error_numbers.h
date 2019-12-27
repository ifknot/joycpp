#pragma once

namespace joy {

	using error_number_t = size_t;

	const size_t DSTACKEMPTY = 0;
	const size_t DNOCONVERSION	= 1;
	const size_t DWRONGTYPE		= 2;
	const size_t DFILENOTFOUND	= 3;
	const size_t DMANNOTFOUND	= 4;
	const size_t DNOMATCH		= 5;

}