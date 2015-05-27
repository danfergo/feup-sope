#ifndef ATTENDANT_H
#define ATTENDANT_H

#include "Counter.h"
#include "Store.h"

typedef struct attendant {
    Counter * counter;
    int duration;
    char fifoName[150];
} Attendant;

int  Attendant_new(Counter * counter, char fifoName[], int fd, char * message);
void * Attendant_run(void * self);
void Attendant_delete(Attendant * self);

#endif