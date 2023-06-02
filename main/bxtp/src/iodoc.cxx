#include "document.h"
#include <stdio.h>
#include <string.h>

using namespace ::libany::bxtp;

IODocument::~IODocument()
{
}

bool IODocument::eos()
{
	return IDocument::eos();
}

int IODocument::read(void* p, int s)
{
	return IDocument::read(p, s);
}

int IODocument::write(const void* p, int s)
{
	return ODocument::write(p, s);
}

