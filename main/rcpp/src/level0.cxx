#include <libany/rcpp/session.h>
#include <stdexcept>
	
using namespace ::libany::rcpp;

PostL0::PostL0(Session* session) 
	: _session(session) 
{
	_buf[0] = 0;
}
				

void PostL0::send_header(char type, char param)
{
	char head[2];

	try {
		head[0] = type;
		head[1] = param;
		_session->stm()->write(head, 2);
	}
	catch(...) {
		throw;
	}

}

void PostL0::recv_header()
{
	try {
		if(_buf[0] == 0) {
			if(_session->stm()->read(_buf, 2) <= 0) {
				throw std::runtime_error("connection broken");
			}
			fprintf(stderr, "recebido: %d(%c) %d(%c)\n", 
					_buf[0], _buf[0], _buf[1], _buf[1]);
		}
	}
	catch(...) {
		throw;
	}
}


/* FIXME: optimize this
 * to make a buffered write */
int PostL0::write(const void* buf, int size)
{
	try {
		unsigned char pks;

		while(size > 0) {
			pks = size > 256 ? 256 : size;
			send_header('S', pks);
			_session->stm()->write(buf, (int)pks);
			buf = (const char*)buf + (int)pks;
			size -= pks;
		}
	}
	catch(...) {
		throw;
	}

	return 0;
}

int PostL0::read(void* buf, int size)
{
	int tr;
	int b = 0;
	int ret;

	if(_buf[0] != 'S' && _buf[0] != 0) {
		return 0;
	}

	while(size > 0) {
		if(_buf[0] == 'S') {
			tr = _buf[1] > size ? size : _buf[1];
			fprintf(stderr, "tenho um S, lendo %d bytes\n", tr);
			if((ret = _session->stm()->read(buf, tr)) <= 0) {
				break;
			}
			((char*)buf)[ret] = 0;
			fprintf(stderr, "recebi isso: %s (%d)\n", buf, ret);
			b += ret;
			_buf[1] -= ret;

			if(_buf[1] == 0) {
				consume_buffer();
			}
		}
		else {
			fprintf(stderr, "NAO tenho um S\n");
		}
		
		recv_header();

		if(_buf[0] != 'S') {
			break;
		}
	}
	
	return b;
}

bool PostL0::recv(char* cmd, char* param)
{
	try {
		recv_header();

		if(_buf[0] != 'S') {
			*cmd = _buf[0];
			*param = _buf[1];
			consume_buffer();
			return true;
		}
		return false;
	}
	catch(...) {
		throw;
	}
}

bool PostL0::eos()
{
	recv_header();
	return _buf[0] != 'S';
}

