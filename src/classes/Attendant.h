#ifndef ATTENDANT_H
#define ATTENDANT_H

typedef struct attendant {
    int fifo_descriptor;
    int duration;
} Attendant;

Attendant * attendant_new(int fifo_descriptor);
Attendant * attendant_run(Attendant * self);
void attendant_delete(Attendant * self);

#endif