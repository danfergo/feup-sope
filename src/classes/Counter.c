#include "Counter.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>


void Counter_init(Counter * self, int index){

  self->index = index;

  printf("open counter \n");


  sprintf(self->fifoName, "/tmp/fb_%d", getpid());

  if (mkfifo(self->fifoName, 0660) < 0){
        perror("Opening fifo");
  }
}