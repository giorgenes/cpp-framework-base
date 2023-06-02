#include <libany/rcpp/session.h>
#include <stdexcept>
#include <string.h>
	
using namespace ::libany::rcpp;

void RcppStream::commit()
{
	try {
		::libany::bxtp::Document doc(*this);
		doc.begin("request");
		doc.write_tag("commit");
		doc.end();
		doc.match_end();
	}
	catch(...) {
		throw;
	}

}

void RcppStream::rollback()
{
	try {
		::libany::bxtp::Document doc(*this);
		doc.begin("request");
		doc.write_tag("rollback");
		doc.end();
		doc.match_end();
	}
	catch(...) {
		throw;
	}
}


