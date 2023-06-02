#include "xmlstream.h"
#include <stdexcept>

using namespace ::libany::stdstream;

#include <stdio.h>
#include <string.h>
#include <ctype.h>

int OXmlStream::write(const void* p_ptr, int p_size)
{   
	XML_Parse(this->parser, (const char*)p_ptr, p_size, 0);
	if(this->content) {
		free(this->content);
		this->content = 0;
	}

	return p_size;
}

void OXmlStream::on_tag_begin(const char*, const char **)
{
}

void OXmlStream::start(void *data, const char *el, const char **attr)
{
	OXmlStream* stream = static_cast<OXmlStream*>(data);

	stream->current_tag_path[stream->current_tag_path_len++] = '/';
	strcpy(stream->current_tag_path + stream->current_tag_path_len, el);
	stream->current_tag_path_len += strlen(el);

	stream->state = 0;

	stream->on_tag_begin(stream->current_tag_path, attr);
	
	attr = NULL;
}

void OXmlStream::end(void *data, const char *el)
{
	OXmlStream* stream = static_cast<OXmlStream*>(data);

	if(stream->content_length > 0) {
		stream->content[stream->content_length] = 0;
		stream->tag_process_callback(stream->current_tag_path, stream->content);
	}
	stream->content_length = 0;

	while(stream->current_tag_path[stream->current_tag_path_len] != '/') {
		stream->current_tag_path_len--;
	}
	stream->current_tag_path[stream->current_tag_path_len] = 0;

	stream->state = 2;

	el = NULL;
}

void OXmlStream::XML_chardata(void *data,const XML_Char *s,int len)
{
	OXmlStream* stream = static_cast<OXmlStream*>(data);

	if(stream->state == 2) return;

	if(stream->state == 0) {
		while(len && isspace(*s)) {
			s++;
			len--;
		}
	}

	if(len > 0) {
		stream->state = 1;
		if(stream->content_length + len + 1 > stream->content_length_max) {
			stream->content_length_max += len;
			stream->content_length_max *= 2;
			stream->content = (char*)realloc(stream->content, stream->content_length_max);
			if(!stream->content) {
				return;
			}
		}
		strncpy(stream->content + stream->content_length, s, len);
		stream->content_length += len;
	}

#if 0
	_d(fprintf(stderr, "data: '"));
	_d(fwrite(s, 1, len, stderr));
	_d(fprintf(stderr, "'\n"));
#endif
}


OXmlStream::~OXmlStream()
{
}

OXmlStream::OXmlStream()
	: current_tag_path_len(0), 
	content(0), 
	content_length(0),
	content_length_max(0)
{
	this->parser = XML_ParserCreate("ISO-8859-1");

	if(!this->parser) {
		throw std::runtime_error("could not allocate xml parser");
	}

	XML_SetElementHandler(this->parser, start, end);
	XML_SetCharacterDataHandler(this->parser, XML_chardata);

	XML_SetUserData(this->parser, this);
}

