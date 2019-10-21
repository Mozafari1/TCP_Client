#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(char* msg)
{
    perror(msg);
    exit(1);
}

int main(int argc, char const *argv[])
{
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;

    char buffer[1024];
    if (argc < 3) {
        fprintf(stderr, "usage %s hostname port", argv[0]);
        exit(0);
    }
    portno = atoi(argv[2]);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
        error("ERROR opening socket");

    server = gethostbyname(argv[1]);
    if (server == NULL) {
        fprintf(stderr, "ERROR, no such host");
        exit(0);
    }

    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char*)server->h_addr, (char*)&serv_addr.sin_addr.s_addr,
        server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
        error("ERROR connecting");
    bzero(buffer, 1024);
    buffer[0] = 0x1;
    unsigned messageSize1 = htons((short)7);
    memcpy(buffer + 1, &messageSize1, 2);
    buffer[3] = 6;
    char* studentID = "216986";
    memcpy(buffer + 4, studentID, 6);
    n = write(sockfd, buffer, 10);
    for (int i = 0; i < 10; ++i)
       // printf("%x ", *(((char*)buffer) + i));
   if (n < 0)
        error("ERROR writing to socket");
   	printf("The first message was sent to server student nr: \n");
    bzero(buffer, 1024);
    n = read(sockfd, buffer, 1024);
    if (n < 0)
        error("ERROR reading from socket");
   printf("Receiving data [1] :%s\n ", buffer);


   while(1){
    bzero(buffer, 1024);
    buffer[0] = 0x3;
    unsigned messageSize1 = htons((short)0);
    memcpy(buffer + 1, &messageSize1, 2);
    buffer[3] = 4;
    char* pong = "Pong";
    memcpy(buffer + 4, pong, 4);
    n = write(sockfd, buffer, 8);
    for (int i = 0; i < 8; ++i)
       // printf("%x ", *(((char*)buffer) + i));
    if (n < 0)
        error("ERROR writing to socket");
    printf("the second message was sent to server in loop Pong\n");
    bzero(buffer, 1024);
    n = read(sockfd, buffer, 1024);
    if (n < 0)
        error("ERROR reading from socket");
   printf("Receiving data :%s\n ", buffer);


}
    return 0;
}
