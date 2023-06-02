#include "db.h"
#include <stdexcept>
#include <cstring>

namespace impl = ::libany::componentfsdbsrv;

void impl::DB::commit()
{
}

impl::DB::DB()
{
	std::strcpy(__root, "/tmp/componentsrvdb");
}

impl::DB::~DB()
{
}

void impl::DB::begin_transaction()
{
}

