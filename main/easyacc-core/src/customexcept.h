#ifndef __custom_except_h
#define __custom_except_h

#include <stdexcept>

class CmdError : public std::invalid_argument {
	public:
		CmdError(const char* p) : std::invalid_argument(p) {
		};
};

#endif
