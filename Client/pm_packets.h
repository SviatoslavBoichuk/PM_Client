#ifndef PM_PACKETS_H
#define PM_PACKETS_H

#include <stdlib.h>

#define BYTE unsigned char
#define BUFFER_SIZE 128

/* requests from client */
#define NONE        0x00
#define SENDTO      0x01
#define SENDNTO     0x02
#define SENDTOFILE  0x03
#define CHANGESTATE 0x04
#define REGISTER    0x05
#define AUTH        0x06
#define ADDCONTACT  0x07
#define LISTCONTACT 0x08
#define UPDSTATUS   0x09

/* responses from server */
#define SERVER_OK               0x01
#define SERVER_REGISTER_FAIL    0x02
#define SERVER_AUTH_FAIL        0x03
#define SERVER_MSG              0x04
#define SERVER_UPDATE           0x05
#define SERVER_FAIL             0xff

/* user state */
enum State
{
    ONLINE,
    OFFLINE,
    BUSY
};

/* structure for registration purpose */
struct register_st
{
    char username[BUFFER_SIZE];
    char password[BUFFER_SIZE];
    char firstname[BUFFER_SIZE];
    char secondname[BUFFER_SIZE];
    char email[BUFFER_SIZE];
};

/* structure that contains user
 * info that can be seen by other
 * users */
struct user_st
{
    int id;
    char username[BUFFER_SIZE];
    char firstname[BUFFER_SIZE];
    char secondname[BUFFER_SIZE];
};

/* structure for authentication purpose
 * (username + password to log in) */
struct auth_st
{
    char username[BUFFER_SIZE];
    char password[BUFFER_SIZE];
};

/* this structure is used for
 * sending messages */
struct msg_st
{
    int who;                    /* who's sending message */
    int whom;                   /* who would recieve */
    char mime_type[BUFFER_SIZE];
    char file_name[BUFFER_SIZE];
    size_t msg_size;
};

/* structure for adding new contanct
 * to users contact list */
struct contact_add
{
    int who;                    /* who wants to add contact (id) */
    char username[BUFFER_SIZE]; /* whom is trying to add */
};

/* server response structure */
struct server_response_st
{
    BYTE response_code;             /* servers response byte */
    char msg_buffer[BUFFER_SIZE];   /* used for error messages when
                               * response code != SERVER_OK */
};

#endif // PM_PACKETS_H
