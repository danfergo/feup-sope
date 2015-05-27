#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>

#include "classes/Store.h"
#include "classes/Attendant.h"

#define _(X,Y,Z)		if((X) < 0){perror(Y); exit(Z);}

#define MAX_MSG_LENGTH  2048

char smem[1024];
int counterFifoFd;
Store * store;
Counter * counter;
int duration;

void leave();

void timeExpired(int signal){
    printf("Time expired");
    leave();
    exit(0);
}

void interrupted(int signal){
    leave();
    exit(0);
}


void leave(){

    _(unlink(counter->fifoName),"leave, unlink", 23);

    Counter_close(counter,duration);
    close(counterFifoFd);

    pthread_mutex_lock(&m_changing_nClientsInService);

        while (counter->nClientsInService < 0)
            pthread_cond_wait(&c_nClientsInService_changed,&m_changing_nClientsInService);

    pthread_mutex_unlock(&m_changing_nClientsInService);

    if(Store_getNumberOfOpenedCounters(store) == 0){
        Store_close(smem, store);
    }
}


int main(int argc, const char* argv[],const char* envp[]) {

    if (argc < 3){
        printf("Usage: balcao <nome_mempartilhada> <duração> \n"); //TODO change to the correct function call
        exit(1);
    }

    strcpy(smem,argv[1]);
    duration = atoi(argv[2]);
    signal(SIGINT,interrupted);
    signal(SIGALRM,timeExpired);
    setbuf(stdout, NULL);
    _(alarm(duration),"main, alarm", 2);

    store = Store_open(smem);
    counter = Store_openCounter(store);

    printf("N open counters: %d \n", store->nCounters);

    _( counterFifoFd = open(counter->fifoName,  O_RDWR), "main, open", 3);


    char message[MAX_MSG_LENGTH];
    printf("Listening on %s: \n", counter->fifoName);
    while(1){
        read(counterFifoFd, message, MAX_MSG_LENGTH);
        printf("[%s]<--: %s\n", counter->fifoName, message);
        Attendant_new(counter, message);
    }

    leave();
    return 0;
}