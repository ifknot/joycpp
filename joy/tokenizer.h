#pragma once

#include "io_device.h"
#include "joy_types.h"

namespace joy {

	class tokenizer {
	
	public:
	
		tokenizer(io_device& io);

		void tokenize(std::string line);

	protected:

		io_device& io;

	};

}
