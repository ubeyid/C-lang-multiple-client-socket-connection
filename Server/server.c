#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>  
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>  

#define PORT 1453

char received_data[1024];

void* client_thread(void* socket){
    
    int client=*((int *)socket);
    
    int state=1;
    while(1){
        //that means we read data from socket
        if(state == 1){
            
             int data_length = recv(client, (void *)received_data, sizeof(received_data), 0);

             if(data_length == 0){
                 //if this equals 0 that means connection corrupted broken or something like that
                 //so we need to break the loop and close the socket and destroy the thread for the client
                 break;
             } 
             if(data_length > 0){
                 //that means we have received data and we print data to console 
                 printf("Client says :%s\n",received_data);
                 //switch to sending state
                 state=0;
             }
        
        if(state == 0)
        {
        
            int sended=send(client, (void *)received_data, sizeof(received_data), 0);
             
            if(sended == 0){
                //that means we have a problem with connection 
                //so we need to break the loop and destroy the thread for client connected before
              break;
            }
            if(sended > 0){
              printf("Message has been sent :)\n");
              //switch to reading state
              state=1;
            }
        }
        
        
    }
    
  }
    close(client);
    printf("Client connection has been corrupted or broken :( :( \n");
    pthread_exit(NULL);
}
int main(){

 
    int path = socket(AF_INET, SOCK_STREAM, 0);
   
    // setting server settings
    struct sockaddr_in server_info;
    memset(&server_info, 0, sizeof(server_info));  // Reset data structure
    server_info.sin_family = AF_INET;
    server_info.sin_addr.s_addr = htonl(INADDR_ANY);  // Or inet_addr("127.0.0.1")
    server_info.sin_port = htons(PORT);

    // binding adress to 
    if( bind(path, (struct sockaddr*)&server_info, sizeof(server_info)) < 0 ){
        fprintf(stderr, "Something went wrong during binding adress :( :( \n");
        return 1;
    }

    // Listening
    if( listen(path, 5) < 0 ){
        fprintf(stderr, "Something went wrong during listening port :( :( \n");
        return 1;
    }
   
    
    
    printf("listening...\n");
    // accepting connection requests.
    struct sockaddr_in client_info;
    socklen_t size = sizeof(client_info);
    int client_path;
   
    pthread_t clients[500];

    for(int i=0;i<500;i++){
         client_path = accept(path, (struct sockaddr*)&client_info, &size);
    
         if(client_path < 0){
            fprintf(stderr, "Something went wrong during accepting connection :( :( \n");
            return 1;
         }
         
         pthread_create(&clients[i],NULL,client_thread,(void *)&client_path);
         
         printf("New client connected :) :) \n");
         
         
    }
   

    return 0;
 }
