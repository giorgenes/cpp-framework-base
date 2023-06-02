#include "db.h"
#include <stdexcept>

namespace impl = postgresrv_impl;

void impl::PostgreDB::commit()
{
	PGresult *res;
	res = PQexec(_conn, "COMMIT");
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		fprintf(stderr, "COMMIT command failed: %s", PQerrorMessage(_conn));
		PQclear(res);
		throw std::runtime_error("COMMIT failed");
	}
	PQclear(res);
}

impl::PostgreDB::PostgreDB()
{
	_conn = PQsetdbLogin(
			NULL,
			NULL,
			NULL,
			NULL,
			"test",
			NULL,
			NULL);

	if (PQstatus(_conn) != CONNECTION_OK) {
		fprintf(stderr, "Connection to database failed: %s\n",
				PQerrorMessage(_conn));
		throw std::runtime_error("connection to db failed");
	}
}

impl::PostgreDB::~PostgreDB()
{
}


void impl::PostgreDB::begin_transaction()
{
	PGresult *res;
	res = PQexec(_conn, "BEGIN");
	if (PQresultStatus(res) != PGRES_COMMAND_OK)
	{
		fprintf(stderr, "BEGIN command failed: %s\n", PQerrorMessage(_conn));
		PQclear(res);
		throw std::runtime_error("BEGIN failed");
	}
	PQclear(res);
	fprintf(stderr, "connected\n");
}

