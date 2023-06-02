#include "except.h"
#include <errno.h>

namespace except = ::libany::except;

except::Errno::Errno(int e)
	: __errno(e)
{
	strcpy(__msg, strerror(__errno));
}


except::Errno::Errno()
	: __errno(errno)
{
	strcpy(__msg, strerror(__errno));
}

const char* except::Errno::what() const throw()
{
	return __msg;
}

