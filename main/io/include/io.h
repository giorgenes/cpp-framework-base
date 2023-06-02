#include <sys/time.h>

namespace libany {
	namespace io {
		class FD {
			private:
				static bool _fail;
				int rfd;
				int wfd;

				int my_select(
							int fd, 
							struct timeval *p_tv, 
							int p_retry, 
							int p_flags, 
							int* p_error);

			public:
				FD() : rfd(-1), wfd(-1) {};
				FD(int fd) : rfd(fd), wfd(fd) {};
				FD(int fd1, int fd2) : rfd(fd1), wfd(fd2) {};
				~FD();

				int recv(
							void* p_buf, 
							size_t p_size, 
							struct timeval* p_tv, 
							int* p_error,
							int p_complete);
				int send(
						const void* p_buf, 
						size_t p_size, 
						struct timeval* p_tv,
						int* p_error);
				
				int read(
							void* p_buf, 
							size_t p_size, 
							struct timeval* p_tv, 
							int* p_error,
							int p_complete);
				int write(
						const void* p_buf, 
						size_t p_size, 
						struct timeval* p_tv,
						int* p_error);


				static void set_fail(bool);
				void connect(const char* p_host, int p_port, struct timeval* ptv, int* p_error);
				int accept(int p_tmout, int p_retry, int* p_rfd, int* p_error);
		};
	}
}
