#include "Store.h"

void Store_init(Store * self){
	self->openingTime = 100;
	self->nCounters = 0;
}

int Store_open(Store * self){
	return 1;
}

int Store_close(Store * self){
	return 1;
}

void Store_addCounter(Store * self,int index, Counter * counter){
	
}

void Store_getFreerCounter(Store * self, Counter * counter){
	
}

int Store_getOpenedCounter(Store * self, int index, Counter * counter){
	return 1;
}

int smem_getOpenedStore(char smem[], Store * store){
	return 1;
}

int smem_openStore(char smem[], Store * store){
	return 1;
}

int smem_closeStore(char smem[], Store * store){
	return 1;
}