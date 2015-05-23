#ifndef COUNTER_H
#define COUNTER_H

#define MAX_FIFO_NAME_LEN   1024

typedef struct counter {
    int number;
    int openingTime;
    int duration;
    char fifoName[MAX_FIFO_NAME_LEN];
    int inService;
    int alreadyAttended;
    int serviceAverageDuration;
} Counter;


void Counter(Counter * self);

#endif