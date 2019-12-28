#pragma once

#include <iostream>

#ifdef NTRACE 
#define TRACE 0 && std::cerr
#else
#define TRACE std::cerr 
#endif 