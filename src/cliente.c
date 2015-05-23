#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#include "classes/Store.h"
#include "classes/Counter.h"

#define MAX_MSG_LENGTH  2048
#define MAX  100

int main(int argc, const char* argv[]) {
    int fc_fd, fb_fd, readBytes;
    char fifo[MAX]; //TODO change size
    char smem[MAX]; //TODO change size
    char message[MAX_MSG_LENGTH];
    
    if (argc != 1){
        printf("Usage: client <nome_mempartilhada> <num_clientes> \n"); //TODO change to the correct function call
        return 1;
    }
    
    //TODO cria um FIFO privado
    sprintf(fifo, "/tmp/fc_%d", getpid());
    
    if (mkfifo(fifo, 0660) < 0){
        if (errno==EEXIST){
            printf("FIFO '%s' already exists\n", fifo);
            return 2;
        }
        else {
            printf("Can't create FIFO\n");
            return 3;
        }
    }
        else {
        printf("FIFO '%s' sucessfully created\n", fifo);
    }

/*
    //acede à Tabela de Balcões, partilhada, para ver os balcões abertos
    Store store;
    if(smem_getOpenedStore(smem, &store) == -1){
        printf("Store isn't opend yet. Come back later. \n");
        return 1;
    }

    //Escolhe o balcão com menor número de clientes em atendimento
    Counter counter;
    Store_getFreerCounter(&store,&counter); //TODO check erros


    //int bFifoFd = open(counter->fifoName, WRONLY); //TODO check errors
    if ((fb_fd = open(counter.fifoName, O_WRONLY)) != -1){
        printf("FIFO '%s' openned in WRITE mode\n", fifo);
    }
    
    strcpy(message, fifo); //TODO envia ao FIFO desse balcao a identificação do seu FIFO privado
    write(fb_fd, message, MAX_MSG_LENGTH); //TODO ha possibilidade de nao ser escrita a mensagem toda de 1x?

    //int cFifoFd = open (cFifoPath, O_RDONLY), readBytes; //TODO check errors

    if ((fc_fd = open(fifo, O_RDONLY)) != -1){
        printf("FIFO '%s' openned in READ mode\n", fifo);
    }

    do {
        readBytes = read(fc_fd, message, MAX_MSG_LENGTH);
    } while(readBytes != 0 && strcmp(message, "fim_atendimento") != 0);

    if(readBytes != 0){     //TODO improve message handle.
        printf("Success.");
    }

    close(fb_fd);
    close(fc_fd);*/
    
    
    if (unlink(fifo) < 0){
        printf("Error when destroying FIFO '%s'\n", fifo);
        return 4;
    }
    else {
        printf("FIFO '%s' has been destroyed\n", fifo);
    }
    return 0;
}