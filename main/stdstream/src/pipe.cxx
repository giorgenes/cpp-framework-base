#include "pipe.h"
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <libany/utils/string_utils.h>
#include <libany/io/io.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdexcept>

using namespace ::libany::stdstream;

static int
open_program(char* const argv[], int* fds, pid_t* p)
{   
	int p1[2];
	int p2[2];
	int p3[2];

	pipe(p1);
	pipe(p2);
	pipe(p3);

	if ((*p = fork()) == 0) {
		/* i'm the child! run program! */
		close(p1[1]);
		close(p2[0]);
		close(p3[0]);
		dup2(p1[0], 0);
		dup2(p2[1], 1);
	
		/* set close-on-exec */	
		fcntl(p3[1], F_SETFD, 1);
		execvp(argv[0], argv);

		/* send the errno to the parent,
		 * so that it can know we didn't run
		 * the program */
		write(p3[1], &errno, sizeof(errno));
		fprintf(stderr, "could not spawn %s\n", argv[0]);
		
		close(p1[0]);
		close(p2[1]);

		/* if we're here we suck */
		exit(-1);
	}   
	/* i'm the parent */
	close(p1[0]);
	close(p2[1]);
	close(p3[1]);
	fds[0] = p2[0];
	fds[1] = p1[1];

	::libany::io::FD wd(p3[0]);
	int execerror;
	int err;
	if(wd.read(&execerror, sizeof(execerror), 0, &err, 1) > 0) {
		/* the program did't execute 
		 * and the errno was sent throught
		 * the pipe */
		return execerror;
	}

	return 0;
}   

IOPipeStream::IOPipeStream(const char* path)
	: IOFDStream(0, 0), _pid(-1)
{
	int fds[2];
	char aux[2048];

	strncpy(aux, path, sizeof(aux));
	aux[sizeof(aux)-1] = 0;

	char* args[1024];
	int nargs;
	nargs = ::libany::utils::string_split(aux, " \t", args, 1023);
	args[nargs] = 0;
	if(open_program(args, fds, &_pid)) {
		IOPipeStream::close();
		throw std::runtime_error("could not exec program");
	}

	setfd(fds[0], fds[1]);
}


void IOPipeStream::close()
{
	int sm_stat;
	
	if(_pid < 0) {
		return;
	}
	
	IOFDStream::close();
	while (waitpid(_pid, &sm_stat, 0) < 0) {
		if(errno == ECHILD) {
			break;
		}
	}
	_pid = -1;
}

IOPipeStream::~IOPipeStream()
{
	IOPipeStream::close();
}

