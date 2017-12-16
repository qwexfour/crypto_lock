#include"netlib.h"


#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>

#include "../lib/RSA.h"
#include "../lib/msglib.h"
#include "../lib/consts.h"
#include "locklib.h"


#define BUFF_SIZE 255
#define MAX_PENDING 17
#define TIME_LIMIT 10



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

enum res_type
{
	PERMITED,
	DENIED,
	ERROR
};


static enum res_type testTime( const char *recv_time_str )
{
	time_t raw_time;
	struct tm *local_time;
	struct tm recv_time;

	/* Obtaining local time */
	time( &raw_time );
	local_time = localtime( &raw_time );

	/* Parsing received time */
	sscanf( recv_time_str, "%d %d %d %d %d %d", 
							&recv_time.tm_mday,
							&recv_time.tm_mon,
							&recv_time.tm_year,
							&recv_time.tm_hour,
							&recv_time.tm_min,
							&recv_time.tm_sec
		  );


	/* Comparing */
	if( recv_time.tm_mday == local_time->tm_mday		&&
		recv_time.tm_mon  == local_time->tm_mon + 1     &&
		recv_time.tm_year == local_time->tm_year + 1900 &&
		recv_time.tm_hour == local_time->tm_hour		&&
		recv_time.tm_min  == local_time->tm_min			&&
		abs( recv_time.tm_sec - local_time->tm_sec ) < TIME_LIMIT
	  )
	{
		return PERMITED;
	}
	else
	{
		return DENIED;
	}
}


static enum res_type analyze( char *msg )
{
	parsed_msg_t parsed_msg;
	char key_e_str[ BUFF_SIZE ];
	char key_n_str[ BUFF_SIZE ];
	number msg_hash[ LENGTH_2BYTES ];
	number sign[ LENGTH_2BYTES ];
	number key_e[ LENGTH_2BYTES ];
	number key_n[ LENGTH_2BYTES ];

	if( !parseMsg( msg, &parsed_msg ) )
	{
		fprintf( stderr, "Cannot parse message\n" );
		return ERROR;
	}

	if( parsed_msg.type != OPEN )
	{
		fprintf( stderr, "Wrong format: must be OPEN" );
		return ERROR;
	}
	
	/* Check time */
	if( testTime( parsed_msg.time ) == DENIED )
	{
		fprintf( stderr, "Time mismatch\n" );
		return DENIED;
	}


	/* Request keys by name from server */
	if( !requestKey( PORT_SERVER, IP_SERVER,
					 parsed_msg.surname,
					 parsed_msg.name,
					 parsed_msg.patronymic,
					 key_e_str,
					 key_n_str
				   )
	  )
	{
		fprintf( stderr, "Cannot obtain keys" );
		return DENIED;
	}


	/* RSA */
	StrToNumber( key_e_str, key_e );
	StrToNumber( key_n_str, key_n );
	StrToNumber( parsed_msg.sign, sign );

	HashFunction( parsed_msg.text, msg_hash );

	if( VerificationSignatureRSA( msg_hash, sign, key_e, key_n ) == 1 )
	{
		return PERMITED;
	}
	else
	{
		return DENIED;
	}

}


int tcpServer( unsigned short port )
{
	struct sockaddr_in serv_addr = { 0 };    /* Address of the server */
	struct sockaddr_in cli_addr = { 0 };    /* Client address */
	int serv_socket = 0, cli_socket = 0;
	socklen_t cli_addr_size = sizeof( cli_addr );
	char recv_msg[BUFF_SIZE] = "";
	char *send_msg = NULL;
	int recv_msg_len = 0;
	int send_msg_len = sizeof( recv_msg );



	/* Filling serv_addr */
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_port = htons( port );
	serv_addr.sin_addr.s_addr = htonl( INADDR_ANY );


	/* Creating serv_socket */
	if( ( serv_socket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP ) ) < 0 )
	{
		fprintf( stderr, "Cannot create server socket\n");
		return -1;
	}
	fprintf( stderr, "Created server socket\n" );


	/* Binding serv_socket*/
	if( bind( serv_socket, (struct sockaddr *)&serv_addr, sizeof( serv_addr ) ) < 0 )
	{
		fprintf( stderr, "Cannot bind socket\n" );
		return -1;
	}
	fprintf( stderr, "Binded socket\n" );


	/* Setting port to listen */
	if( listen( serv_socket, MAX_PENDING ) < 0 )
	{
		fprintf( stderr, "Cannot set to listen\n" );
		return -1;
	}
	fprintf( stderr, "Listening...\n" );

	
	while( 1 )
	{
		/* Accepting a client */
		if( ( cli_socket = accept( serv_socket, (struct sockaddr *)&cli_addr, &cli_addr_size ) ) < 0 )
		{
			fprintf( stderr, "Cannot accept a client\n" );
			continue;
		}
		fprintf( stderr, "Accepted a client\n" );


		/* Receive a message */
		if( ( recv_msg_len = recv( cli_socket, recv_msg, BUFF_SIZE, 0 ) ) < 0 )
		{
			fprintf( stderr, "Cannot receive data from client\n" );
			continue;
		}


		/* Parse and authorize client */
		switch( analyze( recv_msg ) )
		{
			case ERROR:
				fprintf( stderr, "Received message analysis failed\n" );
				send_msg = makeMsg( "Error", &send_msg_len );
				break;
			case DENIED:
				fprintf( stderr, "Access denied\n" );
				send_msg =  makeMsg( "Denied", &send_msg_len );
				break;
			case PERMITED:
				fprintf( stderr, "Access permited\n" );
				send_msg = makeMsg( "Permited", &send_msg_len );
				openLock();
				break;
			default:
				fprintf( stderr, "Received message analysis failed\n" );
				send_msg = makeMsg( "Error", &send_msg_len );
				break;
		}


		/* Sending a message */
		if( send( cli_socket, send_msg, send_msg_len, 0 ) < 0 )
		{
			fprintf( stderr, "Cannot send message to client\n" );
			continue;
		}
	}

	return 0;
}


int requestKey( unsigned short port, char *addr_string, char *surname, char *name, char *patronymic, char *key_e, char *key_n )
{
	int sockid = 0;
	struct in_addr addr_struct_internal;
	struct sockaddr_in addr_struct;
	char *send_msg;
	char recv_msg[BUFF_SIZE] = "";
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
	if( ( recv_msg_len = recv( sockid, (void *)recv_msg, BUFF_SIZE, 0 ) ) < 0 )
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
