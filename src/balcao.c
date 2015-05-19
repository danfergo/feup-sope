#include <stdio.h>

#include "classes/Store.h"

int main(int argc, const char* argv[]) {

  /*TODO inicia a sua operação verificando se a memória partilhada nomeada já existe e,
  se não existir, cria-a e inicializa as variáveis globais nela contidas */

  //TODO cria um FIFO para atendimento de clientes.

  /*TODO Na memória partilhada atualiza as variáveis relevantes,
   como a que guarda o no de balcões disponíveis,
   e cria e preenche uma linha nova na Tabela de Balcões da memória partilhada */

  /*TODO criar e iniciar as outras variáveis locais necessárias (e.g.
     variáveis de sincronização locais, necessárias ao acesso correto de todos os threads do processo à linha
     referente ao balcão, na Tabela de Balcões da memória partilhada global) */

  while(1){
    //TODO coloca-se numa espera bloqueante no FIFO do balcão enquanto um novo cliente não chega

    //TODO gera um thread de atendimento (passando-lhe como parâmetro a identificação do FIFO do cliente)
  }

  return 0;
}