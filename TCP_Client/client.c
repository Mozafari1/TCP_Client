#include<stdio.h>
#include<winsock2.h>
#include<io.h>

#pragma comment(lib,"ws2_32.lib") //Winsock Library
#pragma warning(disable : 4996)

int main(int argc, char *argv[])
{
	WSADATA wsa;
	SOCKET s;
	struct sockaddr_in client;
	char *message="216986", server_reply[512];
	int recv_size;
	printf("\nInitialising Winsock... Client socket");
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
	{
		printf("Failed. Error Code : %d", WSAGetLastError());
		return 1;
	}
	printf("Initialised.\n");
	//Create a socket
	if ((s = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET)
	{
		printf("Kunne ikke lage socket: %d", WSAGetLastError());
	}
	printf("Socket var laget\n");	
	client.sin_addr.s_addr = inet_addr("158.36.70.56");
	client.sin_family = AF_INET;
	client.sin_port = htons(80);
	//Connect to remote server
	if (connect(s, (struct sockaddr *)&client, sizeof(client)) < 0)
	{
		puts("Det gikk galt med kobling");
		return 1;
	}
	puts("Koblet");
	// arrayet jeg sender skal se slik ut [ meldings ID, data type av short, medlingings størrelse, student nr]
	// setter bufferet til null

	memset(server_reply, 512,0);
	// meldingens id og den ligger på første plassen
	server_reply[0] = 0x01;// arryet ser slik ut [0x01]
	// lager en meldings størrelse i short med 7 plasser
	unsigned M_lengde = htons((short)0x07);
	// kopierer en block med data fra en kilde address til en plassering address.
	memcpy(server_reply+1, &M_lengde, 0x02);// arryet ser slik ut [0x01,0x00, 0x07]
	memcpy(server_reply+4, message, 0x06); // arryet ser slik ut [0x01,0x00,0x07,0x06,0,1,2,4,5,6]

	recv_size = send(s, server_reply,10, 0);// bruker 10 fordi lengde til bufferet 10 
	if (recv_size<0) {
		printf("Det gikk galt med sende data til serveren");
	}
	else {
		printf("\nData var sendt til servern %s", server_reply);
	}
	memset(server_reply, 512,0);
	if((recv_size=recv(s, server_reply, sizeof(server_reply),0)) == SOCKET_ERROR) {
		puts("\nrecv failed");
	}
	printf("\nSvar fra serveren\n");
	server_reply[recv_size] = '\0';
	printf(server_reply);
	closesocket(s);
	WSACleanup();
	return 0;
}
