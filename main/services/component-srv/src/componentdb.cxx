#include "componentdb.h"
#include <stdexcept>
#include <libany/utils/string_utils.h>
#include <libany/dbsrvcli/db.h>
		

void ComponentDB::write_components(::libany::bxtp::ODocument& doc)
{
	::libany::dbsrvcli::DB db(stream(), "default");

	_db->strfile().loadstr("get_component_names");
	::libany::table::Table table;
	db.nexec(_db->strfile().str(), table);
	
	for(int i = 0; i<table.ntuples(); i++) {
		doc.write_tag("component", table.field_value(i, 0));
	}
}

ComponentDB::~ComponentDB()
{
}

