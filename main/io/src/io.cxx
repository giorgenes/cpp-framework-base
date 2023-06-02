#include <libany/io/io.h>

#ifdef _MSC_VER
#  include <io.h>
#  include <winsock2.h>
#else
#  ifndef _GNU_SOURCE
#    define _GNU_SOURCE
#  endif
#  include <sys/select.h>
#  include <sys/time.h>
#  include <sys/types.h>
#  include <unistd.h>
#  include <arpa/inet.h>
#  include <netdb.h>
#  include <netinet/in.h>
#  include <sys/socket.h>
#  include <sys/time.h>
#  include <unistd.h>
#  include <signal.h>
#  include <fcntl.h>
#endif

#include <errno.h>
#include <stdio.h>
#include <stdexcept>

#define SELECT_OPT_READ  (1 << 0)
#define SELECT_OPT_WRITE (1 << 1)

bool ::libany::io::FD::_fail = false;

using namespace ::libany::io;

void FD::set_fail(bool fail)
{
	_fail = fail;
}

static int
timeval_subtract (struct timeval *result, struct timeval *x, struct timeval *y)
{
	/* Perform the carry for the later subtraction by updating y. */
	if (x->tv_usec < y->tv_usec) {
		int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
		y->tv_usec -= 1000000 * nsec;
		y->tv_sec += nsec;
	}
	if (x->tv_usec - y->tv_usec > 1000000) {
		int nsec = (x->tv_usec - y->tv_usec) / 1000000;
		y->tv_usec += 1000000 * nsec;
		y->tv_sec -= nsec;
	}

	/* Compute the time remaining to wait.
	 * tv_usec is certainly positive. */
	result->tv_sec = x->tv_sec - y->tv_sec;
	result->tv_usec = x->tv_usec - y->tv_usec;

	if(result->tv_sec < 0) {
		result->tv_sec  = 0;
		result->tv_usec = 0;
	}

	/* Return 1 if result is negative. */
	return x->tv_sec < y->tv_sec;
}

int 
FD::my_select(
		int fd, 
		struct timeval *p_tv, 
		int p_retry, 
		int p_flags, 
		int* p_error)
{
	int iRetSelect = -1;
	fd_set rfds;
	fd_set* pfdsetr = NULL;
	fd_set* pfdsetw = NULL;
	struct timeval begin;
	struct timeval end;
	struct timeval diff;
#ifdef _WIN32
	struct timeval tv;
	struct timeval *ptv = NULL;
#else
	sigset_t ssigset;
	struct timespec tv;
	struct timespec *ptv = NULL;
#endif
	int olderrno = 0;

	while(!_fail) {
		gettimeofday(&begin, NULL);
		FD_ZERO(&rfds);
		FD_SET(fd, &rfds);
		
		if(p_flags & SELECT_OPT_READ) {
			pfdsetr = &rfds;
		}
		else {
			pfdsetw = &rfds;
		}
		
		ptv = NULL;
		if (p_tv){
			tv.tv_sec = p_tv->tv_sec;
#ifdef _WIN32
			tv.tv_usec = p_tv->tv_usec;
#else
			tv.tv_nsec = p_tv->tv_usec * 100;
#endif
			ptv = &tv;
		}
	
		fprintf(stderr, "waiting on [p]select"
				"(tmout=%ld:%ld) for %s on fd %d [%s:%d]\n",
				p_tv ? p_tv->tv_sec : -1,
				p_tv ? p_tv->tv_usec : -1,
				p_flags & SELECT_OPT_READ ? "read" : "write",
				fd,
				__FILE__, __LINE__);
#ifdef _WIN32
		iRetSelect = select(fd + 1,
				pfdsetr, 
				pfdsetw, 
				NULL, 
				ptv);
#else
		sigemptyset(&ssigset);
		iRetSelect = pselect(fd + 1,
				pfdsetr, 
				pfdsetw, 
				NULL, 
				ptv,
				&ssigset);
#endif
		olderrno = errno;
		
		if(p_tv) {
			gettimeofday(&end, NULL);
			timeval_subtract(&diff, &end, &begin);
			timeval_subtract(p_tv, p_tv, &diff);
		}
		fprintf(stderr, "[p]select returned %d"
				"(errno=%d,tmout=%ld:%ld) for %s on fd %d [%s:%d]\n",
				iRetSelect,
				olderrno,
				p_tv ? p_tv->tv_sec : -1,
				p_tv ? p_tv->tv_usec : -1,
				p_flags & SELECT_OPT_READ ? "read" : "write",
				fd,
				__FILE__, __LINE__);


		if(iRetSelect != -1) {
			if(iRetSelect == 0) {
				*p_error = ETIMEDOUT;
			}
			else {
#if 0
				getsockerror(fd, &sockerror);
				fprintf(stderr, "socket error = %d\n", sockerror);
#endif
			}
			break;
		}
		else {
			if(olderrno != EINTR || !p_retry) {
				*p_error = olderrno;
				break;
			}
		}
	}

	errno = olderrno;	
	return iRetSelect;
}

int 
FD::accept(int p_tmout, int p_retry, int* p_rfd, int* p_error)
{
	int fd;
	int ierr;
	struct timeval tv;
	struct timeval* ptv = NULL;
	int olderrno = EINTR;

	*p_rfd = -1;
	
	if(p_tmout) {
		ptv = &tv;
		tv.tv_sec = p_tmout;
		tv.tv_usec = 0;
	}
	
	while(!_fail) {
#if 1
		ierr = my_select(rfd, ptv, p_retry, SELECT_OPT_READ, p_error);
		olderrno = errno;

		/* se retornar erro aqui eh pq ou deu timeout
		 * ou deu um erro diferente de EINTR.
		 * Ou se o erro for EINTR era pra sair mesmo,
		 * pois o select acima ja controla retentativas
		 * qdo solicitado*/
		if(ierr <= 0) {
			break;
		}
#endif
		fd = ::accept(rfd, NULL, NULL);
		if(fd > 0) {
			*p_rfd = fd;
			return 0;
		}
		olderrno = errno;
		/* se tivemos um erro q aparentemente
		 * parece permanente caimos fora,
		 * senao alguem deve ter roubado a nossa
		 * conexao, entao esperamos por uma proxima */
		if(errno != EINTR && errno != EAGAIN && errno != EWOULDBLOCK) {
			*p_error = errno;
			break;
		}
	}

	return olderrno;
}

void
FD::connect(const char* p_host, int p_port, struct timeval* ptv, int* p_error)
{
	int fd = -1;
	struct hostent* phostent;
	struct sockaddr_in addr;
	int ierr;
	int size;

	fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (fd < 0)	{
		*p_error = errno;
		throw std::runtime_error("could not create socket");
	}		
	
	if (fcntl(fd, F_SETFL, FNDELAY) < 0) {
		throw std::runtime_error("could not set nonblock mode");
	}

	phostent = gethostbyname(p_host);
	if (phostent == NULL) {
		close(fd);
		throw std::runtime_error("could not get hostname");
	}

	addr.sin_family = PF_INET;
	addr.sin_port = htons(p_port);
	addr.sin_addr = *((struct in_addr*)phostent->h_addr);

	// Conecta no servidor
	if(::connect(fd, (struct sockaddr*)&addr, sizeof(addr)) != 0)	{
		if(errno != EINPROGRESS) {
			*p_error = errno;
			close(fd);
			throw std::runtime_error("error connecting to host");
		}
	}

	ierr = my_select(fd, ptv, 1, SELECT_OPT_WRITE, p_error);
	if(ierr <= 0) {
		fprintf(stderr, "connect(select) %m\n");
		close(fd);
		throw std::runtime_error("error waiting for event");
	}

	size = sizeof(ierr);
	if(getsockopt(fd, SOL_SOCKET, SO_ERROR, &ierr, (socklen_t*)&size) != 0) {
		*p_error = errno;
		fprintf(stderr, "getsockopt %m\n");
		close(fd);
		throw std::runtime_error("error getting error :)");
	}

	if(ierr) {
		*p_error = ierr;
		errno = ierr;
		close(fd);
		throw std::runtime_error("error connecting to host");
	}

	rfd = wfd = fd;
}

int 
FD::recv(
		void* p_buf, 
		size_t p_size, 
		struct timeval* p_tv, 
		int* p_error,
		int p_complete)
{
	int ierr;
	int b = 0;

	*p_error = 0;
	//fprintf(stderr, "is_connected: %d\n", Sock_TCPIsConnected(p_fd));
	while(!_fail && p_size > 0) {
		ierr = my_select(rfd, p_tv, 1, SELECT_OPT_READ, p_error);
		if(ierr <= 0) {
			break;
		}
		ierr = ::recv(rfd, p_buf, p_size, 0);
		if(ierr <= 0) {
			/* nao devolve menos bytes
			 * do que o que foi pedido */
			*p_error = errno;
			break;
		}
		p_buf = (char*)p_buf + ierr;
		p_size -= ierr;
		b += ierr;
		if(!p_complete) {
			break;
		}
	}

	fprintf(stderr, "recv loop end(b=%d, p_size=%d,errno=%d)\n", b, 
			p_size, *p_error);
	return b;
}

/** TODO: optimize this
 * to remove duplicated code */
int 
FD::read(
		void* p_buf, 
		size_t p_size, 
		struct timeval* p_tv, 
		int* p_error,
		int p_complete)
{
	int ierr;
	int b = 0;

	*p_error = 0;
	//fprintf(stderr, "is_connected: %d\n", Sock_TCPIsConnected(prfd));
	while(!_fail && p_size > 0) {
		ierr = my_select(rfd, p_tv, 1, SELECT_OPT_READ, p_error);
		if(ierr <= 0) {
			break;
		}
		ierr = ::read(rfd, p_buf, p_size);
		if(ierr <= 0) {
			/* nao devolve menos bytes
			 * do que o que foi pedido */
			*p_error = errno;
			break;
		}
		p_buf = (char*)p_buf + ierr;
		p_size -= ierr;
		b += ierr;
		if(!p_complete) {
			break;
		}
	}

	fprintf(stderr, "recv loop end(b=%d, p_size=%d,errno=%d)\n", b, 
			p_size, *p_error);
	return b;
}


int 
FD::write(
		const void* p_buf, 
		size_t p_size, 
		struct timeval* p_tv,
		int* p_error)
{
	int ierr;
	int b = 0;

	while(!_fail && p_size > 0) {
		ierr = my_select(wfd, p_tv, 1, SELECT_OPT_WRITE, p_error);
		if(ierr <= 0) {
			break;
		}
		ierr = ::write(wfd, p_buf, p_size);
		if(ierr <= 0) {
			*p_error = errno;
			/* nao devolve menos bytes
			 * do que o que foi pedido */
			b = -1;
			break;
		}
		p_buf = (char*)p_buf + ierr;
		p_size -= ierr;
		b += ierr;
	}

	return b;
}



int 
FD::send(
		const void* p_buf, 
		size_t p_size, 
		struct timeval* p_tv,
		int* p_error)
{
	int ierr;
	int b = 0;

	while(!_fail && p_size > 0) {
		ierr = my_select(wfd, p_tv, 1, SELECT_OPT_WRITE, p_error);
		if(ierr <= 0) {
			break;
		}
		ierr = ::send(wfd, p_buf, p_size, 0);
		if(ierr <= 0) {
			*p_error = errno;
			/* nao devolve menos bytes
			 * do que o que foi pedido */
			b = -1;
			break;
		}
		p_buf = (char*)p_buf + ierr;
		p_size -= ierr;
		b += ierr;
	}

	return b;
}

FD::~FD()
{
	if(rfd != -1) {
		close(rfd);
	}
	if(rfd != wfd && wfd != -1) {
		close(wfd);
	}
}


