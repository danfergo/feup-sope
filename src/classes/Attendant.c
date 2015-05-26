#include "Attendant.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>


/**
#define
srand(time(NULL));
**/

int Attendant_new(Counter * counter, char fifoName[]){
    Attendant * self = malloc(sizeof(Attendant));

    strcpy(self->fifoName,fifoName);
    self->counter  = counter;

    pthread_t tid;
    if (pthread_create(&tid, NULL,Attendant_run, self) < 0){
        free(self);
        return -1;
    }

    if (pthread_detach (tid) < 0){
        free(self);  //TODO how to react here ?
        return -2;
    }

    return 0;
}


void * Attendant_run(void * self){
    char * selfFifoName = ((Attendant *)self)->fifoName;
    Counter * selfCounter = ((Attendant *)self)->counter;

    Counter_clientArrives(selfCounter);

    int fd;
    if ((fd = open(selfFifoName, O_WRONLY)) != -1){
        printf("FIFO '%s' openned in O_WRONLY mode\n",selfFifoName);
    }


    // WORKING ...
    int workingTime = rand() % 11;
    sleep(workingTime);


    write(fd, "fim_atendimento", strlen("fim_atendimento")+1); //TODO ha possibilidade de nao ser escrita a mensagem toda de 1x?
    printf("-->[%s]: %s\n",selfFifoName , "fim_atendimento");


    Counter_clientLeaves(selfCounter,workingTime);

    close(fd);
    free(self);
    return 0;
 }
