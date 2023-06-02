#include "iok.h"
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>


static void handle_client(int fd)
{
	char buf[1024];
	int err;
	int len;

	if(trrmgr_iok_recv(fd, buf, 5, NULL, &err, 1) <= 0) {
		fprintf(stderr, "hello recv error: %d\n", err);
		return;
	}
	
	if(trrmgr_iok_send(fd, "OK", 2, NULL, &err) <= 0) {
		fprintf(stderr, "ack send error: %d\n", err);
		return;
	}
	
	while((len=trrmgr_iok_recv(fd, buf, sizeof(buf), NULL, &err, 0)) > 0) {
		if(trrmgr_iok_send(fd, buf, len, NULL, &err) < 0) {
			fprintf(stderr, "send error: %d\n", err);
			return;
		}
	}
}

int main()
{
	int fd;
	int clifd;
	struct sockaddr_in addr;
	int err;
	int op ;

	memset(&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY); 
	addr.sin_port = htons(11121);

	fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if(bind(fd, (struct sockaddr*)&addr, sizeof(addr))) {
		fprintf(stderr, "bind error (%m\n)");
		return -1;
	}

	if(listen(fd, 1)) {
		fprintf(stderr, "listen error (%m\n)");
		return -1;
	}
	op = 1;
	if(setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, &op, sizeof(op)) < 0) {
		fprintf(stderr, "setsockopt error (%m\n)");
	}
	op = 20;
	if(setsockopt(fd, SOL_TCP, TCP_KEEPIDLE, &op, sizeof(op)) < 0) {
		fprintf(stderr, "setsockopt error (%m\n)");
	}
	op = 10;
	if(setsockopt(fd, SOL_TCP, TCP_KEEPINTVL, &op, sizeof(op)) < 0) {
		fprintf(stderr, "setsockopt error (%m\n)");
	}
	op = 3;
	if(setsockopt(fd, SOL_TCP, TCP_KEEPCNT, &op, sizeof(op)) < 0) {
		fprintf(stderr, "setsockopt error (%m\n)");
	}

	fcntl(fd, F_SETFL, FNDELAY);

	do {
		printf("waiting for connection\n");
		if(trrmgr_iok_accept(fd, 0, 1, &clifd, &err)==0) {
			handle_client(clifd);
			printf("client exited\n");
			shutdown(clifd, SHUT_RDWR);
			close(clifd);
		}
	}while(1);

	return 0;
}
