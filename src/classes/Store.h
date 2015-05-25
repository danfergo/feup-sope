#ifndef STORE_H
#define STORE_H


#include "Counter.h"

#define MAX_COUNTERS 10

typedef struct Store {
    int openingTime;
    int nCounters;
    Counter counters[MAX_COUNTERS];
} Store;

void Store_init(Store * self);

Counter * Store_getFreerCounter(Store * self);
Counter * Store_getOpenedCounter(Store * self, int index);
Counter * Store_openCounter(Store * self);
void Store_closeCounter(Store * self,const Counter * counter);



Store * Store_getOpenedStore(const char smem[]);
Store * Store_open(const char smem[]);
int Store_close(const char smem[], Store * store);



#endif