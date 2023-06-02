#ifndef __shconf_h
#define __shconf_h

typedef struct shconfmgr shconfmgr_t;

typedef struct {
	void* userdata;
	int (*get_conf_size)(void*);
	int (*initialize)(void*, char*, int);
}shconfmgr_userhandler_t;

shconfmgr_t* shconfmgr_new();
void shconfmgr_delete(shconfmgr_t*);

void shconfmgr_connect(shconfmgr_t*, int);
void* shconfmgr_getconf(shconfmgr_t* self, int* size, int *shmid, shconfmgr_userhandler_t*);
void shconfmgr_release(shconfmgr_t* self, int shmid);
int shconfmgr_has_new_conf(shconfmgr_t* self, int shmid);

#endif
