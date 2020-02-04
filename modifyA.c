//tcp client workflow : socket create--> connect--> recv-->close the socket


#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<string.h>

struct packetMsg{
    char msg[200];
    int Dest;
    int pkt_len;
};

struct packetErr{
    char er_msg[100];
    int mtu;
};

struct packetMsg pktA;    
struct packetErr packA[2];

int connectSocket(){
    int sock= socket(AF_INET,SOCK_STREAM,0); 
    //printf("%d",sock);

    struct sockaddr_in server_address;
    server_address.sin_family =AF_INET;
    server_address.sin_port = htons(9005);
    server_address.sin_addr.s_addr = INADDR_ANY; 
    

    int con_stats= connect(sock,(struct sockaddr *)&server_address, sizeof(server_address)); 
    if(con_stats==-1){
        printf("unsuccessful connection establishment");
    }
    return sock;
}


int main(){
    int i;
    char arr[200]="abcdefghijklmnopqrstuvwxyz";
    int sock=connectSocket();
    
    
    strcpy(pktA.msg,arr);
    pktA.Dest=2;
    pktA.pkt_len=strlen(pktA.msg);
    printf("%s\t %d\t %d\n",pktA.msg,pktA.Dest,pktA.pkt_len);
    int s=send(sock,(void *)&pktA,sizeof(pktA),0);
    //printf("%d\n",s);

   
    recv(sock,(struct packetErr*)&packA[0],sizeof(packA[0]),0);
    
    if(strcmp(packA[0].er_msg,"Fragment received")==0){
        printf("%s\n",packA[0].er_msg);
    }
    else if(strcmp(packA[0].er_msg,"Fragmentation needed")==0){
        printf("%s: %d\n",packA[0].er_msg,packA[0].mtu);
        for(i=0;i<pktA.pkt_len;i++){
            if(i>=0 &&i<packA[0].mtu){

            }
            else{
                pktA.msg[i]=NULL;
            }
        }
        
        pktA.pkt_len=strlen(pktA.msg);
        printf("%s\t%d\n",pktA.msg,pktA.pkt_len);
        int sret=send(sock,(struct packetMsg*)&pktA,sizeof(pktA),0);
        //printf("%d",sret);

    }


    recv(sock,(struct packetMsg *)&packA[1],sizeof(packA[1]),0);
    if(strcmp(packA[1].er_msg,"Fragment received")==0){
        
        printf("%s\n",packA[1].er_msg);
        
    }
    else if(strcmp(packA[1].er_msg,"Fragmentation needed")==0){
        printf("%s: %d\n",packA[1].er_msg,packA[1].mtu);
        for(i=0;i<pktA.pkt_len;i++){
            if(i>=0 &&i<packA[1].mtu){

            }
            else{
                pktA.msg[i]=NULL;
            }
        }
        pktA.Dest--;
        pktA.pkt_len=strlen(pktA.msg);
        printf("%s\t%d\n",pktA.msg,pktA.pkt_len);
        int sret=send(sock,(void *)&pktA,sizeof(pktA),0);
        //printf("%d",sret);

    }

    recv(sock,(struct packetErr *)&packA[1],sizeof(packA[1]),0);
    if(strcmp(packA[1].er_msg,"Fragment received")==0){
        printf("%s\n",packA[1].er_msg);
    }
   // else if(strcmp(packA[1].er_msg,"Fragmentation needed")==0){
    //    printf("%s: %d\n",packA[1].er_msg,packA[1].mtu);
    
  //  }

    close(sock);




    return 0;
}