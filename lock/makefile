CC=gcc
CFLAGS= -std=c11 -Wall
DBGINFO= 
all: lock


lock: main.o btoothlib.o netlib.o locklib.o msglib.o RSA.o longdiv.o
	$(CC) $(CFLAGS) main.o netlib.o locklib.o btoothlib.o ../lib/msglib.o ../lib/RSA.o ../lib/longdiv.o -o lock -lbluetooth $(DBGINFO)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c -o main.o $(DBGINFO)

netlib.o: netlib.c
	$(CC) $(CFLAGS) -D_BSD_SOURCE -c netlib.c -o netlib.o $(DBGINFO)

locklib.o: locklib.c
	$(CC) $(CFLAGS) -c locklib.c -o locklib.o $(DBGINFO)

btoothlib.o: btoothlib.c
	$(CC) $(CFLAGS) -c btoothlib.c -o btoothlib.o -lbluetooth $(DBGINFO)

msglib.o: ../lib/msglib.c
	$(CC) $(CFLAGS) -c ../lib/msglib.c -o ../lib/msglib.o $(DBGINFO)
	
RSA.o: ../lib/RSA.c
	$(CC) $(CFLAGS) -c ../lib/RSA.c -o ../lib/RSA.o $(DBGINFO)
	
longdiv.o: ../lib/longdiv.c
	$(CC) $(CFLAGS) -c ../lib/longdiv.c -o ../lib/longdiv.o $(DBGINFO)
	
	


clear:
	rm *.o lock

