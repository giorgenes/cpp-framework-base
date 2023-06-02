#include <stdio.h>
#include <sys/socket.h>
#include "iok.h"

static void handle_client(int fd, struct timeval * ptv)
{
	int err;
	char buf[1024];
	int len;
	
	/* manda o hello */
	if(trrmgr_iok_send(fd, "hello", 5, ptv, &err) < 0) {
		fprintf(stderr, "send hello error: %d\n", err);
		return;
	}

	/* aguarda a resposta */
	if(trrmgr_iok_recv(fd, buf, 2, ptv, &err, 1) < 0) {
		fprintf(stderr, "hello ack error: %d\n", err);
		return;
	}

	if(buf[0] != 'O' || buf[1] != 'K') {
		fprintf(stderr, "ack do hello invalido\n");
		return;
	}

	while(fgets(buf, sizeof(buf)-1, stdin)!=NULL) {
		len = strlen(buf);
		
		if(trrmgr_iok_send(fd, buf, len, NULL, &err) < 0) {
			fprintf(stderr, "send error: %d\n", err);
			return;
		}
		
		if(trrmgr_iok_recv(fd, buf, len, NULL, &err, 1) < 0) {
			fprintf(stderr, "recv error: %d\n", err);
			return;
		}

		fwrite(buf, 1, len, stdout);
		fflush(stdout);
	}
}

int main(int argc, char** argv)
{
	int fd;
	struct timeval tv;
	int err;
	
	if(argc < 3) {
		fprintf(stderr, "usage: client host port\n");
		return -1;
	}

	tv.tv_sec = 3;
	tv.tv_usec = 0;
	fd = trrmgr_iok_connect(argv[1], atoi(argv[2]), &tv, &err);
	if(fd < 0) {
		fprintf(stderr, "connect error: %d\n", err);
		return -1;
	}

	handle_client(fd, &tv);

	shutdown(fd, SHUT_RDWR);
	close(fd);
	
	return 0;
}

