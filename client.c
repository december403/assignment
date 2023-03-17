#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <sys/time.h>
#define MAXINTERVAL  8000
 
int echoClient(int socketFD, struct sockaddr_in servaddr, int retryNum)
{
    // 初始化相關參數
    int attemptNum = 0;
    struct timeval timeout = {0,500};
    char sendbuf[1024] = {0};
    char recvbuf[1024] = {0};
    
    //獲取要傳送的資料
    printf("Enter message: ");
    if(fgets(sendbuf, sizeof(sendbuf), stdin) == NULL){
        perror("fget error");
        exit(EXIT_FAILURE);
    }
            
    
    while (true){
        // 檢查 retry 次數以及最大等待間隔是否超過限制
        if(attemptNum>retryNum) 
            break;
        if((timeout.tv_sec*1000 + timeout.tv_usec) > MAXINTERVAL) 
            break;
        
        // 傳送資料
        printf("(Attempt #%d) Send: %s",attemptNum+1, sendbuf);
        printf("(Attempt #%d) Interval: %dms\n",attemptNum+1,(timeout.tv_sec*1000 + timeout.tv_usec));
        sendto(socketFD, sendbuf, strlen(sendbuf), 0, (struct sockaddr *)&servaddr, sizeof(servaddr));
        
        // 設定接收資料的 timeout 後等待 server 回傳資料
        setsockopt(socketFD,SOL_SOCKET,SO_RCVTIMEO,&timeout,sizeof(timeout));
        int ret = recvfrom(socketFD, recvbuf, sizeof(recvbuf), 0, NULL, NULL);
        if(ret == -1)
        {
            // 若是沒收到回傳的資料，加倍下次等待的 timeout
            attemptNum++;
            int msec = (timeout.tv_usec*2) % 1000;
            int sec = timeout.tv_sec * 2 + (timeout.tv_usec*2)/1000;
            timeout.tv_sec = sec;
            timeout.tv_usec = msec;
        }
        else{
            printf("Recive: %s\n",recvbuf);
            close(socketFD);
            return 0;
        }
    }
    close(socketFD);
    return 1;
}
 

 /*
 使用說明:
    $ client [server IP address] [server port] [max-retry]
    (For example:  client 127.0.0.1 3000 10)
 */
int main(int argnum, char *argv[]){
    // 初始化 socket 與 server IP 位址的參數
    char *serverIP = argv[1];
    int serverPort = atoi(argv[2]);
    int socketFD;
    int retryNum = atoi(argv[3]);
    struct sockaddr_in servaddr;
    memset(&servaddr, 0, sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(serverPort);
    servaddr.sin_addr.s_addr = inet_addr(serverIP);
    if ((socketFD = socket(PF_INET, SOCK_DGRAM, 0)) < 0){
        perror("socket");
        exit(EXIT_FAILURE);
    }
    printf("Sending message to %s:%d\n", serverIP, serverPort);
    if(echoClient(socketFD, servaddr, retryNum) == 1){
        printf("Sending Message Failed\n");
        return 1;
    }
    
    return 0;
}