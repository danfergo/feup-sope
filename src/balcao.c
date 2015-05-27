#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>

#include "classes/Store.h"
#include "classes/Attendant.h"

#define _(X,Y,Z)		if((X) < 0){perror(Y); exit(Z);}

#define MAX_MSG_LENGTH  2048

char smem[1024];
int counterFifoFd;
Store * store;
Counter * counter;
int duration;
static int log_fd;

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
    writeToFile(log_fd, "Balcao", counter->index, "fecha_balcao", counter->fifoName);

    pthread_mutex_lock(&m_changing_nClientsInService);

    while (counter->nClientsInService < 0)
        pthread_cond_wait(&c_nClientsInService_changed,&m_changing_nClientsInService);

    pthread_mutex_unlock(&m_changing_nClientsInService);


    if(Store_getNumberOfOpenedCounters(store) == 0){

        while(counter->nClientsInService > 0);
        writeStatisticsTable(store);

        writeToFile(log_fd, "Balcao", counter->index, "fecha_loja", counter->fifoName);
        close(log_fd);
        Store_close(smem, store);

    }

}


int main(int argc, const char* argv[],const char* envp[]) {

    if (argc < 3){
        printf("Usage: balcao <nome_mempartilhada> <duração> \n"); //TODO change to the correct function call
        exit(1);
    }

    log_fd = openLogFile(argv[1]);

    strcpy(smem,argv[1]);
    duration = atoi(argv[2]);
    signal(SIGINT,interrupted);
    signal(SIGALRM,timeExpired);
    setbuf(stdout, NULL);
    _(alarm(duration),"main, alarm", 2);

    store = Store_open(smem);
    counter = Store_openCounter(store);


    if(store->nCounters == 1){
        char string[512];
        char * separator = "----------------------------------------------------------------------------------\n";

        sprintf(string, " %-19s | %-6s | %-6s | %-20s | %-16s\n", "Quando", "Quem", "Balcao", "O_que", "Canal_criado/usado");

        write(log_fd, string, strlen(string));
        write(log_fd, separator, strlen(separator));
        writeToFile(log_fd, "Balcao", counter->index, "inicia_mempart", "-");
    }

    writeToFile(log_fd, "Balcao", counter->index, "cria_linh_mempart", counter->fifoName);

    printf("N open counters: %d \n", store->nCounters);

    _( counterFifoFd = open(counter->fifoName,  O_RDWR), "main, open", 3);


    char message[MAX_MSG_LENGTH];
    printf("Listening on %s: \n", counter->fifoName);
    while(1){
        read(counterFifoFd, message, MAX_MSG_LENGTH);
        writeToFile(log_fd, "Balcao", counter->index, "inicia_atend_cli", message);
        printf("[%s]<--: %s\n", counter->fifoName, message);
        Attendant_new(counter, message, log_fd, message);

    }


    leave();
    return 0;
}