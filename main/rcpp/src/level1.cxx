#include <libany/rcpp/session.h>
#include <stdexcept>
	
#define ST_INIT        (1 << 0)
#define ST_TP_SENT     (1 << 1)
#define ST_FLD_SENT    (1 << 2)
#define ST_GRP_SENT    (1 << 3)
#define ST_DT_SENT     (1 << 4)
#define ST_POST_SENT   (1 << 6)
#define ST_SEND        (1 << 7)
#define ST_RECV        (1 << 8)

using namespace ::libany::rcpp;

PostL1::PostL1(Session* session) 
	: PostL0(session), _cmd('Q'), _state(ST_INIT), _written(0)	
{
}

void PostL1::check_state(int vstates)
{
	fprintf(stderr, "esperando estado\n");
	if(!(_state & vstates)) {
		throw std::runtime_error("invalid state");
	}
}

void PostL1::send_header(char type, char param, int state, int vstates)
{
	try {
		check_state(vstates);
		PostL0::send_header(type, param);
		_state = state|ST_SEND;
	}
	catch(...) {
		throw;
	}

}

bool PostL1::recv_header(char type, int state, int vstates)
{
	try {
		check_state(vstates);

		PostL0::recv_header();
		
		switch(*_buf) {
			case 'G':
				_state = ST_GRP_SENT;
				break;
			case 'F':
				_state = ST_FLD_SENT;
				break;
			case 'S':
				_state = ST_DT_SENT;
				break;
			case 'E':
				_state = ST_POST_SENT;
				break;
		}

		_state |= ST_RECV;
		if(_buf[0] == type) {
			consume_buffer();
			return true;
		}
		return false;
	}
	catch(...) {
		throw;
	}
	
}

int PostL1::write(const void* buf, int size)
{
	try {
		check_state(ST_SEND);
		return PostL0::write(buf, size);
	}
	catch(...) {
		throw;
	}

	return 0;
}

int PostL1::read(void* buf, int size)
{
	return PostL0::read(buf, size);
}

bool PostL1::eos()
{
	fprintf(stderr, "eos?\n");
	return PostL0::eos();
}

bool PostL1::next_group()
{
	try {
		if(_state & ST_SEND) {
			send_header('G', 0, ST_GRP_SENT, 
				ST_TP_SENT|ST_GRP_SENT|ST_FLD_SENT|ST_DT_SENT);
			return true;
		}
		else {
			fprintf(stderr, "esperando header no group\n");
			bool ret = recv_header('G', ST_GRP_SENT,
					ST_TP_SENT|ST_GRP_SENT|ST_FLD_SENT|ST_DT_SENT|ST_POST_SENT);
			if(_buf[0] == 'E') {
				consume_buffer();
			}
			return ret;
		}
	}
	catch(...) {
		throw;
	}
}

bool PostL1::next_field()
{
	try {
		if(_state & ST_SEND) {
			send_header('F', 0, ST_FLD_SENT, 
					ST_DT_SENT|ST_GRP_SENT|ST_FLD_SENT);
			return true;
		}
		else {
			fprintf(stderr, "esperando header no field\n");
			return recv_header('F', ST_GRP_SENT,
				ST_GRP_SENT|ST_FLD_SENT|ST_DT_SENT);
		}
	}
	catch(...) {
		throw;
	}
}

void PostL1::send_end()
{
	try {
		check_state(ST_SEND);
		send_header('E', 0, ST_POST_SENT, ST_GRP_SENT|ST_FLD_SENT|ST_DT_SENT);
	}
	catch(...) {
		throw;
	}
}

void PostL1::send_begin(char p_cmd)
{
	try {
		check_state(ST_INIT|ST_POST_SENT);
		
		send_header('P', 
				p_cmd, 
				ST_TP_SENT|ST_SEND, 
				ST_INIT|ST_POST_SENT);
	}
	catch(...) {
		throw;
	}
}

void PostL1::recv(char *p_param)
{
	try {
		char cmd;
		check_state(ST_INIT|ST_POST_SENT);
		PostL0::recv(&cmd, p_param);
		if(cmd != 'P') {
			throw std::runtime_error("P expected");
		}
		_state = ST_TP_SENT | ST_RECV;
	}
	catch(...) {
		throw;
	}
}

