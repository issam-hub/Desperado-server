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
        fclose(fp);
    }
}

// void fileInDB(char* db, char* todo){
//     FILE* fp;
//     if((fp = fopen("./db.txt", "a"))==NULL){
//         perror("can't open db file\n");
//         exit(1);
//     }else{
//         for(; *todo != '\0'; ++todo){
//             printf("%c\t", *todo);
//             putc((int)*todo, fp);
//         }
//         // putc('\n', fp);
//     }
// }
int fileInDB(char* db, const char* todo) {
    FILE* fp;
    if ((fp = fopen("./db.txt", "a")) == NULL) {
        perror("Can't open db file");
        return 1;
    }
    if(fputs("* ", fp)==EOF){
        perror("Error writing to file");
        fclose(fp);
        return 1;

    }

    while (*todo != '\0') {
        if (putc((int)*todo, fp) == EOF) {
            perror("Error writing to file");
            fclose(fp);
            return 1;
        }
        todo++;
    }

    if (putc('\n', fp) == EOF) {
        perror("Error writing newline to file");
        fclose(fp);
        return 1;
    }

    if (fclose(fp) != 0) {
        perror("Error closing file");
    }
    todo = NULL;
    return 0;
}

void getBody(char* buffer, char* holder){
    char* body = strstr(buffer, "\r\n\r\n");
    if(body != NULL){
        body += 4;
        printf("body: %s\n", body);
    }
    char* bodyContent = strstr(body, "text=");
    if(bodyContent != NULL){
        bodyContent += 5;
        strcpy(holder, bodyContent);
        holder[strlen(bodyContent)] = '\0';
    }
}

void replace(char* str, char target, char replacement){
    for(; *str != '\0'; ++str){
        if(*str == target){
            *str = replacement;
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
    "Content-Type: text/html\n"
    "Connection: Closed\n\n"
    "%s", datetime_buffer, html);
    int newSocket;
    while(1){
        printf("====== WAITING ======\n");
        newSocket = accept(Server->socket, (struct sockaddr *)&Server->address, (socklen_t *)&addressLength);
        read(newSocket, buffer, 30000);
        printf("%s\n", buffer);
        char method[3];
        strncpy(method, buffer, 3);
        method[3] = '\0';
        int todoCounter = 0;
        if(strcmp(method, "POS")==0 && ++todoCounter <= 100){
            char todo[1000];
            getBody(buffer, todo);
            replace(todo, '+', ' ');
            if(fileInDB("db.txt", todo)==0){
                printf("succussfully added todo");
            }else{
                printf("failed to add todo");
            }
        }

        write(newSocket, response, strlen(response));
        if(todoCounter == 100){
            char fullResponse[1000];
            sprintf(fullResponse, "HTTP/1.1 200 OK\n"
            "Date: %s\n"
            "Server: Desperado/1.0.0 (Win64)\n"
            "Content-Type: text/plain\n"
            "Connection: Closed\n\n"
            "Todo list is full. Cannot add more items.", datetime_buffer);
            write(newSocket, fullResponse, strlen(fullResponse)); 
        }
        close(newSocket);
    }
    
}

int main(){
    Server serverr = server_constructor(AF_INET, SOCK_STREAM, 0, INADDR_ANY, 80, 10, run);
    serverr.run(&serverr);
}
