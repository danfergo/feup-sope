#include <stdio.h>

#include "classes/Store.h"
#include "classes/Counter.h"

/**
Nome do FIFO cliente: tem o formato "fc_pid", em que pid é o identificador de sistema do processo
cliente (diferente de ger_cl, por via do fork()...), e é relativo ao diretório /tmp
**/
int main(int argc, const char* argv[]) {

  //TODO cria um FIFO privado

  //TODO acede à Tabela de Balcões, partilhada, para ver os balcões abertos e número de cliente em atendimento em cada um

  //TODO Escolhe o balcão com menor número de clientes em atendimento

  //TODO envia ao FIFO desse balcão a identificação do seu FIFO privado
  //TODO Bloqueia neste FIFO, esperando a chegada da mensagem "fim_atendimento"

  //TODO termina de seguida, eliminando antes o FIFO criado
  return 0;
}