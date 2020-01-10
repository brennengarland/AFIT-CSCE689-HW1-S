#include "TCPServer.h"
#include <sys/socket.h>
#include <stdexcept>
#include <netinet/in.h> 


TCPServer::TCPServer() {

}


TCPServer::~TCPServer() {

}

/**********************************************************************************************
 * bindSvr - Creates a network socket and sets it nonblocking so we can loop through looking for
 *           data. Then binds it to the ip address and port
 *
 *    Throws: socket_error for recoverable errors, runtime_error for unrecoverable types
 **********************************************************************************************/

void TCPServer::bindSvr(const char *ip_addr, short unsigned int port) 
{
    int server_sock;
    struct sockaddr_in address;

    if((server_sock = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        throw std::invalid_argument("runtime_error"); //TO-DO: Fix error handling
    }

    int opt = 1;
    if(setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        throw std::invalid_argument("runtime_error");
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( 8080 );

    if(bind(server_sock, (struct  sockaddr *)&address, sizeof(address))<0)
    {
        throw std::invalid_argument("runtime_error");
    }
    
   
}

/**********************************************************************************************
 * listenSvr - Performs a loop to look for connections and create TCPConn objects to handle
 *             them. Also loops through the list of connections and handles data received and
 *             sending of data. 
 *
 *    Throws: socket_error for recoverable errors, runtime_error for unrecoverable types
 **********************************************************************************************/

void TCPServer::listenSvr() {

}

/**********************************************************************************************
 * shutdown - Cleanly closes the socket FD.
 *
 *    Throws: socket_error for recoverable errors, runtime_error for unrecoverable types
 **********************************************************************************************/

void TCPServer::shutdown() {
}
