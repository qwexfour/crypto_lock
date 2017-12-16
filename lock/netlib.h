#ifndef NET_LIB
#define NET_LIB



#if 0
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
#endif

int requestKey( unsigned short port, char *addr_string, char *surname, char *name, char *patronymic, char *key_e, char *key_n );

int tcpServer( unsigned short port );


#endif /* NET_LIB */
