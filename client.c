#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
 
// #define MYPORT 8887
// char* SERVERIP = "127.0.0.1";
 

 
void echoClient(int socketFD, char* serverIP, int serverPort )
{
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(serverPort);
    servaddr.sin_addr.s_addr = inet_addr(serverIP);
    
    int ret;
    char sendbuf[1024] = {0};
    char recvbuf[1024] = {0};
    while (1){
        printf("Enter message: ",sendbuf);
        if(fgets(sendbuf, sizeof(sendbuf), stdin) == NULL)
            break;
        printf("Send: %s",sendbuf);
        sendto(socketFD, sendbuf, strlen(sendbuf), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
        
        ret = recvfrom(socketFD, recvbuf, sizeof(recvbuf), 0, NULL, NULL);
        if (ret == -1)
        {
            if (errno == EINTR)
                continue;
            perror("recvfrom");
        }
        printf("Recive: %s\n",recvbuf);
        
        memset(sendbuf, 0, sizeof(sendbuf));
        memset(recvbuf, 0, sizeof(recvbuf));
    }
    
    close(socketFD);
    
    
}
 
int main(int argnum, char *argv[]){

    char *serverIP = argv[1];
    int serverPort = atoi(argv[2]);
    int socketFD;
    if ((socketFD = socket(PF_INET, SOCK_DGRAM, 0)) < 0){
        perror("socket");
        exit(EXIT_FAILURE);
    }
    printf("Sending message to %s:%d\n", serverIP, serverPort);
    echoClient(socketFD, serverIP, serverPort);
    
    return 0;
}