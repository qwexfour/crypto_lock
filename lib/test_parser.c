#include<stdio.h>
#include<stdlib.h>
#include"msglib.h"



int main( int argc, char **argv )
{
	parsed_msg_t p = { 0 };

	if( argc < 2 )
		return 0;

	if( !parseMsg( argv[1], &p ) )
	{
		printf( "Surname:    %s\n", p.surname );
		printf( "Name:       %s\n", p.name );
		printf( "Patronymic: %s\n", p.patronymic );
		printf( "Text:       %s\n", p.text );
		printf( "Key:        %s\n", p.key_e );
	}
	else
	{
		printf( "Wrong format\n" );
	}

	return 0;
}
