#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>


void error(char *msg)
{
        perror(msg);
        exit(1);
}

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
        //bzero(buffer, 4096);
 
 	//while(n= (read(newsockfd, buffer, 4095)){


        n = read(newsockfd, buffer, sizeof(buffer),0);


      if(n<0){
                error("Error reading from socket");

        }
      
        printf("Here is the meesage: %s", buffer);
	printf("\n");
        n = write(newsockfd, " I got ur message",18);
        if(n<0)
        {
                error("ERROR writing to socket");

        }
	
	close(sockfd);

	return 0;
}

