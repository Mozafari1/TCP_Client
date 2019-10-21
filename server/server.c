// This is a simple sever code written in C language

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>

void error(char *msg)
{
        perror(msg);
        exit(1);}
int main(int argc, char * argv[]){
        int sockfd, newsockfd, portno, clilen;
        char buffer[4096];
        struct sockaddr_in serv_addr, cli_addr;
        int n;
        if(argc<2){
                fprintf(stderr, "ERROR, no port provided\n");
                exit(1);
        }
        sockfd = socket(AF_INET, SOCK_STREAM,0);
        if(sockfd <0){
                error("ERROR opening socket");
        }
        bzero((char *) &serv_addr, sizeof(serv_addr));
        portno = atoi(argv[1]);
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_addr.s_addr = INADDR_ANY;
        serv_addr.sin_port = htons(portno);
        if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0){
                error("ERROR on binding" );
        }
        listen(sockfd,5);
        clilen  = sizeof(cli_addr);
        newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
        if(newsockfd<0){
                error("Error on accpet");
        }
        while(1){
        bzero(buffer,4096);
        n = read(newsockfd, buffer, 4095);
        if(n<0){
                error("Error reading from socket");
        }
        printf("Here is the meesage: %s\n", buffer);
        n = write(newsockfd, "Ping" ,4);
        printf("ping var send \n");
        
        
        bzero (buffer, 4096);
        n=write(newsockfd, "Quit", 4);
        printf("Quit var sent til serveren\n");
        }
        close(sockfd);
        exit(0);
        return 0;
}

