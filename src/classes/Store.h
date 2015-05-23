#ifndef STORE_H
#define STORE_H


#include "Counter.h"

#define MAX_COUNTERS 10

typedef struct Store {
    int openingTime;
    int nCounters;
    Counter counters[MAX_COUNTERS];
} Store;

void Store(Store * self);
int Store_open(Store * self);
int Store_close(Store * self);

void Store_getFreerCounter(Store * self, Counter * counter);
int Store_getOpenedCounter(Store * self,int index, Counter * counter);




int smem_getOpenedStore(char smem[], Store * store);
int smem_openStore(char smem[], Store * store);
int smem_closeStore(char smem[], Store * store);



#endif