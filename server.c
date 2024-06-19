#include "server.h"
#include <stdio.h>
#include <stdlib.h>

Server server_constructor(int domain, int service, int protocol, unsigned long interface, int port, int backlog, void (*run)(Server *Server)){
    Server serv;
    serv.domain = domain;
    serv.service = service;
    serv.protocol = protocol;
    serv.interface = interface;
    serv.port = port;
    serv.backlog = backlog;
    serv.run = run;
    serv.address.sin_family = domain;
    serv.address.sin_port = htons(port); // convert port from int to a byte network port
    serv.address.sin_addr.s_addr = htonl(interface); // same as previous
    serv.socket = socket(domain, service, protocol);

    if(serv.socket == -1){
        perror("failed to connect to socket!\n");
        exit(1);
    }


    if(bind(serv.socket, (struct sockaddr *)&serv.address, sizeof(serv.address))==-1){
        perror("failed to bind socket!\n");
        exit(1);
    }

    if(listen(serv.socket, serv.backlog)==-1){
        perror("failed to start listening!\n");
        exit(1);
    }

    return serv;
    
}