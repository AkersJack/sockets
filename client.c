#include <netinet/in.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <netdb.h> 
#include <sys/socket.h>
#include <unistd.h> 

#define BUF_SIZE 1024


typedef struct Message{
        char *buffer; // The message itself 
        size_t size; // Size of the message
} Message; 


void *connect_to_server(const char *address, const char *port){
        int gai, numbytes; 
        char buf[BUF_SIZE]; 
        size_t len; 
        ssize_t nread; 
        int *sfd = malloc(sizeof(int)); 

        struct addrinfo sockInfo; 
        struct addrinfo *result, *rp; 


        // Obtain address(es) matching host/port
        sockInfo.ai_family = AF_UNSPEC; 
        sockInfo.ai_socktype = SOCK_STREAM; 
        sockInfo.ai_flags = 0; 
        sockInfo.ai_protocol = IPPROTO_TCP; 



        gai = getaddrinfo(address, port, &sockInfo, &result);
        if (gai != 0){
                fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(gai)); 
                exit(EXIT_FAILURE); 
        }

        for (rp = result; rp != NULL; rp = rp->ai_next){
                *sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol); 
                if(*sfd == -1)
                        continue; 
                if (connect(*sfd, rp->ai_addr, rp->ai_addrlen) != -1){
                        printf("socket connected\n"); 
                        break; 
                }
                close(*sfd); 
        }
        
        freeaddrinfo(result); 
        
        if (rp == NULL){
                fprintf(stderr, "Could not connect\n"); 
                exit(EXIT_FAILURE); 
        }

        return sfd; 

}


int main(int argc, char *argv[]){
        char *message = "Hello from the client"; 
        size_t message_len = strlen(message); 
        int *sock; 

        if(argc != 3){
                fprintf(stderr, "Usage: %s host port \n", argv[0]); 
                exit(EXIT_FAILURE);
        }

        printf("Address: %s\n", argv[1]); 
        printf("Port: %s\n", argv[2]); 


        sock = (int*)connect_to_server(argv[1], argv[2]); 
        if(sock == NULL){
                fprintf(stderr, "Could not connect\n"); 
                exit(EXIT_FAILURE); 
        }
        
        if(send(*sock, message, message_len, 0) == -1){
                perror("send"); 
                close(*sock); 
                exit(EXIT_FAILURE); 
        }
        printf("Sent message\n"); 



        return 0;
        
        


}
