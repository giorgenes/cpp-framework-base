#ifndef __libany__rcpp__session_h
#define __libany__rcpp__session_h

#include <libany/ios/ios.h>

#define LIBANY_RCCP_VER 1

namespace libany {
	namespace rcpp {
		class Session {
			private:
				::libany::stream::IOStream* st;
				int _ver;
			public:
				Session(::libany::stream::IOStream* st, 
						int ver = LIBANY_RCCP_VER)
					: st(st), _ver(ver) {}

				inline ::libany::stream::IOStream* stm() {
					return Session::st;
				};

				virtual void handshake() = 0;
				
		};

		class ClientSession : public Session {
			public:
				ClientSession(::libany::stream::IOStream* st, 
						int ver = LIBANY_RCCP_VER)
					: Session(st, ver) {}
				void handshake();
		};

		class ServerSession : public Session {
			public:
				ServerSession(::libany::stream::IOStream* st, 
						int ver = LIBANY_RCCP_VER)
					: Session(st, ver) {}
				void handshake();
		};

		class PostL0 : public ::libany::stream::IOStream {
			protected:
				Session* _session;
				unsigned char _buf[2];
				
				void recv_header();
				inline void consume_buffer() {
					_buf[0] = 0;
				};
			public:
				PostL0(Session* session); 
				
				void send_header(char, char);
				bool recv(char*, char*);

				/* stream functions */
				virtual int write(const void*, int);
				virtual int read(void*, int);
				virtual bool eos();

		};
		
		class PostL1 : private PostL0 {
			private:
				int _cmd;
				short _state;
				int _written;

				void send_header(char, char, int, int);
				bool recv_header(char, int, int);
				void check_state(int);
			public:
				PostL1(Session* session);
				bool next_field();
				bool next_group();

				void send_end();
				void send_begin(char p_cmd);
				void recv(char*);

				/* stream functions */
				int write(const void*, int);
				int read(void*, int);
				bool eos();
		};
	}
}

#endif
