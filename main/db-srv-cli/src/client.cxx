#include "db.h"
#include <stdexcept>
#include <libany/stfactory/uristream.h>
#include <memory>
#include <libany/utils/path_utils.h>

using namespace ::libany::dbsrvcli;

DB::DB(::libany::rcpp::RcppStream& r, 
		const char* base)
	: ::libany::rcpp::Client(r)
{
	strcpy(_db, base);
}

void DB::exec(const char* query, ::libany::table::Table& table)
try
{
	::libany::rcpp::RcppDocument cmd(_rcppstream);

	cmd.request_begin_query("component", _db);
	cmd.request_key("base", _db);
	cmd.request_data("query", query);
	cmd.request_end();

	if(!cmd.match("/response/table")) return;

	table.build_from_stream(cmd);

	cmd.match_end();
}
catch(...) {
	throw;
}

void DB::exec(const char* query)
{
	::libany::rcpp::RcppDocument cmd(_rcppstream);

	cmd.request_begin_query("component", _db);
	cmd.request_key("base", _db);
	cmd.request_data("query", query);
	cmd.request_end();

	if(!cmd.match("/response/blob")) return;

	cmd.match_end();
}

void DB::nexec(const char* query, ::libany::table::Table& table)
try
{
	::libany::rcpp::RcppDocument cmd(_rcppstream);

	cmd.request_begin_nquery("component", _db);
	cmd.request_key("base", _db);
	cmd.request_data("query", query);
	cmd.request_end();

	if(!cmd.match("/response/table")) return;

	table.build_from_stream(cmd);

	cmd.match_end();
}
catch(...) {
	throw;
}

