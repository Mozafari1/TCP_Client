#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>

void error (char *msg){
        perror(msg);
        exit(0);
}


//buffer char = { 0x01, 0x00, 0x07 0x06 sn i ascii}
//lese melding // lese hva slags melding det er 
//int (recvBytes = revc(clientsocket, (byte) Greie, )
int main (int argc, char *argv[]){
        int sockfd, portno,n;
        struct sockaddr_in serv_addr;
        struct hostent *server;
       	char buffer[512];
	char *ID = "216986";
	
                if(argc <3){
                fprintf(stderr, "Bruk %s hostname og portnummer\n", argv[0]);
                exit(0);
                }

        portno = atoi(argv[2]);

        sockfd = socket(AF_INET, SOCK_STREAM,0);
        if(sockfd<0){
                error("Noe gikk galt ved å åpnet socket");
        }
        server = gethostbyname (argv[1]);
        if(server==NULL){
                fprintf(stderr, "Du har ikke angit hostname eller portnummer\n");
                exit(0);
        }
        bzero((char *) &serv_addr, sizeof(serv_addr));
        serv_addr.sin_family = AF_INET;
         bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,server->h_length);
         serv_addr.sin_port = htons(portno);
        if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
                error("Det gikk noe galt med din internett forbindelse");

        }
	bzero(buffer, 512);
	buffer[0] = 0x01;
	unsigned M_lengde= htons((short)0x07);
	memcpy(buffer +1, &M_lengde,2);
	memcpy (buffer+ 4,ID, 0x06);
	n= send(sockfd, buffer,10,sizeof(buffer));
	if(n<0){
                error("Det gikk galt med sende data til serveren" );
        }
        else{
                printf("\nData var sendt til servern %s\n ",buffer);
	
	    }

	bzero(buffer, 512);
	n = read(sockfd, buffer,sizeof(buffer));	
	if(n<0){
                error("Det gikk galt ved å ta i mott data fra serveren" );
        }
        printf("\nReceving data .. %s", buffer);
	

	printf("\n");


	close(sockfd);

        return 0;
}


