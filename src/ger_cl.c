#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>

#define MAX_CWD_LEN     1024
#define MAX_SMEM_LEN    1024
#define CLI_PATH_LEN    1124


int main(int argc, const char* argv[],const char* envp[]) {
  //int shmfd;
  //TODO Se a loja ainda não estiver aberta (i.e., se a memória partilhada não existir), o programa termina;

  if(argc < 3){
    printf("Usage: \n\tger_cl <nome_mempartilhada> <num_clientes> \n");
  }
  /**
  shmfd = shm_open(argv[1],O_RDONLY|O_CREAT,0600);
  
  if ((shmfd = shm_open(argv[1],O_RDONLY,0600)) == -1 ){
    perror("Shared memmory is not yet defined");
    return 1;
  }
  **/
  char cwd[MAX_CWD_LEN];
  if (getcwd(cwd, sizeof(cwd)) == NULL){
     perror("getting current directory");
     return 2;
  }

  char cli_path[CLI_PATH_LEN];
  strcpy(cli_path, cwd);
  strcat(cli_path, "/cliente");

  int nClients = atoi(argv[2]); //TODO make this conversion bullet prof.
  int i, pid;

  /* const char ** ptr = envp;
  while(*ptr != NULL){
        printf("%s \n", *ptr);
        ptr++;
  } **/
  
  for(i = 0; i < nClients; i++){

      pid = fork();

      if(pid == 0){
          //printf("%d \n", i);
          
          execle(cli_path, "cliente", NULL, envp);
          perror("launching client");
          return 2;

      }else if(pid < 0){
          perror("when forking to launch client");
          return 3;
      }
  }

  int status;
  for(i = 0; i < nClients; i++){
      if(wait(&status) == -1){
            perror("waiting for child client to end");
            return 4;
      }
  }

  return 0;
}
