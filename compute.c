
/*
 * echo - read and echo text lines until client closes connection
 */
/* $begin echo */
#include "csapp.h"

//new argument "j" is the first index in "[HSS:j:k]"
void compute(int connfd, int j) 
{
    int n;
    int i; //used to hold integer sent from client
    char buf[MAXLINE]; 
    rio_t rio;
    int k = 2;  //second index in "[HSS:j:k]"

    //initialize read-write interface
    Rio_readinitb(&rio, connfd);
    while((n = Rio_readlineb(&rio, buf, sizeof buf)) != 0) {
        sscanf(buf, "%d", &i);
	    printf("Received %d\n", i); //received message for server
        printf("Sending sequence\n");   //sending sequence message for server

        //forming and writing starting Hailstone message to client
        char final[MAXLINE] = "";
        char p1[] = "[HSS:";
        char p2[] = ":";
        char p3[] = "] Starting with ";
        sprintf(final, "%s%d%s%d%s%d\n", p1, j, p2, k, p3, i);
        Rio_writen(connfd, final, strlen(final));
        
        k++;
        int m = 0;  //keeps track of number of iterations for final message
	
        //main loop for performing Hailstone sequence
        while(i != 1){
            if(i % 2 == 0){

                //update i according to Hailstone sequence
                i = i / 2;

                //forming and writing Hailstone message to client
                char final1[MAXLINE] = "";
                char p11[] = "[HSS:";
                char p22[] = ":";
                char p33[] = "] ";
                sprintf(final1, "%s%d%s%d%s%d\n", p11, j, p22, k, p33, i);
                Rio_writen(connfd, final1, strlen(final1));
            }
            else{

                //update i according to Hailstone sequence
                i = 3 * i + 1;

                //forming and writing Hailstone message to client
                char final2[MAXLINE] = "";
                char p111[] = "[HSS:";
                char p222[] = ":";
                char p333[] = "] ";
                sprintf(final2, "%s%d%s%d%s%d\n", p111, j, p222, k, p333, i);
                Rio_writen(connfd, final2, strlen(final2));
            }
            k++;
            m++;
        }

        //forming and writing iteration summary message
        char final5[MAXLINE] = "";
        char p1111[] = "[HSS:";
        char p2222[] = ":";
        char p3333[] = "] Finished. Number of iterations = ";
        sprintf(final5, "%s%d%s%d%s%d\n", p1111, j, p2222, k, p3333, m);
        Rio_writen(connfd, final5, strlen(final5));

        //flag message for client to read
        char final4[MAXLINE] = "Done\n";
        Rio_writen(connfd, final4, strlen(final4));
        k = 2;
        j++;
        return;
    }
}
/* $end echo */