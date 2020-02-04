//tcp server workflow: socket creation-> binding the socket to the ip and port where it is should be listening to-->listening to that addresss and port--> accept the connection-->send or receive data

#include<stdint.h>
#include<stdio.h>
#include<stdlib.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/in.h>
#include<unistd.h>
#include<arpa/inet.h>
#include<string.h>


int ssock1,sockC;
int mtuB=10;
int mtuC;

struct packetMsg{
    char msg[200];
   
    int Dest;
    int pkt_len;
};
struct packetErr{
    char er_msg[100];
    int mtu;
};

struct packetMsg pktB;
struct packetErr packB;

int connectSocketA(){
    //printf("In fun");
    ssock1 = socket(AF_INET,SOCK_STREAM,0);
    //printf("%d",ssock1);
    struct sockaddr_in server_address;
    server_address.sin_family=AF_INET;
    server_address.sin_port=htons(9005);
    server_address.sin_addr.s_addr=INADDR_ANY;
    int bret=bind(ssock1,(struct sockaddr*)&server_address,sizeof(server_address) );
    if(bret<0){
    printf("%d",bret);
    }
    
    int lret=listen(ssock1,10);
    if(lret<0){
    printf("%d",lret);
    }
    int client_socket; 
    client_socket=accept(ssock1,NULL,NULL); 
    //printf("%d",client_socket);
    return client_socket;    

}


int connectSocketC(){
   // printf("In fn\n");
    sockC = socket(AF_INET,SOCK_STREAM,0);
    //printf("%d",sockC);

    struct sockaddr_in server_address1;
    server_address1.sin_family=AF_INET;
    server_address1.sin_port=htons(9000);
    server_address1.sin_addr.s_addr=INADDR_ANY;

    int con_stats= connect(sockC,(struct sockaddr *)&server_address1, sizeof(server_address1)); 
    if(con_stats==-1){
        printf("unsuccessful connection establishment");
    }
    recv(sockC,(void *)&packB,sizeof(packB),0);
     mtuC=packB.mtu;
    //printf("%d\n",mtuC);
    return sockC;
    
}

void Dest1(int c_sock){
    printf("Data packet:%s\n",pktB.msg);
    strcpy(packB.er_msg,"Fragment received");
    send(c_sock,(void*)&packB,sizeof(packB),0);
}

void Dest2(int c_sock1){
    if(pktB.pkt_len>mtuB){
       strcpy(packB.er_msg,"Fragmentation needed");
       packB.mtu=mtuB;
       send(c_sock1,(void *)&packB,sizeof(packB),0);
       int ret6=recv(c_sock1,(struct packetMsg*)&pktB,sizeof(pktB),0);
       //printf("%d",ret6);
       printf("%s\t%d\t%d\n",pktB.msg,pktB.Dest,pktB.pkt_len);
       }
       printf("Connecting C\n");
       int c_sock2=connectSocketC();
       pktB.Dest--;
       if(mtuC>=pktB.pkt_len){
           
           int ret3=send(c_sock2,(void *)&pktB,sizeof(pktB),0);
           //printf("%d",ret3);
           printf("%s\t%d\t%d\n",pktB.msg,pktB.Dest,pktB.pkt_len);


            strcpy(packB.er_msg,"Fragment received");
            send(c_sock1,(void*)&packB,sizeof(packB),0);

           
       }else if(mtuC<pktB.pkt_len){
                strcpy(packB.er_msg,"Fragmentation needed");
                packB.mtu=mtuC;

                send(c_sock1,(void *)&packB,sizeof(packB),0);

                int ret5=recv(c_sock1,(struct packetMsg *)&pktB,sizeof(pktB),0);
                //printf("%d",ret5);
                //printf("fragmented data:\n %s\t%d\t%d",pkt.msg,pkt.Dest,pkt.pkt_len);


                //pktB.Dest--;
                int ret3=send(c_sock2,(void *)&pktB,sizeof(pktB),0);
                //printf("%d",ret3);
                printf("%s\t%d\t%d\n",pktB.msg,pktB.Dest,pktB.pkt_len);

                strcpy(packB.er_msg,"Fragment received");
                send(c_sock1,(void*)&packB,sizeof(packB),0);




    }

}
    


    

int main(){
    
    
    int i;
   
   // int valread;
    
       
    
    int client_socket1=connectSocketA();
    // printf("Returned to main\n");
    int client_socket2;
    

   int ret=recv(client_socket1,(struct packetSending *)&pktB,sizeof(pktB),0);
   //printf("%d",ret);
   //printf("Data received:%s\t%d\t%d\n",pkt.msg,pkt.Dest,pkt.pack_l);
   
   
   if(pktB.Dest==1){
    
           Dest1(client_socket1);

        
   }
    else if(pktB.Dest==2){
       
            Dest2(client_socket1);
  

    }

   
    close(ssock1); 
    //close(client_socket2);
    
    return 0;

}

 