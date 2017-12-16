#include"msglib.h"


#include<stdio.h>
#include<stdlib.h>
#include<string.h>



/* To request key_e (lock -> server) */
char *makeAuthMsg( const char *surname, const char *name, const char *patronymic, int *length )
{
	char *send_msg;
	int len;

	len = 6 + strlen( surname ) + strlen( name ) + strlen( patronymic );
	send_msg = (char *)calloc( len, sizeof( *send_msg ) );
	sprintf( send_msg, "a#%s#%s#%s\n", surname, name, patronymic );

	if( length != NULL )
	{
		*length = len;
	}
	return send_msg;
}


/* To register a client (client -> server) */
char *makeRegMsg( const char *surname, const char *name, const char *patronymic, const char *key_e, const char *key_n, int *length )
{
	char *send_msg;
	int len;

	len = 8 + strlen( surname ) + strlen( name ) + strlen( patronymic ) + strlen( key_e ) + strlen( key_n );
	send_msg = (char *)calloc( len, sizeof( *send_msg ) );
	sprintf( send_msg, "r#%s#%s#%s#%s#%s\n", surname, name, patronymic, key_e, key_n );


	if( length != NULL )
	{
		*length = len;
	}
	return send_msg;
}

/* WARNING: Do not send
 * Returns string for signing */
char *makeOpenText( const char *surname, const char *name, const char *patronymic, const char *time, int *length )
{
	char *send_msg;
	int len;

	len = 6 + strlen( surname ) + strlen( name ) + strlen( patronymic ) + strlen( time );
	send_msg = (char *)calloc( len, sizeof( *send_msg ) );
	sprintf( send_msg, "o#%s#%s#%s#%s", surname, name, patronymic, time );


	if( length != NULL )
	{
		*length = len;
	}
	return send_msg;
}


char *makeOpenMsg( const char *surname, const char *name, const char *patronymic, const char *time, const char *sign, int *length )
{
	char *send_msg;
	int len;

	len = 8 + strlen( surname ) + strlen( name ) + strlen( patronymic ) + strlen( time ) + strlen( sign );
	send_msg = (char *)calloc( len, sizeof( *send_msg ) );
	sprintf( send_msg, "o#%s#%s#%s#%s#%s\n", surname, name, patronymic, time, sign );


	if( length != NULL )
	{
		*length = len;
	}
	return send_msg;
}


char *makeMsg( const char *text, int *length )
{
	char *send_msg;
	int len;

	len = 4 + strlen( text );
	send_msg = (char *)calloc( len, sizeof( *send_msg ) );
	sprintf( send_msg, "m#%s\n", text );


	if( length != NULL )
	{
		*length = len;
	}
	return send_msg;
}



int parseMsg( const char *msg, parsed_msg_t *parsed_msg )
{
	char ch;
	int i = 0;			/* Internal counter */
	int j = 0;			/* External counter */

	switch( msg[0] )
	{
		case 'r':
			parsed_msg->type = REG;

			if( msg[1] != '#' )
				return -1;
			
			j = 2;   // Excluded first two symbols

			/* Reading surname */
			for( i = 0; ; i++, j++ )
			{
				ch = msg[j];

				if( ch == '\n' || ch == '\0' )
				{
					return -1;
				}
				if( ch == '#' && i == 0 )
				{
					return -1;
				}
				if( ch == '#' )
				{
					j++;
					break;
				}
				parsed_msg->surname[i] = ch;
			}
			parsed_msg->surname[i] = '\0';

			/* Reading name */
			for( i = 0; ; i++, j++ )
			{
				ch = msg[j];

				if( ch == '\n' || ch == '\0' )
				{
					return -1;
				}
				if( ch == '#' && i == 0 )
				{
					return -1;
				}
				if( ch == '#' )
				{
					j++;
					break;
				}
				parsed_msg->name[i] = ch;
			}
			parsed_msg->name[i] = '\0';

			/* Reading patronymic */
			for( i = 0; ; i++, j++ )
			{
				ch = msg[j];

				if( ch == '\n' || ch == '\0' )
				{
					return -1;
				}
				if( ch == '#' && i == 0 )
				{
					return -1;
				}
				if( ch == '#' )
				{
					j++;
					break;
				}
				parsed_msg->patronymic[i] = ch;
			}
			parsed_msg->patronymic[i] = '\0';


			/* Reading key_e */
			for( i = 0; ; i++, j++ )
			{
				ch = msg[j];

				if( ch == '\n' || ch == '\0' )
				{
					return -1;
				}
				if( ch == '#' && i == 0 )
				{
					return -1;
				}
				if( ch == '#' )
				{
					j++;
					break;
				}
				parsed_msg->key_e[i] = ch;
			}
			parsed_msg->key_e[i] = '\0';


			/* Reading key_n */
			for( i = 0; ; i++, j++ )
			{
				ch = msg[j];

				if( ch == '#' )
				{
					return -1;
				}
				if( ch == '\n' || ch == '\0' )
				{
					if( i == 0 )
					{
						return -1;
					}
					break;
				}
				parsed_msg->key_n[i] = ch;
			}
			parsed_msg->key_n[i] = '\0';

			return 0;
		case 'a':
			parsed_msg->type = AUTH;

			if( msg[1] != '#' )
				return -1;
			
			j = 2;   // Excluded first two symbols

			/* Reading surname */
			for( i = 0; ; i++, j++ )
			{
				ch = msg[j];

				if( ch == '\n' || ch == '\0' )
				{
					return -1;
				}
				if( ch == '#' && i == 0 )
				{
					return -1;
				}
				if( ch == '#' )
				{
					j++;
					break;
				}
				parsed_msg->surname[i] = ch;
			}
			parsed_msg->surname[i] = '\0';

			/* Reading name */
			for( i = 0; ; i++, j++ )
			{
				ch = msg[j];

				if( ch == '\n' || ch == '\0' )
				{
					return -1;
				}
				if( ch == '#' && i == 0 )
				{
					return -1;
				}
				if( ch == '#' )
				{
					j++;
					break;
				}
				parsed_msg->name[i] = ch;
			}
			parsed_msg->name[i] = '\0';

			/* Reading patronymic */
			for( i = 0; ; i++, j++ )
			{
				ch = msg[j];

				if( ch == '#' )
				{
					return -1;
				}
				if( ch == '\n' || ch == '\0' )
				{
					if( i == 0 )
					{
						return -1;
					}
					break;
				}
				parsed_msg->patronymic[i] = ch;
			}
			parsed_msg->patronymic[i] = '\0';

			return 0;
		case 'o':
			parsed_msg->type = OPEN;

			if( msg[1] != '#' )
				return -1;
			
			j = 2;   // Excluded first two symbols

			parsed_msg->text[0] = 'o';
			parsed_msg->text[1] = '#';

			/* Reading surname */
			for( i = 0; ; i++, j++ )
			{
				ch = msg[j];

				if( ch == '\n' || ch == '\0' )
				{
					return -1;
				}
				if( ch == '#' && i == 0 )
				{
					return -1;
				}
				if( ch == '#' )
				{
					parsed_msg->text[j] = ch;
					j++;
					break;
				}
				parsed_msg->surname[i] = ch;
				parsed_msg->text[j] = ch;
			}
			parsed_msg->surname[i] = '\0';

			/* Reading name */
			for( i = 0; ; i++, j++ )
			{
				ch = msg[j];

				if( ch == '\n' || ch == '\0' )
				{
					return -1;
				}
				if( ch == '#' && i == 0 )
				{
					return -1;
				}
				if( ch == '#' )
				{
					parsed_msg->text[j] = ch;
					j++;
					break;
				}
				parsed_msg->name[i] = ch;
				parsed_msg->text[j] = ch;
			}
			parsed_msg->name[i] = '\0';

			/* Reading patronymic */
			for( i = 0; ; i++, j++ )
			{
				ch = msg[j];

				if( ch == '\n' || ch == '\0' )
				{
					return -1;
				}
				if( ch == '#' && i == 0 )
				{
					return -1;
				}
				if( ch == '#' )
				{
					parsed_msg->text[j] = ch;
					j++;
					break;
				}
				parsed_msg->patronymic[i] = ch;
				parsed_msg->text[j] = ch;
			}
			parsed_msg->patronymic[i] = '\0';


			/* Reading time */
			for( i = 0; ; i++, j++ )
			{
				ch = msg[j];

				if( ch == '\n' || ch == '\0' )
				{
					return -1;
				}
				if( ch == '#' && i == 0 )
				{
					return -1;
				}
				if( ch == '#' )
				{
					parsed_msg->text[j] = '\0';
					j++;
					break;
				}
				parsed_msg->time[i] = ch;
				parsed_msg->text[j] = ch;
			}
			parsed_msg->time[i] = '\0';


			/* Reading sign */
			for( i = 0; ; i++, j++ )
			{
				ch = msg[j];

				if( ch == '#' )
				{
					return -1;
				}
				if( ch == '\n' || ch == '\0' )
				{
					if( i == 0 )
					{
						return -1;
					}
					break;
				}
				parsed_msg->sign[i] = ch;
			}
			parsed_msg->sign[i] = '\0';

			return 0;
		case 'k':
			parsed_msg->type = KEY;

			if( msg[1] != '#' )
				return -1;
			
			j = 2;   // Excluded first two symbols


			/* Reading key_e */
			for( i = 0; ; i++, j++ )
			{
				ch = msg[j];

				if( ch == '\n' || ch == '\0' )
				{
					return -1;
				}
				if( ch == '#' && i == 0 )
				{
					return -1;
				}
				if( ch == '#' )
				{
					j++;
					break;
				}
				parsed_msg->key_e[i] = ch;
			}
			parsed_msg->key_e[i] = '\0';


			/* Reading key_n */
			for( i = 0; ; i++, j++ )
			{
				ch = msg[j];

				if( ch == '#' )
				{
					return -1;
				}
				if( ch == '\n' || ch == '\0' )
				{
					if( i == 0 )
					{
						return -1;
					}
					break;
				}
				parsed_msg->key_n[i] = ch;
			}
			parsed_msg->key_n[i] = '\0';

			return 0;
		case 'm':
			parsed_msg->type = MSG;

			if( msg[1] != '#' )
				return -1;
			
			j = 2;   // Excluded first two symbols


			/* Reading text */
			for( i = 0; ; i++, j++ )
			{
				ch = msg[j];

				if( ch == '#' )
				{
					return -1;
				}
				if( ch == '\n' || ch == '\0' )
				{
					if( i == 0 )
					{
						return -1;
					}
					break;
				}
				parsed_msg->text[i] = ch;
			}
			parsed_msg->text[i] = '\0';

			return 0;
		default:
			return -1;
	}

	/* We mustn't reach this command */
	fprintf( stderr, "msglib: Fatal error\n" );
	exit( -1 );
}

