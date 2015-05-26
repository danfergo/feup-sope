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
    /*TODO simula o atendimento
    adormecendo por um tempo em segundos igual
    ao no de clientes do balcão mais uma unidade,
    mas sujeito a um máximo de 10 segundos */
    char * selfFifoName = ((Attendant *)self)->fifoName;

    int fd;
    if ((fd = open(selfFifoName, O_WRONLY)) != -1){
        printf("FIFO '%s' openned in O_WRONLY mode\n",selfFifoName);
    }


    // WORKING ...
    sleep(rand() % 11);


    write(fd, "fim_atendimento", strlen("fim_atendimento")+1); //TODO ha possibilidade de nao ser escrita a mensagem toda de 1x?
    printf("-->[%s]: %s\n",selfFifoName , "fim_atendimento");

    /* TODO Antes
   e depois de efetuar o atendimento, acede à linha
   desse balcão, na Tabela de Balcões, e atualiza a
   informação relevante, como o no de clientes em
   atendimento ou o no de clientes atendidos e o
   tempo médio de atendimento por cliente */

    close(fd);
    free(self);

    return 0;
 }
