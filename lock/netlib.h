#ifndef NET_LIB
#define NET_LIB



typedef struct in_addr
{
	unsigned long s_addr;							/* Internet address (32 bits) */
} in_addr;

typedef struct sockaddr_in
{
	unsigned short sin_family;						/* Internet protocol (AF_INET) */
	unsigned short sin_port;						/* Address port (16 bits) */
	struct in_addr sin_add;							/* Internet address (32 bits) */
	char sin_zero[8];								/* Not used */
} sockaddr_in;


/* Establish TCP connection
 * addr_string is in 255.255.255.255 format
 * returns socket descriptor */
int getClientSocket( char *addr_string );
