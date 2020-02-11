#pragma once

#include "error_numbers.h"
#include "error_messages.h"

namespace joy {
	 /**
	 * Report a Joy specific error via std::cerr 
	 * (Uses terminal colours if enabled)
	 */
	bool error(size_t message, std::string name);

}