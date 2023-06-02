#include "vta.h"
#include <string.h>
#include <stdlib.h>
#include <errno.h>

typedef struct {
	char   name[32];
	char   type;
	size_t offset;
	size_t size;
}field_t;

typedef struct {
	char name[64];
	int (*get_cb)(const char* const *);

	field_t* keys;
	int      nkeys;
	size_t   keysize;
	
	field_t* data;
	int      ndata;
	size_t   datasize;
}table_descriptor_t;

typedef struct {
	table_descriptor_t* tables;
	int                 ntables;
	int                 keymaxsize;
}vta_t;

typedef struct {
}table_t;

typedef struct {
	table_t* tables;
}session_t;

static vta_t     g_vta;
static session_t g_session;

int vta_init()
{
	memset(&g_vta, 0, sizeof(g_vta));
}

int vta_finish()
{
}

int vta_begin()
{
}

int vta_end()
{
}

/* TODO: optimize this */
static int get_table_idx(const char* p_table)
{
	int i;

	for(i=0; i<g_vta.ntables; i++) {
		if(strcmp(g_vta.tables[i].name, p_table)==0) {
			return i;
		}
	}

	return -1;
}

static int 
table_get(table_descriptor_t* tdesc, table_t* self, const char* const * p_key)
{
	/* weee! c99 rulez */
	char binkey[tdesc->keysize];

	return 0;
}

int 
vta_get(const char* p_tname, const char* const * p_key, const char** p_data)
{
	int idx;
	void* key;

	idx = get_table_idx(p_tname);
	if(idx < 0) {
		return -EBADF;
	}

	table_get(&g_vta.tables[idx], &g_session.tables[idx], key);
	
	return 0;
}

int 
vta_set(
		const char* p_tname, 
		const char* const* p_key, 
		const char* const* p_data)
{
	return 0;
}

