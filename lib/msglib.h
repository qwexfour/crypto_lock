#ifndef MSG_LIB
#define MSG_LIB


#define BUFF_SIZE 255


enum msg_type
{
	REG,
	AUTH,
	KEY,
	OPEN,
	MSG
};


typedef struct parsed_msg
{
	enum msg_type type;
	char surname[BUFF_SIZE];
	char name[BUFF_SIZE];
	char patronymic[BUFF_SIZE];
	char text[BUFF_SIZE];
	char key_e[BUFF_SIZE];
	char key_n[BUFF_SIZE];
	char sign[BUFF_SIZE];
	char time[BUFF_SIZE];
} parsed_msg_t;




/* All make functions return size of generated string 
 * through length pointer if it is not NULL.
 *
 * All returned strings must be freed. */

char *makeAuthMsg ( const char *surname, const char *name, const char *patronymic, int *length );

char *makeRegMsg  ( const char *surname, const char *name, const char *patronymic, const char *key_e,const char *key_n, int *length );

char *makeOpenText( const char *surname, const char *name, const char *patronymic, const char *time, int *length );

char *makeOpenMsg ( const char *surname, const char *name, const char *patronymic, const char *time, const char *sign, int *length );

char *makeMsg     ( const char *text, int *length );



/* Returns 0 if the message parsed successfully */
int parseMsg( const char *msg, parsed_msg_t *parsed_msg );


#endif /* MSG_LIB */
