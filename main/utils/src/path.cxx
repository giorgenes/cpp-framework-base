#include <ctype.h>
#include <string.h>
#include "path_utils.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

namespace utils = ::libany::utils;

void utils::path_create(const char* path)
{
	struct stat st;

	if(stat(path, &st) != 0) {
		char* p = strrchr(path, '/');
		if(p) {
			int size = p - path;
			char buf[size+1];
			strncpy(buf, path, size);
			buf[size] = 0;
			path_create(buf);
		}
		mkdir(path, 0700);
	}
}

void utils::getrealpath(char* path, int pathsize)
{
	char cwd[2048];

	getcwd(cwd, sizeof(cwd));
	chdir(path);
	getcwd(path, pathsize);
	chdir(cwd);
}


