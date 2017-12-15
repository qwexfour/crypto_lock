#include"btoothlib.h"


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#include "../lib/consts.h"
#include "../lib/msglib.h"
//#include "../lib/RSA.h"
#include "netlib.h"


#define BUFF_SIZE 255
#define MAX_PENDING 1
#define RECV_MSG_SIZE 255
#define SEND_MSG_SIZE 255


struct sockaddr_rc {
	sa_family_t	rc_family;
	bdaddr_t	rc_bdaddr;
	uint8_t		rc_channel;
};


enum res_type
{
	PERMITED,
	DENIED,
	ERROR
};

#if 0
enum res_type analyse( char *msg )
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
	
	/* Request keys by name from server */
	if( !requestKey( PORT, IP_SERVER,
					 parsed_msg.surname,
					 parsed_msg.name,
					 parsed_msg.patronymic,
					 key_e_str,
					 key_e_str )
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

#endif

int btoothServer( int port )
{
	struct sockaddr_rc serv_addr = { 0 };    /* Address of the server */
	struct sockaddr_rc cli_addr = { 0 };    /* Client address */
	int serv_socket = 0, cli_serv_socket = 0;
	socklen_t cli_addr_size = sizeof( cli_addr );
	char recv_msg[RECV_MSG_SIZE] = "";
	char send_msg[SEND_MSG_SIZE] = "";
	int recv_msg_len = sizeof( recv_msg );
	int send_msg_len = sizeof( recv_msg );



	/* Filling serv_addr */
	serv_addr.rc_family = AF_BLUETOOTH;
	serv_addr.rc_bdaddr = *BDADDR_ANY;		/* TODO: Maybe its better to hardcode in */
	serv_addr.rc_channel = (uint8_t) port;


	/* Creating serv_socket */
	if( ( serv_socket = socket( AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM ) ) < 0 )
	{
		fprintf( stderr, "Cannot create bluetooth socket\n");
		return -1;
	}
	fprintf( stderr, "Created bluetooth socket\n" );


	/* Binding serv_socket to a port of BT adapter */
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
		cli_serv_socket = accept( serv_socket, (struct sockaddr *)&cli_addr, &cli_addr_size );
		fprintf( stderr, "Accepted a client\n" );


		/* Receive a message */
		if( ( recv_msg_len = read( cli_serv_socket, recv_msg, RECV_MSG_SIZE ) ) < 0 )
		{
			fprintf( stderr, "Cannot receive data from client\n" );
			continue;
		}


		//data = parseCliMsg( recv_msg );


		/* Sending a message */
		sprintf( send_msg, "m:Permit" );
		if( write( cli_serv_socket, send_msg, sizeof( send_msg ) ) < 0 )
		{
			fprintf( stderr, "Cannot send message to client\n" );
			continue;
		}

	}


	return;
}
