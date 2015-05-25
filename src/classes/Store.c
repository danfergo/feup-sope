#include "Store.h"

#include <unistd.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>        /* For mode constants */
#include <sys/types.h>       /* For ftruncate */
#include <fcntl.h>           /* For O_* constants */


void Store_init(Store * self){
	self->openingTime = 100;
	self->nCounters = 0;
}


Counter * Store_getFreerCounter(Store * self){
	int i = 0, freerCounterIndex = 0;

	for(; i < self->nCounters; i++){
		if(self->counters[i].nClientsInService < self->counters[freerCounterIndex].nClientsInService){
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

void Store_closeCounter(Store * self, const Counter * counter){
	self->nCounters--;
}


Counter *  Store_openCounter(Store * self){
	Counter_init(&(self->counters[self->nCounters]), self->nCounters);
	self->nCounters++;
	return &(self->counters[self->nCounters-1]);
}


/**
* creating and deleting shared memory.
**/





Store * Store_getOpenedStore(const char smem[]){
	Store * store;
	int shmfd = shm_open(smem, O_RDWR,0600);
	if(shmfd < 0){
    	return 0; // it's not an error. just the memory was not created yet.
	}
	store = mmap(0, sizeof(Store), PROT_READ | PROT_WRITE , MAP_SHARED, shmfd, 0);
	if(store == 0 ){
		perror("asds111adads");
		return 0;
	}

	return store;
}


Store * Store_open(const char smem[]){
	Store * store = 0;

	if((store=Store_getOpenedStore(smem)) == 0){
		int shmfd = shm_open(smem, O_RDWR|O_CREAT|O_EXCL,0600);
		if(shmfd < 0){
			perror("");
			return 0;
		}

		if(ftruncate(shmfd, sizeof(Store)) < 0){
			perror("qweeqweqw");
			return 0;
		}

		store = mmap(NULL, sizeof(Store), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd, 0);
		if(store < 0 ){
			perror("asdsadads");
			return 0;
		}

		Store_init(store);
	}

	return store;
}


int Store_close(const char smem[], Store * store){


	if (munmap(store,sizeof(Store)) < 0){
		perror("error unmaping shared memory");
		return -1;
	}

	if (shm_unlink(smem) < 0){
		perror("error unlinking shared memory");
		return -2;
	}

	return 0;
}

