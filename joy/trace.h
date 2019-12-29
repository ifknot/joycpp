#pragma once

#include <iostream>

#define NTRACE

#ifdef NTRACE 
#define TRACE 0 && std::cerr
#else
#define TRACE std::cerr 
#endif 