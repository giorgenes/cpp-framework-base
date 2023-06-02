#ifndef __basetransaction_h
#define __basetransaction_h

#include <libany/prodsrv/product.h>

#define PARAM_NAME  0
#define PARAM_BRAND 1
#define PARAM_BASE  2

#define OPER_NONE     0
#define OPER_CREATE   1
#define OPER_CHECK    2
#define OPER_LIST     3
#define OPER_CHECKOUT 4
#define OPER_COMMIT   5
#define OPER_LISTVER  6

#include "product.h"
#include <stdio.h>

namespace productsrv {

class BaseTransaction : public ::libany::rcpp::BaseTransaction {
	private:
		void get_files(::libany::bxtp::IDocument&);
		void get_file(::libany::bxtp::IDocument&, FILE*);
		void get_version(const char*, ::libany::bxtp::IDocument&);
	protected:
		char _base[128];
		char _brand[128];
		char _name[128];
		char _version[128];
		int _oper;
		int _param;
		char _tmppath[1024];
		char _tmppath_filelist[1024];
	public:
		virtual void begin();
		virtual void begin_exec(const char*, const char*);
	
		virtual void exec(::libany::bxtp::ODocument&);
		virtual void key(::libany::bxtp::IDocument&);
		virtual void data(::libany::bxtp::IDocument&);

		virtual ~BaseTransaction();
};

}

#endif
