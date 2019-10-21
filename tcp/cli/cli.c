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
    char* studentID = "215138";
    memcpy(buffer + 4, studentID, 6);
    n = write(sockfd, buffer, 10);
    for (int i = 0; i < 10; ++i)
       // printf("%x ", *(((char*)buffer) + i));
    if (n < 0)
        error("ERROR writing to socket");
    bzero(buffer, 1024);
    n = read(sockfd, buffer, 1024);
    if (n < 0)
        error("ERROR reading from socket");
   // printf("%s", buffer);

    unsigned char messageID;
    unsigned short messageSize;
    unsigned short textSize;
    memcpy(&messageID, buffer, 1);
    memcpy(&messageSize, buffer + 1, 2);
    memcpy(&textSize, buffer + 3, 2);
    messageSize = ntohs(messageSize);
    textSize = ntohs(textSize);

    printf("%i %i %i %s", messageID, messageSize, textSize, buffer + 5);
    
    return 0;
}
