#include "Store.h"

#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <sys/types.h>       /* For ftruncate */
#include <fcntl.h>           /* For O_* constants */

#define _(X,Y,Z)		if((X) < 0){perror(Y); return Z;}
#define __(X,Y,Z)		if((X) == 0){perror(Y); return Z;}

/**
_(pthread_mutex_lock(&self->m_nCounters), "Store_closeCounter, mutex_lock",-1);
_(pthread_mutex_unlock(&self->m_nCounters), "Store_closeCounter, mutex_unlock",-2);

**/


int Store_init(Store * self){

	//_(pthread_mutex_init(&self->m_changingNCounters,NULL), "Store_init, mutex_init m_changingNCounters", -1);
	//_(pthread_mutex_init(&self->m_choosingCounter,NULL), "Store_init, mutex_init m_choosingCounter", -2);

	self->openingTime = 100; //TODO get operating system time.
	self->nCounters = 0;

	return 0;
}

int Store_delete(Store *self){
	//_(pthread_mutex_destroy(&self->m_nCounters), "Store_delete, mutex_destroy m_nCounters" ,-1);
	//_(pthread_mutex_destroy(&self->m_choosingCounter), "Store_delete, mutex_destroy m_nCounters" ,-2);

	return 0;
}


Counter * Store_getFreerCounter(Store * self){
	int i = 0, freerCounterIndex = 0;

	for(; i < self->nCounters; i++){
		if(Counter_getDuration(&self->counters[i]) == -1 && Counter_getNClientsInService(&self->counters[i]) < Counter_getNClientsInService(&self->counters[freerCounterIndex])){
			freerCounterIndex = i;
		}
	}
	return &self->counters[freerCounterIndex];
}

Counter * Store_getOpenedCounter(Store * self, int index){
	if(self->nCounters < index + 1){
		return 0;
	}
	return &self->counters[index];
}



Counter * Store_openCounter(Store * self){
	int index = self->nCounters++;

	Counter_init(&self->counters[index], self->nCounters);
	return &self->counters[index];
}



int Store_getNumberOfOpenedCounters(Store *self){
	int i = 0, openedCounterCounter = 0;

	for(; i < self->nCounters; i++){
		if(Counter_getDuration(&self->counters[i]) == -1){
			openedCounterCounter++;
		}
	}

	return openedCounterCounter;
}

/**
* Store's shared memory operations
**/

Store * Store_getOpenedStore(const char smem[]){
	Store * store;
	int shmfd;

	if((shmfd = shm_open(smem, O_RDWR,0600)) < 0){
    	return 0; // it's not an error. the memory was just not created yet.
	}

	__(store = mmap(0, sizeof(Store), PROT_READ | PROT_WRITE , MAP_SHARED, shmfd, 0),"Store_getOpenedStore, mmap",0);

	return store;
}


Store * Store_open(const char smem[]){
	Store * store = 0;
	int shmfd;

	if((store=Store_getOpenedStore(smem)) == 0){
		_(shmfd = shm_open(smem, O_RDWR|O_CREAT|O_EXCL,0600), "Store_open, shm_open", 0);
		_(ftruncate(shmfd, sizeof(Store)), "Store_open, ftruncate", 0);
		__(store = mmap(NULL, sizeof(Store), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0), "Store_open, mmap",0);

		if(Store_init(store) < 0){ // No semaphores, no store.
			Store_close(smem,store);
			return 0;
		}
	}

	return store;
}


int Store_close(const char smem[], Store * store){
	int r = Store_delete(store);

	_(munmap(store,sizeof(Store)), "Store_close, munmap", -1);
	_(shm_unlink(smem), "Store_close, shm_unlink", -2);

	return r;
}

