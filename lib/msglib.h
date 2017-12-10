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
	char key[BUFF_SIZE];
} parsed_msg_t;




/* All make functions return size of generated string 
 * through length pointer if it is not NULL.
 * All returned strings must be freed. */

char *makeAuthMsg ( char *surname, char *name, char *patronymic, int *length );

char *makeRegMsg  ( char *surname, char *name, char *patronymic, char *key, int *length );

char *makeOpenText( char *surname, char *name, char *patronymic, char *time, int *length );

char *makeOpenMsg ( char *surname, char *name, char *patronymic, char *time, char *sign, int *length );


/* Returns 0 if the message parsed successfully */
int parseMsg( char *msg, parsed_msg_t *parsed_msg );


#endif /* MSG_LIB */
