#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "csapp.h"


// A6

// socket could be any file descriptor
int Send(int socket, char *buf);


// socket could be any file descriptor.
int readline(int socket, char *buf);

int login(int socket, char *args, void *user);

int logout(int socket, void *user, pid_t msg_handler);

int lookup(int socket, char *args);

int EXIT(int socket, char *user, pid_t msg_handler);

// A7 part.

int send_message(int server, char *nick, char* msg);


// get message, formate it and save it in file
// peername.msg at the end of file.
int store_message(int socket, char* folder, char *nick);


// TODO need to handle read for all messages.
// assume that the message are saved correctly formatted.
// and only one peer for each folder. 
int get_messages(char *folder, char *nick, FILE *output);


int reap_messages(char* folder);