#include <iostream>
#include <string>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
using namespace std;
int PortNumber = 20000, sock; 
const string IpAddress = "127.0.0.1";
char buffer[4096];

void creatsocket(){
        struct sockaddr_in serv_addr;
	sock = socket(AF_INET, SOCK_STREAM, 0);
	cout <<"this is sock "<<sock<<endl;
        if(sock<0){
                cout<<"SOCKET Failed, try agin"<<endl;
        }else{
		cout <<"Socket was successed: "<<sock<<endl;
	}
        cout <<"Status connecting on IP [ "<< IpAddress<< " ] and on Port [ " <<PortNumber<<" ]" <<endl;
        serv_addr.sin_addr.s_addr = inet_addr(IpAddress.c_str());
        serv_addr.sin_family = AF_INET;
        serv_addr.sin_port = htons(PortNumber);
        if((connect(sock,(struct sockaddr *)&serv_addr, sizeof(serv_addr))<0)){
			cout <<"Connecting was failed and try it agin"<<endl;
			}
			else{
			cout <<"Connecting was successed "<<endl;
			}
}
void sendstud(){
   
        memset(buffer, 4096, 0);
        buffer[0]= 0;
	char *nr = "216986";
        char id =0x01;
        unsigned short messageSize = sizeof(messageSize) + sizeof(id) + strlen(nr);
        memcpy(buffer, &messageSize, sizeof(messageSize));
	memcpy(buffer+2, &id, 1);
	memcpy(buffer+3, nr, 6);
	if(send(sock,buffer,9,sizeof(buffer))<0){
		cout<<"message was failed, try it agin"<<endl;
	}else{
		cout <<"message was successful sent" <<endl;
	}
}
void hentnewport(){
        memset(buffer, 4096, 0);
        if((recv(sock, buffer, sizeof(buffer),0)<0)){
                cout<<"Something went wrong to receving data from serveren"<<endl;
        }
        else {
                short newport;
                memcpy( & newport, buffer+3, sizeof(newport));

                cout <<"Here is the new port, and it makes auto connect "<<htons(newport)<<endl;
                PortNumber = htons(newport);   
        }
}
void pingpong(){
        while(1){
        buffer[0]= 0;
        char pong =0x04;
        unsigned short messageSize = sizeof(messageSize);
        memset(buffer, 4096, 0);

        memcpy(buffer, &messageSize, sizeof(messageSize));
	memcpy(buffer+2, &pong, sizeof(pong));
       
        if((recv(sock, buffer, sizeof(buffer),0)<0)){
                cout<<"Something went wrong to receving data from serveren"<<endl;
        }
        else if(buffer[2]==0x03)
        {
            send(sock,buffer,9,sizeof(buffer));
            cout<<"buf2 "<<htons(buffer[2])<<endl;
        }
        else{
                break;
        }

        }
}
int main(){
        creatsocket(); // lager socket 
        sendstud();     // sender student nummer
        hentnewport();  // henter den nye porten
        creatsocket();  // lager socket med den nye porten
        pingpong();     // sender pong når ping kommer, og avslutter dersom det kommer noe annet enn ping 0x03
        return  0;
}





