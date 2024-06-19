#include "server.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>

void run(Server *Server){ 
    char buffer[30000];
    int addressLength = sizeof(Server->address);
    char* response = "HTTP/1.1 200 OK\n"
    "Date: Mon, 27 Jul 2009 12:28:53 GMT\n"
    "Server: Desperado/1.0.0 (Win64)\n"
    "Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT\n"
    "Content-Type: text/html\n"
    "Connection: Closed\n\n"
    "<html><body><h1>Hello, World!</h1></body></html>";
    int newSocket;
    while(1){
        printf("====== WAITING ======\n");
        newSocket = accept(Server->socket, (struct sockaddr *)&Server->address, (socklen_t *)&addressLength);
        read(newSocket, buffer, 30000);
        printf("%s\n", buffer);

        write(newSocket, response, strlen(response));
        close(newSocket);
    }
    
}

int main(){
    Server serverr = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 80, 10, run);
    printf("hello here 2\n");
    serverr.run(&serverr);
}