#include"btoothlib.h"


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <bluetooth/bluetooth.h>
#include <bluetooth/hci.h>
#include <bluetooth/hci_lib.h>

#include "../lib/consts.h"


#define MAX_PENDING 1
#define RECV_MSG_SIZE 255
#define SEND_MSG_SIZE 255


struct sockaddr_rc {
	sa_family_t	rc_family;
	bdaddr_t	rc_bdaddr;
	uint8_t		rc_channel;
};


void btoothServer( int port )
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
	serv_addr.rc_bdaddr = AD_BTOOTH;		/* TODO: Maybe its better to hardcode in */
	serv_addr.rc_channel = (uint8_t) port;


	/* Creating serv_socket */
	serv_socket = socket( AF_BLUETOOTH, SOCK_STREAM, BTPROTO_RFCOMM );
	fprintf( stderr, "Created serv_socket\n" );


	/* Binding serv_socket to a port of BT adapter */
	bind( serv_socket, (struct sockaddr *)&serv_addr, sizeof( serv_addr ) );
	fprintf( stderr, "Binded serv_socket\n" );


	/* Setting port to listen */
	listen( serv_socket, MAX_PENDING );
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


		//data = parseCliMsg( recv_msg, recv_msg_len );


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
