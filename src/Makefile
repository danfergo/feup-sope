all: makedirs cliente balcao ger_cl


#compiling

balcao: balcao.o Loja.o Balcao.o
	gcc -Wall ../out/cliente.o ../out/classes/Loja.o ../out/classes/Balcao.o -o ../bin/balcao

cliente: cliente.o Loja.o Balcao.o
	gcc -Wall ../out/cliente.o ../out/classes/Loja.o ../out/classes/Balcao.o -o ../bin/cliente

ger_cl: ger_cl.o
	gcc -Wall ../out/ger_cl.o -o ../bin/ger_cl


#linking all

balcao.o: balcao.c classes/Loja.h classes/Balcao.h
	gcc -Wall -c balcao.c -o ../out/balcao.o

cliente.o: cliente.c classes/Loja.h classes/Balcao.h
	gcc -Wall -c cliente.c -o ../out/cliente.o

ger_cl.o: ger_cl.c
	gcc -Wall -c ger_cl.c -o ../out/ger_cl.o


# classes
Balcao.o: classes/Balcao.h classes/Balcao.c
	cc -Wall -c classes/Balcao.c -o ../out/classes/Balcao.o

Loja.o: classes/Loja.h classes/Loja.c
	cc -Wall -c classes/Loja.c -o ../out/classes/Loja.o


# auxiliar
makedirs:
	mkdir -p ../out; mkdir -p ../out/classes; mkdir -p ../bin

clean:
	rm -rf ../out/