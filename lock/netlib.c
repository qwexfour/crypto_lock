#include"netlib.h"


#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<string.h>
#include<stdlib.h>
#include<stdio.h>

#include"../lib/msglib.h"


#define RECV_MSG_SIZE 255



static int keyFromServMsg( char *msg, char *key_e, char *key_n )
{
	parsed_msg_t parsed_msg;
	
	if( !parseMsg( msg, &parsed_msg ) )
	{
		fprintf( stderr, "Cannot parse\n" );
		return -1;
	}
	
	if( parsed_msg.type != KEY )
	{
		fprintf( stderr, "Wrong format: not a key\n" );
		return -1;
	}

	strcpy( parsed_msg.key_e, key_e );
	strcpy( parsed_msg.key_n, key_n );

	return 0;
}


int requestKey( unsigned short port, char *addr_string, char *surname, char *name, char *patronymic, char *key_e, char *key_n )
{
	int sockid = 0;
	struct in_addr addr_struct_internal;
	struct sockaddr_in addr_struct;
	char *send_msg;
	char recv_msg[RECV_MSG_SIZE] = "";
	int send_msg_len = 0, recv_msg_len = 0;

	
	/* Fill addr_struct */
	inet_aton( addr_string, &addr_struct_internal );
	addr_struct.sin_addr = addr_struct_internal;
	addr_struct.sin_port = htons( port );
	addr_struct.sin_family = AF_INET;


	/* Creating socet */
	if( ( sockid = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP ) ) < 0 )
	{
		fprintf( stderr, "Cannot create socket\n" );
		return -1;
	}


	/* Connecting to server */
	if( connect( sockid, ( struct sockaddr * )&addr_struct, sizeof( addr_struct ) ) < 0 )
	{
		fprintf( stderr, "Cannot connect to server\n" );
		return -1;
	}
	fprintf( stderr, "Connected to server\n" );


	/* Sending FIO */
	send_msg = makeAuthMsg( surname, name, patronymic, &send_msg_len );
	if( ( send( sockid, (void *)send_msg, send_msg_len, 0 ) ) != send_msg_len )
	{
		fprintf( stderr, "Cannot send data to server\n" );
		return -1;
	}	
	free( send_msg );
	fprintf( stderr, "Sended massege\n" );


	/* Receiving key */
	if( ( recv_msg_len = recv( sockid, (void *)recv_msg, RECV_MSG_SIZE, 0 ) ) < 0 )
	{
		fprintf( stderr, "Cannot receive data from server\n" );
		return -1;
	}
	fprintf( stderr, "Received:%s\n", recv_msg );

	/* Parsing keys */
	if( !keyFromServMsg( recv_msg, key_e, key_n ) )
	{
		fprintf( stderr, "Cannot parse key\n" );
		return -1;
	}


	/* Closing socket */
	close( sockid );

	return 0;

}
