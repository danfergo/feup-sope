#ifndef BALCAO_H
#define BALCAO_H

#define MAX_FIFO_NAME_LEN   1024

typedef struct balcao {
    int numero;
    int tempo_abertura;
    int duracao;
    char nome_fifo[MAX_FIFO_NAME_LEN];
    int em_atendimento;
    int ja_atendidos;
    int tempo_medio_atendimento;
} Balcao;


void init_balcao(Balcao * b);

#endif