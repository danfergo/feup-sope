all: makedirs cliente balcao ger_cl


# compiling

balcao: balcao.o Store.o Counter.o Attendant.o
	gcc -Wall ../out/balcao.o ../out/classes/Store.o ../out/classes/Counter.o ../out/classes/Attendant.o -o ../bin/balcao -lrt

cliente: cliente.o Store.o Counter.o
	gcc -Wall ../out/cliente.o ../out/classes/Store.o ../out/classes/Counter.o -o ../bin/cliente -lrt

ger_cl: ger_cl.o
	gcc -Wall ../out/ger_cl.o -o ../bin/ger_cl -lrt


#linking all

balcao.o: balcao.c classes/Store.h classes/Counter.h classes/Attendant.h
	gcc -Wall -c balcao.c -o ../out/balcao.o

cliente.o: cliente.c classes/Store.h classes/Counter.h
	gcc -Wall -c cliente.c -o ../out/cliente.o

ger_cl.o: ger_cl.c
	gcc -Wall -c ger_cl.c -o ../out/ger_cl.o


# classes
Counter.o: classes/Counter.h classes/Counter.c
	cc -Wall -c classes/Counter.c -o ../out/classes/Counter.o

Store.o: classes/Store.h classes/Store.c
	cc -Wall -c classes/Store.c -o ../out/classes/Store.o
	
Attendant.o: classes/Attendant.h classes/Attendant.c
	gcc -Wall -c classes/Attendant.c -o ../out/classes/Attendant.o


# auxiliar
makedirs:
	mkdir -p ../out; mkdir -p ../out/classes; mkdir -p ../bin

clean:
	rm -rf ../out/