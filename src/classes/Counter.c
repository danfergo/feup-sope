#include "Counter.h"

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>

pthread_cond_t c_nClientsInService_changed = PTHREAD_COND_INITIALIZER;
pthread_mutex_t m_changing_nClientsInService = PTHREAD_MUTEX_INITIALIZER;

pthread_mutex_t m_changing_doingStats = PTHREAD_MUTEX_INITIALIZER;

#define _(X,Y,Z)		if((X) < 0){perror(Y); return Z;}

int Counter_getNClientsInService(Counter * self){
    return self->nClientsInService;
}

char * Counter_getFifoName(Counter * self){
    return self->fifoName;
}

int Counter_getDuration(Counter * self){
    return self->duration;
}

int Counter_getIndex(Counter * self){
    return self->index;
}

int Counter_init(Counter * self, int index){

    self->index = index;
    self->openingTime = time(NULL);
    self->duration = -1;
    self->nClientsInService = 0;
    self->alreadyAttended = 0;
    self->serviceAverageDuration = 0.0f;

    printf("open counter \n");

    sprintf(self->fifoName, "/tmp/fb_%d", getpid());

    _(mkfifo(self->fifoName, 0660), "Counter_init, mkfifo",0);

    return 0;
}

void Counter_close(Counter * self, int duration){
      self->duration = duration;
}

void Counter_waitToEnd(Counter * self){

    pthread_mutex_lock(&m_changing_nClientsInService);

        while (self->nClientsInService > 0)
            pthread_cond_wait(&c_nClientsInService_changed,&m_changing_nClientsInService);

    pthread_mutex_unlock(&m_changing_nClientsInService);

}


void Counter_clientArrives(Counter * self){
    pthread_mutex_lock(&m_changing_nClientsInService);
    self->nClientsInService++;
    pthread_cond_signal(&c_nClientsInService_changed);
    pthread_mutex_unlock(&m_changing_nClientsInService);
}

void Counter_clientLeaves(Counter * self , int duration){
    pthread_mutex_lock(&m_changing_nClientsInService);
    self->nClientsInService--;
    pthread_cond_signal(&c_nClientsInService_changed);
    pthread_mutex_unlock(&m_changing_nClientsInService);

    pthread_mutex_lock(&m_changing_doingStats);
        self->serviceAverageDuration = (self->alreadyAttended*self->serviceAverageDuration +  duration)/(double)(self->alreadyAttended + 1);
        self->alreadyAttended++;
    pthread_mutex_unlock(&m_changing_doingStats);

    self->serviceAverageDuration = (self->alreadyAttended*self->serviceAverageDuration +  duration)/(double)(self->alreadyAttended + 1);
    self->alreadyAttended++;
}