/*
 * echoclient.c - An echo client
 */
/* $begin echoclientmain */
#include "csapp.h"
#include <stdbool.h>
#include <string.h>

int main(int argc, char **argv) 
{
    int clientfd;
    char *host, *port, buf[MAXLINE];
    rio_t rio;

    if (argc != 3) {
	    fprintf(stderr, "usage: %s <host> <port>\n", argv[0]);
	    exit(0);
    }
    host = argv[1];
    port = argv[2];
    
    //establish connection
    clientfd = Open_clientfd(host, port);
    Rio_readinitb(&rio, clientfd);
    
    //client welcome message and connection established message
    printf("--> Hailstone Sequence Client v1.0\n"); 
    printf("--> Connection Established To %s Port %s\n", host, port);   

    //read welcome message from server
    Rio_readlineb(&rio, buf, MAXLINE);

    Fputs(buf, stdout);
    
    //prompt user to enter a positive integer
    printf("\n--> Enter A Positive Integer (^d to quit): ");
    int i = 0;
    int i2 = 0;
    while (Fgets(buf, MAXLINE, stdin) != NULL) {
        int ii;
        if(sscanf(buf, "%d", &ii)){ //ensure an integer is entered
            if(ii > 0){ //ensure a positive integer is entered

                //establish a new connection for each 
                //run of the Hailstone sequence
                if(i == 1){
                    clientfd = Open_clientfd(host, port);
                    Rio_readinitb(&rio, clientfd);
                }

                //write integer to server
	            Rio_writen(clientfd, buf, strlen(buf));

                //read Hailstone starting message from server
                Rio_readlineb(&rio,buf,MAXLINE);

                //continuously read lines from server
                //until "Done" message is sent
                int i1 = 0;
                while(strcmp(buf, "Done\n") != 0){
                    if(i1 > 0 && i2 > 0){
                        Fputs(buf, stdout);
                    }
                    else if(i2 == 0){
                        Fputs(buf,stdout);
                    }
                    Rio_readlineb(&rio,buf,MAXLINE);
                    i1++;
                }
                Rio_readlineb(&rio,buf,MAXLINE);
                i = 1;
                i2++;

                //prompt user to enter another positive integer
                printf("\n--> Enter A Positive Integer (^d to quit): ");
            }
            else{
                //catch error and prompt user to enter a positive integer
                printf("\n--> Please Enter a Positive Integer (^d to quit): ");
            }
        }
        else{
            //catch error and prompt user to enter a positive integer
            printf("\n--> Please Enter a Positive Integer (^d to quit): ");
        }
    
    }
    //disconnection/closing message
    printf("\n--> Disconnected from %s port %s. Goodbye.\n", argv[1], argv[2]);
    Close(clientfd); //line:netp:echoclient:close
    exit(0);
}
/* $end echoclientmain */