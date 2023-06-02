#include "document.h"
#include <stdio.h>
#include <string.h>
#include <stdexcept>

using namespace ::libany::bxtp;

IDocument::~IDocument()
try
{
	match_end();
}
catch(...) {
}

IDocument::IDocument(IDocument& o)
	: _depth(1), _end(false), _bxtp(o._bxtp), _parent(&o)
{
	char* p;
	
	*_path = 0;
	p = strrchr(o._path, '/');
	if(p) {
		strcpy(_path, "/");
		strcat(_path, p+1);
	}
}

void IDocument::path_cat()
{
	strcat(_path, "/");
	strcat(_path, _bxtp.tag());
	_depth++;
}

void IDocument::path_cut()
{
	char* p = strrchr(_path, '/');
	if(p) {
		*p = 0;
	}

	_depth--;
}

void IDocument::reset()
{
	/* we finish the stream
	 * and also reset the parent
	 * stream. The parent stream
	 * must know the stream was
	 * reseted too, to avoid
	 * waiting for more data */
	_depth = 0;
	_end = true;
	if(_parent) {
		_parent->reset();
	}
}

bool IDocument::next_token(int* tk)
{
	if(_end) {
		return false;
	}
	
	if(!_bxtp.next_token(tk)) {
		_end = true;
		if(_depth > 0) {
			throw std::runtime_error("broken stream");
		}
		return false;
	}

	switch(*tk) {
		case LIBANY_BXTP_TOKEN_BEGIN:
			path_cat();
			break;
		case LIBANY_BXTP_TOKEN_DATA:
			break;
		case LIBANY_BXTP_TOKEN_END:
			path_cut();
			break;
		case LIBANY_BXTP_TOKEN_RESET:
			reset();
			throw std::runtime_error("stream reseted");
			break;
	}
	if(_depth == 0) {
		if(_parent) {
			_parent->path_cut();
		}
		_end = true;
	}

	return true;
}

void IDocument::match_end()
{
	while(next_token(&_tk)) {
	}
}

bool 
IDocument::match(MatchList* mlist, int* list, int* idx, bool throw_exception)
{
	MatchList* cur;
	while(match_next()) {
		cur = mlist;
		*list = 0;
		while(cur) {
			*idx = 0;
			while(cur->items[*idx] != 0) {
				if(strcmp(_path, cur->items[*idx])==0) {
					return true;
				}
				(*idx)++;
			}
			cur = cur->next;
			(*list)++;
		}
	}

	if(throw_exception) {
		char buf[1024];
		sprintf(buf, "could not get tags");
		throw std::runtime_error(buf);
	}

	*idx = -1;
	return false;
}

bool IDocument::match(const char** paths, int* idx, bool throw_exception)
try
{
	while(match_next()) {
		*idx = 0;
		while(paths[*idx] != 0) {
			if(strcmp(_path, paths[*idx])==0) {
				return true;
			}
			(*idx)++;
		}
	}

	if(throw_exception) {
		char buf[1024];
		sprintf(buf, "could not get tags");
		throw std::runtime_error(buf);
	}
	

	*idx = -1;
	return false;
}
catch(...) {
	throw;
}

bool IDocument::match(const char* path, bool throw_exception)
{
	while(match_next()) {
		if(strcmp(_path, path)==0) {
			return true;
		}
	}

	if(throw_exception) {
		char buf[1024];
		sprintf(buf, "could not get tag %s", path);
		throw std::runtime_error(buf);
	}
	
	return false;
}

/* TODO: 
 * - optimize this 
 * - skip DATA tokens
 * - fix the matching of END tags
 *   */
bool IDocument::match_next()
try
{
	return next_token(&_tk);
}
catch(...) {
	throw;
}

void IDocument::gotodata()
{
	/* if we didn't get to the data yet,
	 * go to there */
	if(_tk != LIBANY_BXTP_TOKEN_DATA) {
		next_token(&_tk);
	}
}

bool IDocument::eos()
{
	gotodata();
	return _bxtp.eos();
}

int IDocument::read(void* p, int s)
{
	gotodata();
	return _bxtp.read(p, s);
}

