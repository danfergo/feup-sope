#include <time.h>
#include <stdlib.h>
#include "Attendant.h"

#define 
srand(time(NULL));


Attendant * attendant_new(int fifo_descriptor){

    Attendant * self = malloc(sizeof(struct Attendant)); //TODO malloc p/ struct.

    self->duration = rand() % 11;
    //TODO duration = random int 0 - 10
    self->fifo_descriptor = fifo_descriptor;
    return self;
}


void attendant_run(Attendant * self){
    /*TODO simula o atendimento
    adormecendo por um tempo em segundos igual
    ao no de clientes do balcão mais uma unidade,
    mas sujeito a um máximo de 10 segundos */

    /*TODO escreve no FIFO “fim_atendimento” */

    /* TODO Antes
   e depois de efetuar o atendimento, acede à linha
   desse balcão, na Tabela de Balcões, e atualiza a
   informação relevante, como o no de clientes em
   atendimento ou o no de clientes atendidos e o
   tempo médio de atendimento por cliente */


    return self;
}

void attendant_delete(Attendant * self){
    //TODO free self.
}