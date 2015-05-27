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

void leave();

void getTimeFormated(char * buffer){
    time_t rawtime;
    struct tm * timeinfo;

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    strftime (buffer, 80, "%F %T", timeinfo);
}

void writeStatisticsTable(){
	char table[MAX_MSG_LENGTH];
	int i = 0;

	sprintf(table, "\nBalcao |     Abertura    |  Nome   |          Num_clientes         | Tempo_medio\n #     | Tempo | Duracao |  FIFO   | em_atendimento | ja_atendidos | atendimento\n------------------------------------------------------------------------------\n");

	//printf("%d\n", store->nCounters);
	for(; i < store->nCounters; i++){
		//printf("a\n");
		sprintf(
			table + strlen(table),
			" %-6d| %-6d| %-8d| %-8s| %-15d| %-13d| %-11d\n",
			store->counters[i].index,
			store->counters[i].openingTime%100000,
			store->counters[i].duration,
			rindex(store->counters[i].fifoName,'/') + 1,
			store->counters[i].nClientsInService,
			store->counters[i].alreadyAttended,
			store->counters[i].serviceAverageDuration
		);
	}


	printf("%s", table);
}

void writeToFile(int fd, char * who, int counter, char * what, char * channel){
    char string[MAX_MSG_LENGTH];
    char time[80];
    getTimeFormated(time);
    sprintf(string, " %-19s | %-6s | %-6d | %-20s | %-16s\n", time, who, counter, what, channel);
    if(write(fd, string, strlen(string)) == -1){
        perror("Writing to file");
    }
}


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

        while(counter->nClientsInService > 0);
        writeStatisticsTable();

        Store_close(smem, store);


    }

}


int main(int argc, const char* argv[],const char* envp[]) {

    if (argc < 3){
        printf("Usage: balcao <nome_mempartilhada> <duração> \n"); //TODO change to the correct function call
        exit(1);
    }

    char logfile[MAX_MSG_LENGTH] = "..";

    strcpy(logfile + 2, argv[1]);

    int fd = open(logfile, O_RDWR|O_CREAT|O_APPEND, 0660);
    printf("%d\n", fd);


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

        write(fd, string, strlen(string));
        write(fd, separator, strlen(separator));
        writeToFile(fd, "Balcao", counter->index, "inicia_mempart", "-");
    }

    writeToFile(fd, "Balcao", counter->index, "cria_linh_mempart", counter->fifoName);

    printf("N open counters: %d \n", store->nCounters);

    _( counterFifoFd = open(counter->fifoName,  O_RDWR), "main, open", 3);


    char message[MAX_MSG_LENGTH];
    printf("Listening on %s: \n", counter->fifoName);
    while(1){
        read(counterFifoFd, message, MAX_MSG_LENGTH);
        printf("[%s]<--: %s\n", counter->fifoName, message);
        Attendant_new(counter, message);
    }

    close(fd);
    leave();
    return 0;
}