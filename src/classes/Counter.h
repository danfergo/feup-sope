#ifndef COUNTER_H
#define COUNTER_H

#define MAX_FIFO_NAME_LEN   1024

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


void Counter_init(Counter * self, int index);
void Counter_clientArrives(Counter * self);
void Counter_clientLeaves(Counter * self , int duration);


#endif