

#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>

#include<netinet/in.h>
#include<strings.h>

int sock;

struct packetMsg{
    char msg[200];
    int Dest;
    int pkt_len;
};

struct packetErr{
    char er_msg[100];
    int mtu;
};

struct packetMsg pktC;
struct packetErr packC;

int connectSocket(){
    //printf("Inside fn\n");
    sock= socket(AF_INET,SOCK_STREAM,0);
    //printf("%d",sock);
    struct sockaddr_in server_address;
    server_address.sin_family=AF_INET;
    server_address.sin_port=htons(9000);
    server_address.sin_addr.s_addr=INADDR_ANY;
    int bret=bind(sock,(struct sockaddr*)&server_address,sizeof(server_address));
    if(bret<0){
            printf("%d",bret);
    }
    
    int lret=listen(sock,10);
    if(lret<0){
            printf("%d",lret);
    }
    int client_socket;
    client_socket=accept(sock,NULL,NULL);
    //printf("%d",client_socket);
    packC.mtu=10;
    send(client_socket,(void*)&packC,sizeof(packC),0);
    return client_socket;
  }

    void packet_recv(int c_sock){
        
        int ret=recv(c_sock,(struct packetMsg *)&pktC,sizeof(pktC),0);
        pktC.Dest--;
        //printf("%d",ret);
        printf("Data Packet:%s\n",pktC.msg);
        printf("Destination:%d\n",pktC.Dest);
    }


int main(){
    int client_socket=connectSocket();
    packet_recv(client_socket);
    close(client_socket);
    return 0;
}