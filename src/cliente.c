#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "classes/Store.h"
#include "classes/Counter.h"

#define MAX_MSG_LENGTH  2048

typedef struct client {
  char fifoName[MAX_FIFO_NAME_LEN];
  Store * store;
  Counter * counter;
} Client;

/**
1
**/
void Client_init(Client * self, const char smem[]){

    if((self->store = Store_getOpenedStore(smem)) == 0){
        printf("Store isn't opend yet. Come back later. \n");
        exit(11);
    }

    sprintf(self->fifoName, "/tmp/fc_%d", getpid());
    if (mkfifo(self->fifoName, 0660) < 0){
        perror("creating private fifo"); exit(12);
    }


    //Escolhe o balcão com menor número de clientes em atendimento
    self->counter = Store_getFreerCounter(self->store);
}


/**
2
**/
void Client_callCounter(Client * self){
    int fd;

    if ((fd = open(Counter_getFifoName(self->counter), O_WRONLY)) != -1){
        printf("FIFO '%s' openned in WRITE mode\n", self->fifoName);
    } //TODO error

    write(fd, self->fifoName, strlen(self->fifoName)+1); //TODO ha possibilidade de nao ser escrita a mensagem toda de 1x?
    printf("-->[%s]: %s\n",Counter_getFifoName(self->counter), self->fifoName);

    close(fd);
}


/**
3
**/
void Client_waitReply(Client * self){
    int fd, readBytes;
    char message[MAX_MSG_LENGTH];

    if ((fd = open(self->fifoName, O_RDONLY)) != -1){
        printf("FIFO '%s' openned in READ mode\n", self->fifoName);
    }

    do {
        readBytes = read(fd, message, MAX_MSG_LENGTH);
        printf("[%s]<--:%s\n" ,self->fifoName, message);
    } while(readBytes > 0 && strcmp(message, "fim_atendimento") != 0);


    close(fd);
}


/**
4
**/
void Client_close(Client * self){

    if (unlink(self->fifoName) < 0){
        printf("Error when destroying FIFO '%s'\n", self->fifoName);
        exit(31);
    }
    else {
        printf("FIFO '%s' has been destroyed\n", self->fifoName);
    }
}


/**
main - 5
**/
int main(int argc, const char* argv[]) {

    setbuf(stdout, NULL);

    if (argc < 2){
        printf("Usage: cliente <nome_mempartilhada>\n"); //TODO change to the correct function call
        return 1;
    }
    printf("Initializing client \n");

    Client client;
    Client_init(&client,argv[1]);

    printf("Creating private fifo and contacting balcao \n");
    Client_callCounter(&client);


    printf("Waiting for balcao to end\n");
    Client_waitReply(&client);

    printf("Deleting private fifo \n");
    Client_close(&client);
    return 0;
}