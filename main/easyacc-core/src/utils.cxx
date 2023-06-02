#include "utils.h"
#include <string.h>
#include <stdexcept>

void File::open(const char* p_path, const char* p_mode)
{
	if(strcmp(p_path, "-")==0) {
		if(strcmp(p_mode, "r")==0) {
			this->fp = stdin;
		}
		else if(strcmp(p_mode, "w")==0) {
			this->fp = stdout;
		}
		this->owner = false;
	}
	else {
		this->fp = fopen(p_path, p_mode);
		this->owner = true;
	}
	
	if(!this->fp) {
		char aux[2048];
		sprintf(aux, "could not open file '%s' (%m)", p_path);
		throw std::runtime_error(aux);
	}
}

void File::close()
{
	if(this->fp && this->owner) {
		fclose(this->fp);
	}
	this->fp = NULL;
}

