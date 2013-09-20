all: clean client server sfarsit linii

client: client.o culori.o interfata.o harta.o protocolp.o
	gcc -o client client.o culori.o interfata.o harta.o protocolp.o -lcurses

server: server.o vecini.o protocolp.o intrebare.o
	gcc -o server server.o vecini.o protocolp.o intrebare.o -lcurses

client.o:
	gcc -c client.c

server.o:
	gcc -c server.c

culori.o:
	gcc -c culori.c

interfata.o:
	gcc -c interfata.c

harta.o:
	gcc -c harta.c

protocolp.o:
	gcc -c protocolp.c

vecini.o:
	gcc -c vecini.c

intrebare.o:
	gcc -c intrebare.c

clean:
	rm -fr *.o client server iclient_* iserver_* j_*

sfarsit:
	rm -fr *.o

linii:
	clear
	wc -l culori.h interfata.h harta.h protocolp.h intrebare.h vecini.h macro.h culori.c interfata.c harta.c protocolp.c intrebare.c vecini.c client.c server.c Makefile
