#ifndef UTIL_HEADER
#define UTIL_HEADER

#include "life.hpp"

//sobrecarga de operador << para impressao em stream
std::ostream& operator<< (std::ostream& os, const Life& life);

#endif
