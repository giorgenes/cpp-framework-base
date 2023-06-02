#include "transaction.h"
#include <stdio.h>
#include <stdexcept>
#include <libany/table/table.h>

namespace pqsrv = postgresrv_impl;

void pqsrv::Transaction::commit()
{
	_db->commit();
}

void pqsrv::Transaction::rollback()
{
	fprintf(stderr, "rollback\n");
}

void pqsrv::Transaction::begin()
{
	_db->begin_transaction();
}

void pqsrv::Transaction::exec(::libany::bxtp::Document& doc)
{
	int len;
	char buf[1024];
	PGresult *res;
	fprintf(stderr, "executando %s\n", __query);
	int restatus;
	
	res = PQexec(_db->_conn, __query);
	restatus = PQresultStatus(res);
	if(restatus == PGRES_COMMAND_OK) {
		PQclear(res);
		return;
	}
	
	if (restatus != PGRES_TUPLES_OK) {
		len = sprintf(buf, "PQexec failed: %s", PQerrorMessage(_db->_conn));
		PQclear(res);
		throw std::runtime_error(buf);
	}

	doc.begin("table");
	int i;
	int nFields = PQnfields(res);
	int ntuples = PQntuples(res);
	::libany::table::TableWriter table(doc, nFields, ntuples);
	for (i = 0; i < nFields; i++) {
		table.write_field_name(PQfname(res, i));
	}

	/* next, print out the rows */
	int j;
	for (i = 0; i < ntuples; i++)
	{
		for (j = 0; j < nFields; j++) {
			//fprintf(stderr, "field: %s\n", PQgetvalue(res, i, j));
			table.write_field_value(PQgetvalue(res, i, j));
		}
	}

	doc.end();
	
	PQclear(res);
}

