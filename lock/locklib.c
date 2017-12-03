#include"locklib.h"

#include<stdio.h>

int openLock( void )
{
	printf( "Sending signal to open lock\n" );
	return 1;
}
