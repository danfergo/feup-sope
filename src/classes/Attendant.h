#ifndef ATTENDANT_H
#define ATTENDANT_H

#include "Counter.h"

typedef struct attendant {
    Counter * counter;
    char fifo_path[150];
} Attendant;

Attendant * Attendant_new(Counter * counter, char fifo_path[]);
void Attendant_run(Attendant * self);
void Attendant_delete(Attendant * self);

#endif