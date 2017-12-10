CC=gcc
CFLAGS= -std=c11 -Wall
DBGINFO= 
all: lock


lock: main.o btoothlib.o netlib.o locklib.o
	$(CC) $(CFLAGS) main.o netlib.o locklib.o btoothlib.o -o lock -lbluetooth $(DBGINFO)

main.o:
	$(CC) $(CFLAGS) -c main.c -o main.o $(DBGINFO)

netlib.o:
	$(CC) $(CFLAGS) -D_BSD_SOURCE -c netlib.c -o netlib.o $(DBGINFO)

locklib.o:
	$(CC) $(CFLAGS) -c locklib.c -o locklib.o $(DBGINFO)

btoothlib.o:
	$(CC) $(CFLAGS) -c btoothlib.c -o btoothlib.o -lbluetooth $(DBGINFO)
	


clear:
	rm *.o lock

