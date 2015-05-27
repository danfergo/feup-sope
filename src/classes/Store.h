#ifndef STORE_H
#define STORE_H

#include <pthread.h>

#include "Counter.h"

#define MAX_COUNTERS 10

typedef struct Store {
    int openingTime;
    int nCounters;
    Counter counters[MAX_COUNTERS];
    pthread_mutex_t m_choosingCounter;
    pthread_mutex_t m_ending;
} Store;

int Store_init(Store * self);
int Store_delete(Store * self);

int Store_getNumberOfOpenedCounters(Store *self);

Counter * Store_getFreerCounter(Store * self);
Counter * Store_getOpenedCounter(Store * self, int index);

Counter * Store_openCounter(Store * self);

Store * Store_getOpenedStore(const char smem[]);
Store * Store_open(const char smem[]);
int Store_close(const char smem[], Store * store);

void getTimeFormated(char * buffer);
void writeStatisticsTable();
int openLogFile();
void writeToFile(int fd, char * who, int counter, char * what, char * channel);

#endif