#include "Counter.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>


int Counter_getNClientsInService(Counter * self){
    return self->nClientsInService;
}

char * Counter_getFifoName(Counter * self){
    return self->fifoName;
}



void Counter_init(Counter * self, int index){

  self->index = index;
  self->openingTime = 0; //TODO
  self->duration = -1;
  self->nClientsInService = 0;
  self->alreadyAttended = 0;
  self->serviceAverageDuration = 0.0f;

  printf("open counter \n");


  sprintf(self->fifoName, "/tmp/fb_%d", getpid());

  if (mkfifo(self->fifoName, 0660) < 0){
        perror("Opening fifo");
  }
}

void Counter_clientArrives(Counter * self){
  self->nClientsInService++;
}

void Counter_clientLeaves(Counter * self , int duration){
  self->serviceAverageDuration = (self->alreadyAttended*self->serviceAverageDuration +  duration)/(double)(self->alreadyAttended + 1);
  self->nClientsInService--;
  self->alreadyAttended++;
}