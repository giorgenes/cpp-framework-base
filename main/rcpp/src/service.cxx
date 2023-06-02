#include "libany/rcpp/service.h"
#include "libany/rcpp/session.h"
#include <libany/stdstream/complete.h>
#include <string.h>
#include <stdlib.h>
#include <stdexcept>

namespace impl = ::libany::rcpp;

void impl::Transaction::begin()
{
}

void impl::Transaction::begin_exec(const char*, const char*)
{
}

void impl::Transaction::key(::libany::bxtp::Document&)
{
}

void impl::Transaction::data(::libany::bxtp::Document&)
{
}

void impl::Transaction::exec(::libany::bxtp::Document&)
{
}

void impl::Transaction::nexec(::libany::bxtp::Document&)
{
}


impl::ClientHandler::~ClientHandler()
{
}

impl::Service::~Service()
{
}

void 
impl::Service::create_trans(impl::ClientHandler* cli, impl::Transaction*& trn)
{
	if(!trn) {
		trn = cli->new_transaction();
		trn->begin();
	}
}

void impl::Service::exec(
		impl::Transaction* trans, ::libany::bxtp::Document& doc, bool atomic)
{
	try {
		doc.match("/request/service");
		char service[1024];
		int len = doc.read(service, sizeof(service)-1);
		service[len] = 0;
		
		doc.match("/request/base");
		char base[1024];
		len = doc.read(base, sizeof(base)-1);
		base[len] = 0;

		trans->begin_exec(service, base);
		
		doc.match("/request/key");
		{
			::libany::bxtp::Document idoc(doc);
			trans->key(idoc);
		}
		
		doc.match("/request/data");
		{
			::libany::bxtp::Document idoc(doc);
			trans->data(idoc);
		}

		doc.match_end();

		if(atomic) {
			trans->nexec(doc);
		} else {
			trans->exec(doc);
		}
	}
	catch(...) {
		throw;
	}
}

void impl::Service::handle_client(::libany::stream::Stream* p_clistm)
{
	ClientHandler *cli = new_client();
	if(cli == 0) {
		throw std::runtime_error("could not allocate client");
	}
	Transaction *trans = 0;
	::libany::bxtp::BxtpStream sax(*p_clistm);
	bool hasdoc;
	bool began = false;
	do {
		try {
			::libany::bxtp::Document doc(sax);
			if((hasdoc = doc.match("/request"))) {
				const char * opts[] = {
					"/request/nquery", 
					"/request/query", 
					"/request/commit", 
					"/request/rollback", 
					0
				};

				int opt;
				if(!doc.match(opts, &opt)) {
					throw std::runtime_error("invalid request");
				}

				create_trans(cli, trans);

				doc.begin("response"); began = true;
				try {
					switch(opt) {
						case 0:
							exec(trans, doc, true);
							break;
						case 1:
							exec(trans, doc, false);
							break;
						case 2:
							trans->commit();
							break;
						case 3:
							trans->rollback();
							break;
					}
					doc.match_end();
				}
				catch(std::exception& e) {
					doc.write_tag("exception", e.what());
					fprintf(stderr, "server: exception: %s\n", e.what());
				}
				doc.end();
			}
		}
		catch(std::exception& e) {
			fprintf(stderr, "server: exception: %s\n", e.what());
		}
	}while(hasdoc);
	
	if(trans) {
		delete trans;
	}
	//_cli->end();
	delete cli;
}

