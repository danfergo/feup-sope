#ifndef STORE_H
#define STORE_H


#include "Counter.h"

#define MAX_COUNTERS 10

typedef struct Store {
    int nCounters;
    Counter counters[MAX_COUNTERS];
} Store;

#endif