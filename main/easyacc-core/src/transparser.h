#ifndef __transparser_h
#define __transparser_h

#include <time.h>

class TransactionParser {
	private:
		void parse_command(char* cmd, struct tm* ttm);
		void process_auto_command(char** args);
	protected:
		virtual void 
			on_transaction(const char*, const char*, double, bool, time_t);
	public:
		void process_file(const char*);

		virtual ~TransactionParser() {
		};
};

#endif
