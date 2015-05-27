#include "Attendant.h"

#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>


static int log_fd;
static char * message;

int Attendant_new(Counter * counter, Store * store, char fifoName[], int fd, char * msg){
    Attendant * self = malloc(sizeof(Attendant));

    log_fd = fd;
    message = msg;

    strcpy(self->fifoName,fifoName);
    self->counter  = counter;
    self->store  = store;

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
    Store * selfStore = ((Attendant *)self)->store;

    Counter_clientArrives(selfCounter);
    pthread_mutex_unlock(&selfStore->m_choosingCounter);

    int fd;
    if ((fd = open(selfFifoName, O_WRONLY)) != -1){
        printf("FIFO '%s' openned in O_WRONLY mode\n",selfFifoName);
    }


    // WORKING ...
    int workingTime = rand() % 11;
    sleep(workingTime);


    write(fd, "fim_atendimento", strlen("fim_atendimento")+1); //TODO ha possibilidade de nao ser escrita a mensagem toda de 1x?
    writeToFile(log_fd, "Balcao", selfCounter->index, "fim_atend_cli", message);
    printf("-->[%s]: %s\n",selfFifoName , "fim_atendimento");


    //pthread_mutex_lock(&selfStore->m_choosingCounter);
        Counter_clientLeaves(selfCounter,workingTime);
    //pthread_mutex_unlock(&selfStore->m_choosingCounter);

    close(fd);
    free(self);
    return 0;
 }
