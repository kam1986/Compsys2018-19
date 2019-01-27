#include <stdio.h>
#include <string.h>
#include "csapp.h"
#include "name_server.h"
#include "peer_help.h"

#define ARGNUM 1 // TODO: Put the number of arguments you want the
                 // program to take

int main(int argc, char**argv) {
    if (argc != ARGNUM + 1) {
        printf("%s expects %d arguments.\n", (argv[0]+2), ARGNUM);
        return(0);
    }

    int status, connfd, listenfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr; // Enough space for any address
    char client_hostname[MAXLINE], client_port[MAXLINE];

    // buffers, such as command arguments, argument1 and argument2
    char buf[MAXLINE], 
         command[MAXLINE/4], 
         args[MAXLINE/4], 
         arg1[MAXLINE/2], 
         arg2[MAXLINE/4], 
         arg3[MAXLINE/4];

    pid_t childpid;

    DIR *dir;
    struct dirent *peer_log;
    char *log_path = "Log";

    // initiate socket
    listenfd = Open_listenfd(argv[1]);

    while(1){
        clientlen = sizeof(struct sockaddr_storage);
        connfd = Accept(listenfd, (SA *)&clientaddr, &clientlen);

        Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE,
                    client_port, MAXLINE, 0);
        
        fprintf(stdout,"Connection accepted from (%s, %s)\n", client_hostname, client_port);

        // making new process on server for each client. 
        if((childpid = Fork()) == 0){

            while(1){
                // TODO - get stuff from client.
                command[0] = '\0';
                args[0] = '\0';
                buf[0] = '\0';

                readline(connfd, buf);

                sscanf(buf, "%s %[^\n]",command, args);

                if(strcmp(command,"EXIT:") == 0 ){
                    break; // terminate process
                }

                if(strcmp(command, "LOOKUP:") == 0){
                    
                    if(args != NULL){ 
                        sprintf(arg1, "%s.on", args);   // appen file ending.
                        
                        dir = opendir(log_path);        // open directory of peer datafiles.
                        
                        while((peer_log = readdir(dir)) != NULL){
                            if(strcmp(peer_log -> d_name, arg1)==0){ // checking file name.
                        
                                int file = Open(peer_log -> d_name, O_RDONLY, 0666);

                                readline(file, buf); // read ip

                                sscanf(buf, "%s\n", arg2); // remove line ending since Send append one.
                                Send(connfd, arg2); // send ip back e.i. IP: ip\n

                                readline(file, buf); // read port
                                sscanf(buf, "%s\n", arg2); // remove line ending since Send append one.
                                Send(connfd, arg2); // send ip back e.i. Port: port\n

                                Close(file);
                                break; // end correspondence.
                            }
                        }

                        if(peer_log == NULL){ // no peer found
                            Send(connfd, "Peer not found"); // append linebreak.
                        }

                        closedir(dir);
                    } else {
                        dir = opendir(log_path);

                        while((peer_log = readdir(dir)) != NULL){
                            if(strcmp(peer_log -> d_name, ".") == 0 || strcmp(peer_log -> d_name, ",,") == 0){
                                continue;
                            }
                            sscanf(peer_log -> d_name, "%s.on", arg1);
                            sprintf(arg2, "%s.on", arg1);
                            // check if it is a .on file
                            if(strcmp(peer_log -> d_name, arg2) == 0 ){
                                int file = Open(peer_log -> d_name, O_RDONLY, 0666);
                                sprintf(buf, "%s is online", arg1);
                                while(strcmp(buf,"\n") != 0){ // stop at double linefeed
                                    Send(connfd, buf);
                                    readline(file, buf);
                                }
                                Close(file);
                            }
                        }

                        closedir(dir);
                    }

                    continue;
                }

                if(strcmp(buf, "LOGIN:") == 0){
                    int file;
                   
                    sprintf(buf, "%s.off", args);

                    if((file = Open(buf, O_RDONLY, 0666)) > 0){

                            readline(connfd, buf); // get password.

                            int file = Open(peer_log -> d_name, O_RDONLY, 0666);
                            sscanf(buf, "PASS: %s", arg3);

                            readline(file, buf); // read password
                            
                            if(strcmp(arg3, buf) == 0){ // check password
                                Send(connfd, "PASS: OK");
                            } else {
                                Send(connfd, "LOGIN ERROR");
                                break;
                            }

                            Close(file);

                            readline(connfd, buf);
                            // get ip and port
                            sscanf(buf, "IP: %s Port: %s", arg1, arg2);
                            
                            sscanf((peer_log -> d_name), "%s.on", arg3);
                            file = Open(arg3, O_CREAT | O_WRONLY, 0666);
                            
                            sprintf(buf, "IP: %s\nPort: %s\n", arg1, arg2);
                            Send(file, buf);
                            Close(file);

                        }
                    } // if null the peer does not exists

                    continue;
                }

                if(strcmp(buf, "LOGOUT:") == 0){
                    // DO STUFF
                    continue;
                }

                // make sure to not miss
                command[0] = '\0';
                args[0] = '\0';

            }

            // terminate process.
            close(connfd);
            exit(0);
        }
        

        // TODO - should be changed to RIO function.
        Fgets(buf, MAXLINE, stdin);
    }

    //TODO: Implement
    exit(0);
}
