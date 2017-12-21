#include <stdlib.h>
#include <stdio.h>
#include <time.h>


#include "RSA.h"



int main( void )
{
	number key_e[LENGTH_2BYTES];
	number key_n[LENGTH_2BYTES];
	number key_d[LENGTH_2BYTES];
	number msg[LENGTH_2BYTES];
	number sign[LENGTH_2BYTES];
	int res;
	
	srand( time( NULL ) );
	RandLong( msg, LENGTH_2BYTES );
	printf( "MSG: " );
	PrintfLong( msg );
	GenKeys( key_e, key_n, key_d );
	SignatureRSA( msg, key_d, key_n, sign );
	printf( "Sign: " );
	PrintfLong( sign );
	printf( "MSG: " );
	PrintfLong( msg );
	res = VerificationSignatureRSA( msg, sign, key_e, key_n );

	printf( "%d\n", res );
	return 0;
}
