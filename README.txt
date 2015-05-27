# feup-sope
Descrição geral
Numa loja virtual (ver figura junto), diversos balcões abrem para atendimento 
de clientes de um determinado serviço 
e operam durante um certo tempo. 
Cada cliente que chega à loja dirige-se ao balcão com menor número de 
clientes em fase de atendimento e notifica o balcão da sua chegada; 
seguidamente é atendido, o que demora um tempo variável. 
Um balcão permanece aberto durante um tempo que pode ser especificado. 
Quando o último balcão encerrar, a loja fecha. 
O último balcão, antes de encerrar, recolhe os dados disponíveis e gera 
estatísticas tais como o número total de clientes atendidos na loja e em cada 
balcão, o tempo médio de atendimento por
cliente, geral e por balcão e o tempo de abertura da loja e de cada balcão. 

## Utilização
A simulação do funcionamento da loja decorre mediante o arranque de dois tipos de programas, 
um ligado aos balcões de atendimento (**balcao**) e outro à geração de clientes (**ger_cl**). 

### Balcão:
O programa tipo ligado aos balcões é invocado na linha comando, tantas vezes quantas o utilizador quiser, 
por: 

    balcao <nome_mempartilhada> <tempo_abertura> 
    

### Cliente:
O programa tipo associado aos clientes é invocado na linha comando uma só vez, por: 

    ger_cl <nome_mempartilhada> <num_clientes> 
    
### Correr o Programa:
Para compilar o programa basta correr o compile.sh ou correr o comando make dentro da pasta src.
Os executaveis encontram-se na pasta bin. Em cima encontram-se os comandos para executar o programa.
Primeiro executa-se o balcão (e.x. balcao /mem_partilhada 10) e só depois é que se executa os clientes 
(e.x. ger_cl /mem_partilhada 5) ou abre-se outros balcões. Também vêm incluidos 2 scripts (run-balcao.sh e 
run-client.sh) que compilam e executam. Depois da execução são criados ficheiros .log com o nome atribuido 
à memoria partilhada.
