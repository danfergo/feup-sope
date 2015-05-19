#include <stdio.h>

#include "classes/Loja.h"
#include "classes/Balcao.h"

int main(int argc, const char* argv[]) {

  Balcao b;
  init_balcao(&b);

  printf("%d \n", b.tempo_abertura);

  return 0;
}