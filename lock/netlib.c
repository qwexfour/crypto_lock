#include"netlib.h"


#include<sys/socket.h>
#include<arpa/inet.h>


#define RECV_MSG_SIZE 255


static char *createSendMsg( char *surname, char *name, char *patronymic, int *length )
{
	char *send_msg;
	int len;
	
	len = 5 + strlen( surname ) + strlen( name ) + strlen( patronymic );
	send_msg = (char *)calloc( len, sizeof( *send_msg ) );
	sprintf( send_msg, "a#%s#%s#%s", surname, name, patronymic );

	*length = len;
	return send_msg;
}


key_t keyFromRecvMsg( char *msg, int msg_len )
{
	key_t key;

	if( msg[1] != '#' )
	{
		fprintf( stderr, "Wrong message format" );
		return 0;
	}

	if( msg[0] != 'k' )
	{
		fprintf( stderr, "Wrong message type" );
		return 0;
	}

	/* Key starting from third character ( k#key ) */
	key = atoll( msg[2] )

	return key;

}

key_t requestKey( unsigned short port, char *addr_string, char *surname, char *name, char *patronymic )
{
	int sockid = 0;
	in_addr addr_struct_internal;
	sockaddr_in addr_struct;
	char *send_msg;
	char recv_msg[RECV_MSG_SIZE] = "";
	int send_msg_len = 0, recv_msg_len = 0;
	key_t key;

	
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


	/* Sending FIO */
	send_msg = createSendMsg( surname, name, patronymic, &send_msg_len );
	if( ( send( sockid, (void *)send_msg, send_msg_len, 0 ) ) != send_msg_len )
	{
		fprintf( stderr, "Cannot send data to server\n" );
		return 0;
	}
	free( send_msg );


	/* Receiving key */
	if( ( recv_msg_len = recv( sockid, (void *)recv_msg, RECV_MSG_SIZE, 0 ) ) < 0 )
	{
		fprintf( stderr, "Cannot receive data from server\n" );
		return 0;
	}
	key = keyFromRecvMsg( recv_msg, recv_msg_len );


	/* Closing socket */
	close( sockid );


	return key;

}
