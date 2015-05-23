#include <unistd.h>
#include <stdio.h>

#include "classes/Store.h"
#include "classes/Counter.h"


#define MAX_MSG_LENGTH     2048


int main(int argc, const char* argv[]) {
    pid_t pid = getpid();

    char smem[];
    //verfiy arguments //TODO


    //crate private fifo
    char cFifoPath[MAX_FIFO_NAME_LEN] = "/tmp/fc_";
    sprintf(str, "%d", pid);

    if(mkfifo(cFifoPath, 0777) < 0){ // TODO what mode ?
        perror("making private fifo");
    }

    //acede à Tabela de Balcões, partilhada, para ver os balcões abertos
    Store store;
    if(smem_getOpenedStore(smem, &store) == -1){
        printf("Store isn't opend yet. Come back later. \n");
        return 1;
    }

    //Escolhe o balcão com menor número de clientes em atendimento
    Counter counter;
    Store_getFreerCounterIndex(&store,&counter); //TODO check erros


    int bFifoFd = open(counter->fifoName, WRONLY); //TODO check errors
    write(bFifoFd,message,MAX_MSG_LENGTH); //TODO  ha possibilidade de nao ser escrita a mensagem toda de 1x?


    char message[MAX_MSG_LEGTH];
    int cFifoFd = open (cFifoPath, O_RDONLY), readBytes; //TODO check errors
    do {
        readBytes = read(cFifoFd,message,MAX_MSG_LENGTH);
    }while(readBytes != 0 && strcmp(message,"fim_atendimento") != 0);

    if(readBytes != 0){     //TODO improve message handle.
        printf("Success.");
    }

    close (cFifoFd);  //TODO check errors
    unlink(cFifoPath);  //TODO check errors
    return 0;
}