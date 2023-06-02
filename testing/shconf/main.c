#include "shconf.h"
#include <stdio.h>
#include <signal.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#if 0
static void pause(const char* msg)
{
	char aux[1024];
	
	fprintf(stderr, "%s ", msg);
	fgets(aux, sizeof(aux), stdin);
}
#endif

static int g_upconf = 0;
static int g_quit   = 0;


static void handle_signal(int sig)
{
	switch(sig) {
		case SIGHUP:
			g_upconf = 1;
			break;
		case SIGINT:
			g_quit = 1;
			break;
	}
}

static int get_size(void* data)
{
	struct stat st;

	if(stat("./conf.txt", &st)==0) {
		return st.st_size + 1;
	}

	return 0;
}

static int initialize(void* data, char* buf, int size)
{
	FILE* fp;

	fprintf(stderr, "[%d] LOADING CONF...\n", getpid());
	fp = fopen("./conf.txt", "r");
	if(fp) {
		fread(buf, 1, size-1, fp);
		fclose(fp);
		buf[size-2] = 0;
	}
	sleep(10);
	fprintf(stderr, "[%d] CONF LOADED\n", getpid());
}

int main()
{
	shconfmgr_t* mgr;
	char* conf;
	int shmid;
	int size;
	time_t last = 0;
	shconfmgr_userhandler_t user;

	user.get_conf_size = get_size;
	user.initialize = initialize;

	signal(SIGHUP, handle_signal);
	signal(SIGINT, handle_signal);
	mgr = shconfmgr_new();
	
	shconfmgr_connect(mgr, 13453);

	fprintf(stderr, "[%d] pegando configuracoes iniciais \n", getpid());
	conf = shconfmgr_getconf(mgr, &size, &shmid, &user);

	while(!g_quit) {
		fprintf(stderr, "[%d] conf: %s\n", getpid(), conf);
		if(g_upconf) {
			fprintf(stderr, "[%d] updating conf\n", getpid());
			shconfmgr_release(mgr, shmid);
			g_upconf = 0;
		}
		if(time(NULL) > last + 5) {
			fprintf(stderr, "[%d] verifying new conf ...\n", getpid());
			if(shconfmgr_has_new_conf(mgr, shmid)) {
				fprintf(stderr, "[%d] has new conf\n", getpid());
				conf = shconfmgr_getconf(mgr, &size, &shmid, &user);
			}
			last = time(NULL);
		}
		sleep(2);
	}

	shconfmgr_delete(mgr);
	return 0;
}

