#include<stdlib.h>
#include<stdio.h>

#include"../lib/consts.h"
#include"../lib/cryptolib.h"
#include"locklib.h"
#include"netlib.h"
#include"btoothlib.h"


int main( void )
{
	key_type key;
	
	btoothServer( 1 );
	key = requestKey( PORT, IP_SERVER, "Pavlov", "Anton", "Egorovich" );
	printf( "%lli\n", key );
	openLock();
	return 0;
}
