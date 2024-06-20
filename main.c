#include "server.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>


void getHTMLindexFile(char* buffer){
    FILE *fp;
    if((fp = fopen("./index.html", "a+"))==NULL){
        perror("can't open index.html file\n");
        exit(1);
    }else{
        int c;
        int i = 0;
        while((c = getc(fp))!=EOF){
            buffer[i++] = c;
        }
    }
}

void run(Server *Server){ 
    char buffer[30000];
    int addressLength = sizeof(Server->address);
    char* datetime_format = "%a, %d %b %Y %H:%M:%S GMT";
    char datetime_buffer[100];
    time_t t = time(NULL);
    
    strftime(datetime_buffer, sizeof(datetime_buffer), datetime_format, gmtime(&t));
    char response[10000];
    char html[2048];
    getHTMLindexFile(html);
    sprintf(response, "HTTP/1.1 200 OK\n"
    "Date: %s\n"
    "Server: Desperado/1.0.0 (Win64)\n"
    // "Last-Modified: Wed, 22 Jul 2009 19:15:56 GMT\n"
    "Content-Type: text/html\n"
    "Connection: Closed\n\n"
    "%s", datetime_buffer, html);
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
    serverr.run(&serverr);
}
