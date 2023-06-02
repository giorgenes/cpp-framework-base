#include "libany/rcpp/service.h"
#include "libany/rcpp/session.h"
#include <libany/stdstream/complete.h>
#include <string.h>
#include <stdlib.h>
#include <stdexcept>

using namespace ::libany::rcpp;

Transaction::~Transaction()
{
}

Client::~Client()
{
}

void Service::handle_client(::libany::stream::IOStream* p_clistm)
{
	ServerSession session(p_clistm);
	PostL1 pcmd(&session);
		
	char cmd;
	Client* cli = new_client();
	if(cli == 0) {
		throw std::runtime_error("could not allocate client");
	}
	cli->begin();
	
	char buf[1024];
	int len;
	while(1) {
		pcmd.recv(&cmd);
		fprintf(stderr, "%c recebido\n", cmd);
		while(pcmd.next_group()) {
			fprintf(stderr, "tenho um grupo\n");
			while(pcmd.next_field()) {
				fprintf(stderr, "tenho um campo\n");
				while(!pcmd.eos()) {
					fprintf(stderr, "tenho dados\n");
					len = pcmd.read(buf, sizeof(buf));
					buf[len] = 0;
					fprintf(stderr, "buf: %s\n", buf);
				}
			}
		}
		fprintf(stderr, "acabaram-se os grupos\n");
		switch(cmd) {
			case 'q':
				break;
			case 'Q':
				break;
			case 'C':
				break;
			case 'R':
				break;
		}
	}
	
	cli->end();
	delete cli;
}

