#ifndef LOJA_H
#define LOJA_H


#include "Balcao.h"

#define MAX_BALCOES 10

typedef struct loja {
    int nBalcoes;
    Balcao balcoes[MAX_BALCOES];

} Loja;

#endif