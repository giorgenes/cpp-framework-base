#include "shconf.h"
#include <stdlib.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/sem.h>
#include <string.h>

typedef struct {
	int shmid;
}shmgr_shared_t;

struct shconfmgr {
	int key;
	int shmid;
	int semid;
	shmgr_shared_t* shared;
};

shconfmgr_t* shconfmgr_new(void)
{
	shconfmgr_t* p = malloc(sizeof(*p));

	p->key = 0;

	return p;
}

void shconfmgr_delete(shconfmgr_t* self)
{
	free(self);
}

static void sem_wait(shconfmgr_t* self)
{
	struct sembuf buf;
	
	buf.sem_num = 0;
	buf.sem_op = -1;
	buf.sem_flg = SEM_UNDO;

	semop(self->semid, &buf, 1);
}

static void sem_post(shconfmgr_t* self, int flags)
{
	struct sembuf buf;
	
	buf.sem_num = 0;
	buf.sem_op = 1;
	buf.sem_flg = flags;

	semop(self->semid, &buf, 1);
}


void shconfmgr_connect(shconfmgr_t* self, int key)
{
	int first_sem = 0;
	int first_shm = 0;

	/* get semafore handle */
	self->semid = semget(key, 1, IPC_EXCL|IPC_CREAT|0400|0200);
	if(self->semid != -1) {
		first_sem = 1;
	}
	else {
		self->semid = semget(key, 1, 0400|0200);
	}

	if(self->semid == -1) {
		fprintf(stderr, "error (%m)\n");
		return;
	}

	/* get shared memory handle */
	self->shmid = 
		shmget(key, sizeof(shmgr_shared_t), IPC_EXCL|IPC_CREAT|0400|0200);

	if(self->shmid != -1) {
		first_shm = 1;
	}
	else {
		self->shmid = 
			shmget(key, sizeof(shmgr_shared_t), 0400|0200);
	}
	if(self->shmid == -1) {
		fprintf(stderr, "error (%m)\n");
		return;
	}

	/* inicialize.
	 * shared memory initialization come's first
	 * to avoid other processes to access it
	 * uninitialized */
	fprintf(stderr, "shmid: %d\n", self->shmid);
	self->shared = shmat(self->shmid, 0, 0);
	if((int)self->shared == -1) {
		fprintf(stderr, "error (%m)\n");
		return;
	}
	if(first_shm) {
		self->shared->shmid = 0;
		sem_post(self, 0);
	}
	fprintf(stderr, "tm: %d\n", self->shared->shmid);
}

void* 
shconfmgr_getconf(
		shconfmgr_t* self, 
		int *size, 
		int* shmid, 
		shconfmgr_userhandler_t* user)
{
	void* p = 0;
	int init = 0;

	sem_wait(self);
	if(self->shared->shmid == 0) {
		*size = user->get_conf_size(user->userdata);
		if(*size > 0) {
			*shmid = shmget(IPC_PRIVATE, 
					*size + sizeof(*size), 
					IPC_CREAT|0400|0200);
			init = 1;
		}
	}
	else {
		*shmid = self->shared->shmid;
	}

	if(*shmid != 0) {
		p = shmat(*shmid, 0, 0);

		if(init) {
			memcpy(p, size, sizeof(*size));
			p = ((char*)p) + sizeof(*size);
			user->initialize(user->userdata, p, *size);
		}
		else {
			memcpy(size, p, sizeof(*size));
			p = ((char*)p) + sizeof(*size);
		}
		self->shared->shmid = *shmid;
	}

	sem_post(self, SEM_UNDO);

	return p;
}

void shconfmgr_release(shconfmgr_t* self, int shmid)
{
	sem_wait(self);
	if(self->shared->shmid == shmid) {
		/** this changes the key to 0, making it
		 * impossible to other processes to
		 * attach to it */
		shmctl(shmid, IPC_RMID, 0);
		self->shared->shmid = 0;
	}
	sem_post(self, SEM_UNDO);
}


int shconfmgr_has_new_conf(shconfmgr_t* self, int shmid)
{
	int has;

	sem_wait(self);
	has = self->shared->shmid != shmid;
	sem_post(self, SEM_UNDO);

	return has;
}

