#include"netlib.h"


#include<sys/socket.h>
#include<arpa/inet.h>



static void *getMsg( char *surname, char *name, char *patronymic, int *length )
{
	char *msg;
	int len;
	
	len = 5 + strlen( surname ) + strlen( name ) + strlen( patronymic );
	msg = (char *)calloc( len, sizeof( *msg ) );
	sprintf( msg, "a#%s#%s#%s", surname, name, patronymic );

	*length = len;
	return (void *)msg;
}

/* Establish TCP connection
 * addr_string is in 255.255.255.255 format
 * returns socket descriptor */
key_t requestKey( unsigned short port, char *addr_string, char *surname, char *name, char *patronymic )
{
	int sockid = 0;
	in_addr addr_struct_internal;
	sockaddr_in addr_struct;
	void *msg;
	int msg_len;
	key_t key;

	
	/* Fill addr_struct */
	inet_aton( addr_string, &addr_struct_internal );
	addr_struct.sin_addr = addr_struct_internal;
	addr_struct.sin_port = htons( port );
	addr_struct.sin_family = AF_INET;

	/* Create socet */
	if( ( sockid = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP ) ) < 0 )
	{
		fprintf( stderr, "Cannot create socket\n" );
		return 0;
	}
	
	/* Connect to server */
	if( connect( sockid, ( struct sockaddr * )&addr_struct, sizeof( addr_struct ) ) < 0 )
	{
		fprintf( stderr, "Cannot connect to server\n" );
		return 0;
	}

	/* Sending FIO */
	msg = getMsg( surname, name, patronymic, &msg_len );
	if( ( send( sockid, msg, msg_len, 0 ) ) < 0 )
	{
		fprintf( stderr, "Cannot send data to server\n" );
		return 0;
	}

	
	close( sockid );
	return key;
}
