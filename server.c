/* 
 * echoserverp.c - A concurrent echo server based on processes
 */
/* $begin echoserverpmain */
#include "csapp.h"
#include <stdbool.h>
void compute(int connfd, int j);

void sigchld_handler(int sig) //line:conc:echoserverp:handlerstart
{
    while (waitpid(-1, 0, WNOHANG) > 0)
	;
    return;
} //line:conc:echoserverp:handlerend

int main(int argc, char **argv) 
{
    int listenfd, connfd;
    socklen_t clientlen;
    struct sockaddr_storage clientaddr;
	char client_hostname[MAXLINE], client_port[MAXLINE];
	
    if (argc != 2) {
		fprintf(stderr, "usage: %s <port>\n", argv[0]);
		exit(0);
    }

	
	printf("Hailstone Sequence Server v1.0\n");	//server welcome message
	printf("Listening On Port %s\n\n", argv[1]);	//server listening message
    Signal(SIGCHLD, sigchld_handler);
    listenfd = Open_listenfd(argv[1]);
	int j = 1;	//first index in "[HSS:j:k]", sent to compute
	int a = 0;
	int x = 0;
	int k = 0; //counts number of active open connections
	printf("Waiting for connection...\n");
    while (1) {
		clientlen = sizeof(struct sockaddr_storage); 
		connfd = Accept(listenfd, (SA *) &clientaddr, &clientlen);
		k += 1;
		
		//getting name of host and port information
		Getnameinfo((SA *) &clientaddr, clientlen, client_hostname, MAXLINE, 
					client_port, MAXLINE, 0);
	
		//getting IP address


		char ipaddr[20] = "";
		Inet_ntop(((SA *) &clientaddr)->sa_family, &((struct sockaddr_in *) ((SA *) &clientaddr))->sin_addr, ipaddr, clientlen);
		printf("Server connected to %s (%s:%s)\n", client_hostname, ipaddr, client_port);
		
		
		//compose and write welcome message to client
		char final5[MAXLINE] = "";
        char p1111[] = "[HSS:";
        char p2222[] = ":1] Welcome to the Hailstone Sequence Server v1.0";
		if(j == 1){
			sprintf(final5, "%s%d%s\n", p1111, j, p2222);	
		}
		else{
			sprintf(final5, "%s%d%s\n", p1111, j+1, p2222);
		}
		
        Rio_writen(connfd, final5, strlen(final5));
		j = j + 1;
	
		a = 1;
		
		if (Fork() == 0) { 
			
	    	Close(listenfd); /* Child closes its listening socket */
			//call Compute to generate the Hailstone return messages 
			//to the client
	    	compute(connfd, j);    /* Child services client */ //line:conc:echoserverp:echofun
			printf("Closing connection\n\n"); //closing connection message
			//printf("Closing connection at port %s\n\n", client_port); //closing connection with PORT
			printf("Waiting for connection...\n");	//re-print waiting message
			Close(connfd);   /* Child closes connection with client */ //line:conc:echoserverp:childclose
			
			exit(0);         /* Child exits */
		
		}
		Close(connfd); /* Parent closes connected socket (important!) */ //line:conc:echoserverp:parentclose
    
	}
}
/* $end echoserverpmain */
