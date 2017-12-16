#include<stdlib.h>
#include<stdio.h>

#include"../lib/consts.h"
#include"locklib.h"
#include"netlib.h"
#include"btoothlib.h"


int main( void )
{
	//char key_e[255], key_n[255];
	
	tcpServer( PORT_LOCK );
	//btoothServer( 1 );
	//requestKey( PORT, IP_SERVER, "Pavlov", "Anton", "Egorovich", key_e, key_n );
	//printf( "%s %s\n", key_e, key_n );
	//openLock();
	return 0;
}
