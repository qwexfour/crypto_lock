#include"netlib.h"


#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

#include"../lib/cryptolib.h"



#define RECV_MSG_SIZE 255


static char *createSendMsg( char *surname, char *name, char *patronymic, int *length )
{
	char *send_msg;
	int len;
	
	len = 5 + strlen( surname ) + strlen( name ) + strlen( patronymic ) + 5;
	send_msg = (char *)calloc( len, sizeof( *send_msg ) );
	sprintf( send_msg, "a#%s#%s#%s\n", surname, name, patronymic );

	*length = len;
	return send_msg;
}


static key_type keyFromRecvMsg( char *msg, int msg_len )
{
	key_type key;

	if( msg[1] != '#' )
	{
		fprintf( stderr, "Wrong message format\n" );
		return 0;
	}

	if( msg[0] != 'k' )
	{
		fprintf( stderr, "Wrong message type\n" );
		return 0;
	}

	/* Key starting from third character ( k#key ) */
	key = atoll( &msg[2] );

	return key;

}

key_type requestKey( unsigned short port, char *addr_string, char *surname, char *name, char *patronymic )
{
	int sockid = 0;
	struct in_addr addr_struct_internal;
	struct sockaddr_in addr_struct;
	char *send_msg;
	char recv_msg[RECV_MSG_SIZE] = "";
	int send_msg_len = 0, recv_msg_len = 0;
	key_type key = 0;

	
	/* Fill addr_struct */
	inet_aton( addr_string, &addr_struct_internal );
	addr_struct.sin_addr = addr_struct_internal;
	addr_struct.sin_port = htons( port );
	addr_struct.sin_family = AF_INET;


	/* Creating socet */
	if( ( sockid = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP ) ) < 0 )
	{
		fprintf( stderr, "Cannot create socket\n" );
		return 0;
	}


	/* Connecting to server */
	if( connect( sockid, ( struct sockaddr * )&addr_struct, sizeof( addr_struct ) ) < 0 )
	{
		fprintf( stderr, "Cannot connect to server\n" );
		return 0;
	}
	fprintf( stderr, "Connected to server\n" );


	/* Sending FIO */
	send_msg = createSendMsg( surname, name, patronymic, &send_msg_len );
	if( ( send( sockid, (void *)send_msg, send_msg_len, 0 ) ) != send_msg_len )
	{
		fprintf( stderr, "Cannot send data to server\n" );
		return 0;
	}	
	free( send_msg );
	fprintf( stderr, "Sended massege\n" );


	/* Receiving key */
	if( ( recv_msg_len = recv( sockid, (void *)recv_msg, RECV_MSG_SIZE, 0 ) ) < 0 )
	{
		fprintf( stderr, "Cannot receive data from server\n" );
		return 0;
	}
	printf( "Received:%s:end\n", recv_msg );
	key = keyFromRecvMsg( recv_msg, recv_msg_len );


	/* Closing socket */
	close( sockid );


	return key;

}
