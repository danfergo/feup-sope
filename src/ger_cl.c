#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, const char* argv[]) {

  if(argc < 3){
    printf("Usage: \n\tger_cl <nome_mempartilhada> <num_clientes> \n");
  }

  int nClients = atoi(argv[2]); //TODO make this conversion bullet prof.
  int i, pid;

  for(i = 0; i < nClients; i++){

      pid = fork();

      if(pid == 0){
          // exec.
          printf("x \n");
          execl("./cliente","cliente",NULL); // TODO this thing does not work.
          return 0;
      }else if(pid < 0){
          perror("when forking to launch client");
          return 1;
      }
  }

  return 0;
}
