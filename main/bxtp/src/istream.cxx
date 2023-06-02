#include <libany/bxtp/stream.h>
#include <string.h>
#include <stdexcept>

using namespace ::libany::bxtp;

BxtpIStream::~BxtpIStream()
{
}

int BxtpIStream::read(void* ptr, int s)
{
	if(_st != 2) return -1;

	char buf[2];
	int b = 0;
	char* p = (char*)ptr;

	while(s > 0 && _in.read(buf, 2) == 2) {
		if(buf[0] != 'D') {
			_st = 1;
			_in.unread(buf, 2);
			break;
		}

		if(buf[1] == 0) {
			_st = 1;
			break;
		}
		
		if(s >= buf[1]) {
			_in.read(p, buf[1]);
			p += buf[1];
			s -= buf[1];
			b += buf[1];
		}
		else {
			_in.read(p, s);
			b += s;
			s = 0;
			buf[1] = buf[1] - s;
			_in.unread(buf, 2);
		}
	}
	
	return b;
}

bool BxtpIStream::eos()
{
	return _st != 2;
}

bool BxtpIStream::next_token(int* token)
{
	char buf[2];
	int len;
	
again:
	switch(_st) {
		case 0:
			if(_in.read(buf, 2) <= 0) {
				return false;
			}
			if(buf[0] != 'T' || buf[1] == 0) {
				throw std::runtime_error("T token expected");
			}
			if((len=_in.read(_tag, buf[1])) <= 0) {
				throw std::runtime_error("error reading tag");
			}
			_tag[len] = 0;
			_st = 1;
			*token = LIBANY_BXTP_TOKEN_BEGIN;
			break;
		case 1:
			if(_in.read(buf, 2) <= 0) {
				return false;
			}
			if(buf[0] == 'T') {
				if(buf[1] == 0) {
					*token = LIBANY_BXTP_TOKEN_END;
				}
				else {
					if((len=_in.read(_tag, buf[1])) <= 0) {
						throw std::runtime_error("error reading tag");
					}
					_tag[len] = 0;
					*token = LIBANY_BXTP_TOKEN_BEGIN;
				}
			}
			else if(buf[0] == 'D') {
				_in.unread(buf, 2);
				_st = 2;	
				*token = LIBANY_BXTP_TOKEN_DATA;
			}
			else if(buf[0] == 'R') {
				*token = LIBANY_BXTP_TOKEN_RESET;
			}
			else {
				throw std::runtime_error("invalid token");
			}
			break;
		case 2:
			/* user didn't get all the data, lets
			 * fetch it and keep going */
			{
				char aux[1024];
				while(!eos()) {
					read(aux, sizeof(aux));
				}
			}
			_st = 1;
			goto again;
			break;
		case 3:
			_st = 0;
			return false;
			break;
	}

	return true;
}

