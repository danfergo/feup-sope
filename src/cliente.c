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

#define _(X,Y,Z)		if((X) < 0){perror(Y); exit(Z);}
#define __(X,Y,Z)		if((X) == 0){perror(Y); return Z;}

typedef struct client {
  char fifoName[MAX_FIFO_NAME_LEN];
  Store * store;
  Counter * counter;
} Client;

static int fd_log;

/**
1
**/
void Client_init(Client * self, const char smem[]){

    if((self->store = Store_getOpenedStore(smem)) == 0){
        printf("Store isn't opend yet. Come back later. \n");
        exit(11);
    }

    sprintf(self->fifoName, "/tmp/fc_%d", getpid());
    _(mkfifo(self->fifoName, 0660), "Client_init, mkfifo", 12);

    //Escolhe o balcão com menor número de clientes em atendimento
    self->counter = Store_getFreerCounter(self->store);
    writeToFile(fd_log, "Client", Counter_getIndex(self->counter), "pede_atendimento", self->fifoName);
}


/**
2
**/
void Client_callCounter(Client * self){
    int fd;

    _(fd = open(Counter_getFifoName(self->counter), O_WRONLY), "Client_callCounter, open", 21);
    printf("FIFO '%s' openned in WRITE mode\n", self->fifoName);

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

    _(fd = open(self->fifoName, O_RDONLY), "Client_waitReply, open", 31);
    printf("FIFO '%s' openned in READ mode\n", self->fifoName);

    do {
        readBytes = read(fd, message, MAX_MSG_LENGTH);
        writeToFile(fd_log, "Client", Counter_getIndex(self->counter), "fim_atendimento", self->fifoName);
        printf("[%s]<--:%s\n" ,self->fifoName, message);
    } while(readBytes > 0 && strcmp(message, "fim_atendimento") != 0);



    close(fd);
}


/**
4
**/
void Client_close(Client * self){
    _(unlink(self->fifoName), "Client_close, unlink", 41);
    printf("FIFO '%s' has been destroyed\n", self->fifoName);
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

    fd_log = openLogFile(argv[1]);
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