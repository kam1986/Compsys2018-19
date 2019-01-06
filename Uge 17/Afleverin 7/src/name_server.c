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
    char buf[MAXLINE], command[MAXLINE], args[MAXLINE], arg1[MAXLINE], arg2[MAXLINE], nick[MAXLINE];

    pid_t childpid;

    DIR *dir;
    struct dirent *peer_log;
    char *log_path = "Log"

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
                readline(connfd, buf);

                sscanf(buf, "%s %[^\n]",command, args);

                if(strcmp(buf,"EXIT:") == 0 ){
                    break; // terminate process
                }

                if(strcmp(buf, "LOOKUP:") == 0){
                    
                    if(strcmp(args,"") > 1){            // if args = "\n" strcmp(args,"") == 1 else some argument is given.
                        sscanf(args, "%s\n", buf);      // remove line ending.
                        sprintf(arg1, "%s.on", buf):    // appen file ending.
                        
                        dir = opendir(log_path);        // open directory of peer datafiles.
                        
                        while((peer_log = readdir(dir)) != NULL){
                            if(strcmp(peer_log -> d_name, arg1)==0){ // checking file name.
                                int file = Open(peer_log -> d_name, O_RDONLY, 0666);

                                readline(file, buf); // read ip

                                sscanf(buf, "%s\n", arg2); // remove line ending since Send append one.
                                send(connfd, arg2); // send ip back e.i. IP: ip\n

                                readline(file, buf); // read port
                                sscanf(buf, "%s\n", arg2); // remove line ending since Send append one.
                                send(connfd, arg2); // send ip back e.i. Port: port\n

                                Close(file);
                                break; // end correspondence.
                            }
                        }

                        if(peer_log == NULL){ // no peer found
                            send(connfd, "Peer not found"); // append linebreak.
                        }

                        close(dir);
                    }

                    continue;
                }

                if(strcmp(buf, "LOGIN:") == 0){

                    sscanf(args, "%s\n", arg1);

                    continue;
                }

                if(strcmp(buf, "LOGOUT:") == 0){
                    // DO STUFF
                    continue;
                }

                // make sure to not miss
                command[0] = "\0";
                args = "\0";

            }

            // terminate process.
            close(connfd);
            exit(0);
        }
        

        // TODO - should be changed to RIO function.
        Fgets(buf, MAXLINE, stdin);
        // closing server, waiting for all clients to disconnect.
        if(strcmp(buf,"Close") == 0){
            // waiting for all processes to die.
            while(waitpid(-1, &status, 0) > 0){
                int ret;
                if((ret = WIFEXITED(status)) < 0) {
                    printf("child %d terminated normally with exit status=%d\n", childpid, ret);
                }
                printf("child %d terminated abnormally\n", childpid);
            }
            break;
        }
    }

    //TODO: Implement
    exit(0);
}
