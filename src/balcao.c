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

#define MAX_MSG_LENGTH  2048

char smem[1024];
int counterFifoFd;
Store * store;
Counter * counter;

void leave();

void timeExpired(int signal){
    printf("Time expired");
    leave();
    pthread_exit(NULL);
}

void interrupted(int signal){
    leave();
    exit(0);
}


void leave(){
    Store_closeCounter(store,counter);
    close(counterFifoFd);

    if(store->nCounters == 0){
        Store_close(smem, store);
    }

}


int main(int argc, const char* argv[],const char* envp[]) {

    if (argc < 3){
        printf("Usage: balcao <nome_mempartilhada> <duração> \n"); //TODO change to the correct function call
        exit(1);
    }

    strcpy(smem,argv[1]);
    signal(SIGINT,interrupted);
    signal(SIGALRM,timeExpired);
    setbuf(stdout, NULL);

    if(alarm(atoi(argv[2])) < 0){
        //TODO
    }

    store = Store_open(smem);
    counter = Store_openCounter(store);

    printf("N open counters: %d \n", store->nCounters);

    counterFifoFd = open(counter->fifoName,  O_RDWR);
    if (counterFifoFd < 0){
        perror ("Error opening file");
        return 1;
    }


    /** TODO criar e iniciar as outras variáveis locais necessárias (e.g.
    variáveis de sincronização locais, necessárias ao acesso correto de todos os threads do processo à linha
    referente ao balcão, na Tabela de Balcões da memória partilhada global) **/


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