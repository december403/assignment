# The assignment from M800

## 如何編譯
* `$ gcc server.c -o server`
* `$ gcc client.c -o client`

## 如何執行
* `$ ./server [port]`
* `$ ./client [IP address] [port] [max-retry]`

### 使用範例
假設 server 與 client 在同一台電腦上，可透過
```
$ ./server 3000
$ ./client 127.0.0.1 3000 10
```
設定 server 監聽 port 3000 ，client 發送資料到 localhost 的 3000 port

