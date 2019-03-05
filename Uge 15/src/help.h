#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "csapp.h"


// Server responces 
#define S_LOGIN_RESPONCE  "You are now logged in.\n"
#define S_LOGIN_REQUEST   "Send host and port\n"
#define S_LOGOUT_RESPONCE "You are now logged out.\n"
#define S_LOOKUP_RESPONCE "%s user online. The List follows.\n%s"

// Server errors
#define S_LOGIN_ERROR "Wrong nick or password.\n"
#define S_LOGOUT_ERROR "Logout error try again\n"
#define S_LOOKUP_ERROR "%s is not a valid user.\n"

// Peer requests
#define P_LOGIN_REQUEST1 "LOGIN: NICK %s PASS: %s"
#define P_LOGIN_REQUEST2 "IP: %s\nPort: %s"
#define P_LOGOUT_REQUEST "LOGOUT: %s"
#define P_LOOKUP_REQUEST "LOOKUP: %s"

// Peer errors
#define P_LOGIN_ERROR "Usage: /login <nick> <password> <host> <port>\n"


// send content to file descriptor with line ending appended
int Send(int clientfd, char* content);

// Read next line of source into output
int Readline(int source, char* output);

// Private function to simplify other helper functions.
// takes an input file discriptor as both in- and output.
int Source_dump(int soucre, int sink);
// takes a file descriptor or socket and a list of argument
int login(int serverfd, char* args);

// logout function
int logout(int serverfd, char* nick);

// lookup wrapper uses a sink as output since we will use it in A7 too.
int lookup(int serverfd, int sink, char* args);

int Exit(int serverfd, char* args);

int Login(int connfd, char* args);

int Logout(int connfd, char* args);

int Lookup(int connfd, char* args);


