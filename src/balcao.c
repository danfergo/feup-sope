#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <signal.h>

#include "classes/Store.h"
#include "classes/Attendant.h"



#define MAX_MSG_LENGTH  2048

char smem[1024];
FILE * counterFifo;
Store * store;
Counter * counter;

void leave(int signal){
    Store_closeCounter(store,counter);
    fclose(counterFifo);

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
    signal(SIGINT,leave);
    setbuf(stdout, NULL);

    store = Store_open(smem);
    counter = Store_openCounter(store);

    printf("N open counters: %d \n", store->nCounters);

    counterFifo = fopen(counter->fifoName, "r+");
    if (counterFifo == NULL){
        perror ("Error opening file");
        return 1;
    }


    /** TODO criar e iniciar as outras variáveis locais necessárias (e.g.
    variáveis de sincronização locais, necessárias ao acesso correto de todos os threads do processo à linha
    referente ao balcão, na Tabela de Balcões da memória partilhada global) **/


    char message[MAX_MSG_LENGTH];

    printf("Listening on %s: \n", counter->fifoName);
    while(1){
        fscanf(counterFifo, "%s", message);
        //read(fileno(counterFifo), message, 12);
        printf("[%s]<--%s", counter->fifoName, message);
        Attendant_run(Attendant_new(counter, message));
    }

    leave(0);

    return 0;
}