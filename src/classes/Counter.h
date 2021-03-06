#ifndef COUNTER_H
#define COUNTER_H

#include <pthread.h>
#include <time.h>


#define MAX_FIFO_NAME_LEN   1024
#define MAX_MSG_LENGTH  2048

typedef struct counter {
    int index;
    int openingTime;
    int duration;
    char fifoName[MAX_FIFO_NAME_LEN];
    int nClientsInService;
    int alreadyAttended;
    int serviceAverageDuration;
} Counter;

int Counter_getNClientsInService(Counter * self);
char * Counter_getFifoName(Counter * self);
int Counter_getDuration(Counter * self);
int Counter_getIndex(Counter * self);

void Counter_waitToEnd(Counter * self);

int Counter_init(Counter * self, int index);
void Counter_close(Counter * self, int duration);

void Counter_clientArrives(Counter * self);
void Counter_clientLeaves(Counter * self , int duration);


#endif