#include <libany/stdsrvi/ucspi.h>
#include <dlfcn.h>
#include <stdexcept>

class Module {
	private:
		void* _mod;
	public:
		Module();
		void load(const char*);
		void* sym(const char*);
		~Module();
};

void Module::load(const char* path)
{
	_mod = dlopen(path, RTLD_NOW);
	if(!_mod) {
		char buf[1024];

		sprintf(buf, "error opening %s (%s)", path, dlerror());
		throw std::runtime_error(buf);
	}
}

void* Module::sym(const char* symbol)
{
	void* s = dlsym(_mod, symbol);
	if(!s) {
		char buf[1024];

		sprintf(buf, "could not get symbol %s (%s)", symbol, dlerror());
		throw std::runtime_error(buf);
	}
	return s;
}

Module::Module()
	: _mod(0)
{
}

Module::~Module()
{
	if(_mod) {
		dlclose(_mod);
	}
}

int main(int argc, char** argv)
try
{
	if(argc < 3) {
		fprintf(stderr, "%s: usage: %s so_path module_name\n", 
				argv[0],
				argv[0]);
		return -1;
	}

	Module mod;

	mod.load(argv[1]);
	::libany::ios::Service* (*iosrvmod_new)(int, char* const*);
	char buf[1024];
	sprintf(buf, "iosrvmod_%s_new", argv[2]);
	iosrvmod_new = reinterpret_cast
		< ::libany::ios::Service* (*)(int, char* const *)>
		(mod.sym(buf));

	::libany::ios::Service* srv 
		= iosrvmod_new(argc -2, &argv[2]);
	if(!srv) {
		throw std::runtime_error("could not allocate service");
	}
	
	::libany::stdsrvi::Ucspi u(*srv);

	u.run();
	
	return 0;
}
catch(std::exception& e) {
	fprintf(stderr, "exception: %s\n", e.what());
}

