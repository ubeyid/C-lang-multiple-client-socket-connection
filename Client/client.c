#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h> 
#include <unistd.h>
#include <pthread.h>  



#define SIZE 1024
char received_data[SIZE]; 
char sent_data[SIZE];

int main(){


    const char* server_adress = "localhost";
    const int server_port = 1453;

    //Setting up the server that we want to connect 
    struct sockaddr_in server_info;
    memset(&server_info, 0, sizeof(server_info));
    server_info.sin_family = AF_INET;
    inet_pton(AF_INET, server_adress, &server_info.sin_addr);
    server_info.sin_port = htons(server_port);

    int path = socket(AF_INET, SOCK_STREAM, 0);

    // Connecting to the server
    if( connect(path, (struct sockaddr*)&server_info, sizeof(server_info)) < 0 ){
        fprintf(stderr, "Something went wrong suring connecting to server :( :(");
        return 1;
    }
    
    printf("Connected server :) :)\n");
    
   int state=0;//this is for determining the state that we are sending data or receiving data
   
   while(1){
       if(state == 0){
           printf("Enter your message:");
           scanf("%s",&sent_data);   
           int sended=send(path,&sent_data,sizeof(sent_data),0);
           if(sended < 0){
            fprintf(stderr, "Message couldn't send...\n");
 
           }
           if(sended == 0){
               //that means our connection corrupted broken or something like that
               //so close break te loop and close the socket
               break;
           }
           if(sended > 0){
               state=1;//switch to receiving state
               printf("Message sent :)\n");
           }
       }
       if(state == 1){
            int data_length = recv(path, &received_data, sizeof(received_data), 0);

            if(data_length < 0){
               fprintf(stderr, "Something went wrong during receiving data :(\n");

            } 
            if(data_length == 0){
                break;//that means connection has a problem so break the loop
            }
            if(data_length > 0){
               printf("Server message:%s\n",received_data);
               state=0;//switch to sending state
            }
       }
       
   }    
    
      
    close(path);//if program receive here that means we had a problem so close the socket
    printf("Server connection corrupted ,broken or something like that :( :(\n");
   
    
    
    return 0;
}