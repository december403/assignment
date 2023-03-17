#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<errno.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<string.h>

void echoServer(int socketFD){
    char buffer[1024] = {0};
    struct sockaddr_in senderAddr;
    socklen_t len;
    int n;
    while(1){
        len = sizeof(senderAddr);
        memset(buffer, 0, sizeof(buffer));
        n = recvfrom(socketFD, buffer, sizeof(buffer), 0, (struct sockaddr *)&senderAddr, &len);
        if(n<=0){
            if(errno == EINTR) continue;
            else perror("recvfrom error");
        }
        else{
            sendto(socketFD, buffer, n, 0, (struct sockaddr *)&senderAddr, len);
            printf("%s\n", buffer);
        }
    }
    
    
    return;
}

int main(int argnum, const char *argv[]){
    int port = atoi(argv[1]);
    int socketFD = socket(AF_INET, SOCK_DGRAM, 0);
    if( socketFD < 0 ){
        perror("failed to create socket file descriptor");
        exit(EXIT_FAILURE);
    }
    struct sockaddr_in servAddr;
    memset(&servAddr, 0, sizeof(servAddr));
    servAddr.sin_family = AF_INET;
    servAddr.sin_port = htons(port);
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    printf("Listening on port %d\n", port);
    if( bind(socketFD, (struct sockaddr *)&servAddr, sizeof(servAddr)) < 0){
        perror("bind fail\n");
        exit(EXIT_FAILURE);
    }

    echoServer(socketFD);

    return 0;
}