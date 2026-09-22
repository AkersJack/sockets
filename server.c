#include <stdlib.h>
#include <netinet/in.h>
#include <stdio.h> 
//#include <errno.h> 
#include <string.h> 
#include <sys/socket.h> 
#include <unistd.h> 
#include <netdb.h> /* Holds address information for sockets, primarily returned by the getaddrinfo() function */

#define BUF_SIZE 1024


void handle_client(void *sock){
        int client_socket = *((int*) sock); 
        char mbuff[BUF_SIZE]; 
        printf("server: got connection on socket %d\n", client_socket); 
        int numbytes = 0; 

        ssize_t bytes_received; 

        while(1){
                memset(mbuff,  0, sizeof(mbuff)); 
                printf("Waiting for client...\n"); 
                numbytes = recv(client_socket, mbuff, BUF_SIZE, 0); 
                if (numbytes == -1){
                        perror("recv"); 
                        close(client_socket); 
                        break;

                }else if(numbytes == 0){
                        printf("Client disconnected gracefully (Socket FD: %d).\n", client_socket); 
                }
        }

        printf("%s\n", mbuff); 
}



int main(int argc, char *argv[]){
        /* Argv1 is port */ 

        if(argc != 2){
                fprintf(stderr, "Usage: %s port\n", argv[0]); 
                exit(EXIT_FAILURE); 
        }

        char *port = argv[1]; 
        printf("Server started on port %s\n", port); 

        int gai, sfd, new_fd; 
        char buf[BUF_SIZE]; 
        ssize_t nread; 
        socklen_t peer_addrlen; 
        struct addrinfo sockInfo; 
        struct addrinfo *result, *rp; 
        struct sockaddr_storage peer_addr; 
        struct sockaddr_in server_addr; 
        socklen_t server_addrlen = sizeof(server_addr);

        memset(&sockInfo, 0, sizeof(sockInfo)); 
        sockInfo.ai_family = AF_UNSPEC; // Allow IPV4 or IPV6
        sockInfo.ai_socktype = SOCK_STREAM; 
        sockInfo.ai_flags = AI_PASSIVE; 
        sockInfo.ai_protocol = IPPROTO_TCP; // TCP protocol
        sockInfo.ai_canonname = NULL; 
        sockInfo.ai_addr = NULL; 
        sockInfo.ai_next = NULL; 



        /* Returns a list of address structures. */
        gai = getaddrinfo(NULL, port, &sockInfo, &result); 
        if(gai != 0){
                fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(gai)); 
                exit(EXIT_FAILURE); 
        }

        /* Try each address until we successfully bind. */
        for (rp = result; rp != NULL; rp = rp->ai_next){
                sfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol); 
                if (sfd == -1)
                        continue;

                if(bind(sfd, rp->ai_addr, rp->ai_addrlen) == 0){
                        getsockname(sfd, (struct sockaddr*)&server_addr, &server_addrlen); 
                        char ip_str[INET_ADDRSTRLEN]; 
                        printf("Socket bound at address: %s:%d\n", ip_str, ntohs(server_addr.sin_port)); 
                        break;

                }
                close(sfd); 
        }

        /* No longer needed */ 
        freeaddrinfo(result); 

        /* No address succeeded */ 
        if (rp == NULL){
                fprintf(stderr, "Failed to bind\n"); 
                exit(EXIT_FAILURE); 
        }


        if(listen(sfd, 10) == -1){
                perror("listen"); 
                close(sfd); 
                exit(EXIT_FAILURE); 
        }


        peer_addrlen = sizeof(peer_addr); 

        while(1){
                new_fd = accept(sfd, (struct sockaddr *)&peer_addr, &peer_addrlen); 
                printf("new fd: %d\n", new_fd); 


                if (new_fd == -1){
                        perror("accept failed"); 
                        continue; 
                }

                handle_client(&new_fd);
        }






        printf("End of the server\n"); 
        close(new_fd); 
        close(sfd); 
        return 0; 
}
