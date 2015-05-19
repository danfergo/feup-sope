#ifndef COUNTER_H
#define COUNTER_H

#define MAX_FIFO_NAME_LEN   1024

typedef struct counter {
    int number;
    int opening_time;
    int duration;
    char fifo_name[MAX_FIFO_NAME_LEN];
    int in_service;
    int already_attended;
    int service_average_duration;
} Counter;


void init_counter(Counter * b);

#endif