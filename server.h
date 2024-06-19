#ifndef server_h
#define server_h

#include <sys/socket.h>
#include <netinet/in.h>

typedef struct Server {
    int domain; // domain of the ip address (ipv4, ipv6)
    int service; // the service that is going to be used by the server, for example the value might by SOCK_STREAM indicating TCP-based service
    int protocol;
    unsigned long interface; // represent the network interface or the ip address that the server will listen to
    int port;
    int backlog;

    int socket;

    struct sockaddr_in address; // accept ipv4 address
    void (* run) (struct Server *Server); // function pointer

} Server;

Server server_constructor(int domain, int service, int protocol, unsigned long interface, int port, int backlog, void (* run)(Server *Server)); // a function prototype that returns a Server struct

#endif