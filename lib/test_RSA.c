#include <stdlib.h>
#include <stdio.h>
#include <time.h>


#include "RSA.h"
#include "msglib.h"

#define BUFF_SIZE 255

int main( void )
{
	number key_e[LENGTH_2BYTES];
	number key_n[LENGTH_2BYTES];
	number key_d[LENGTH_2BYTES];
	number msg[LENGTH_2BYTES];
	number msg1[LENGTH_2BYTES];
	number sign[LENGTH_2BYTES];
	char *text_str, *send_str;
	char sign_str[BUFF_SIZE];
	parsed_msg_t parsed_msg;
	int res;
	
	srand( time( NULL ) );
	//RandLong( msg, LENGTH_2BYTES );
	text_str = makeOpenText( "Иванов", "Иван", "Иванович", "19 12 17 16 33 23", NULL );
	HashFunction( text_str, msg );
	
	printf( "Text hash: " );
	PrintfLong( msg );

	GenKeys( key_e, key_n, key_d );
	SignatureRSA( msg, key_d, key_n, sign );
	NumberToStr( sign, sign_str );

	send_str = makeOpenMsg( "Иванов", "Иван", "Иванович", "19 12 17 16 33 23", sign_str, NULL );
	//printf( "Sign: " );
	//PrintfLong( sign );

	parseMsg( send_str, &parsed_msg );
	HashFunction( parsed_msg.text, msg1 );

	printf( "Parsed hash: " );
	PrintfLong( msg1 );

	res = VerificationSignatureRSA( msg1, sign, key_e, key_n );

	printf( "%d\n", res );
	return 0;
}
