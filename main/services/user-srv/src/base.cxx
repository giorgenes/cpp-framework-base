#include "base.h"
#include <stdio.h>
#include <string.h>

using namespace usersrv_impl;

void BaseTransaction::begin()
{
}

void BaseTransaction::exec(::libany::bxtp::ODocument& st)
{
	//fprintf(stderr, "trans exec\n");
}

BaseTransaction::~BaseTransaction()
{
}

#if 0
void BaseTransaction::key_begin(const char* tag)
{
	if(strcmp(tag, "username")==0) {
		_param = PARAM_NAME;
	}
	else if(strcmp(tag, "brand")==0) {
		_param = PARAM_BRAND;
	}
	else if(strcmp(tag, "base")==0) {
		_param = PARAM_BASE;
	}
	else {
	}
	//fprintf(stderr, "chave: %s\n", tag);
}

void BaseTransaction::key_content(::libany::stream::IStream& st)
{
	int len;
	if(_param == PARAM_NAME) {
		len = st.read(_name, sizeof(_name)-1);
		_name[len] = 0;
	}
	else if(_param == PARAM_BRAND) {
		len = st.read(_brand, sizeof(_brand)-1);
		_brand[len] = 0;
	}
	else if(_param == PARAM_BASE) {
		len = st.read(_base, sizeof(_base)-1);
		_base[len] = 0;
	}
}

void BaseTransaction::key_end()
{
}

void BaseTransaction::data_begin(const char* tag)
{
	if(strcmp(tag, "create")==0) {
		_oper = OPER_CREATE;
	}
	else if(strcmp(tag, "check")==0) {
		_oper = OPER_CHECK;
	}
	else if(strcmp(tag, "list")==0) {
		_oper = OPER_LIST;
	}


	//fprintf(stderr, "dado: %s\n", tag);
}

void BaseTransaction::data_content(::libany::stream::IStream&)
{
}

void BaseTransaction::data_end()
{
}

#endif

void BaseTransaction::data(::libany::bxtp::IDocument& doc)
{
	const char* tags[] = {
		"/data/create",
		"/data/check",
		"/data/list",
		0
	};

	int idx;

	while(doc.match(tags, &idx)) {
		switch(idx) {
			case 0:
				_oper = OPER_CREATE;
				break;
			case 1:
				_oper = OPER_CHECK;
				break;
			case 2:
				_oper = OPER_LIST;
				break;
		}
	}
}

void BaseTransaction::key(::libany::bxtp::IDocument& st)
{
	const char* tags[] = {
		"/key/username",
		"/key/brand",
		"/key/base",
		0
	};

	int idx;
	int len;
	while(st.match(tags, &idx)) {
		switch(idx) {
			case 0:
				len = st.read(_name, sizeof(_name)-1);
				_name[len] = 0;
				break;
			case 1:
				len = st.read(_brand, sizeof(_brand)-1);
				_brand[len] = 0;
				break;
			case 2:
				len = st.read(_base, sizeof(_base)-1);
				_base[len] = 0;
				break;
		}
	}

}


void BaseTransaction::begin_exec(const char* service, const char*)
{
	*_name  = 0;
	*_brand = 0;
	*_base  = 0;
}

