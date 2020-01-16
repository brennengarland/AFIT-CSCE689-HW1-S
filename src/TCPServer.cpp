#include "TCPServer.h"
#include <stdexcept>
#include <netinet/in.h> 
#include "exceptions.h"
#include <fcntl.h>
#include <sys/time.h>
#include "TCPConn.h"
#include <map>
#include <iostream>
#include <sstream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h> 
#include <arpa/inet.h>


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
    
    struct sockaddr_in address;
    // Create socket to listen for incoming connections
    if((listener_sock = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        throw socket_error("Error: Failed to create socket");
    }

    // Set listener to Non-Blocking
    fcntl(listener_sock, F_SETFL, O_NONBLOCK);

    int opt = 1;
    if(setsockopt(listener_sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
    {
        throw std::runtime_error("Error: Failed to set socket options");
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( port );
    // Bind listener to port
    if(bind(listener_sock, (struct  sockaddr *)&address, sizeof(address)) < 0)
    {
        std::stringstream error_out;
        error_out << "Address: " << address.sin_addr.s_addr;

        throw socket_error(error_out.str());
    }
    
   
}

/**********************************************************************************************
 * listenSvr - Performs a loop to look for connections and create TCPConn objects to handle
 *             them. Also loops through the list of connections and handles data received and
 *             sending of data. 
 *
 *    Throws: socket_error for recoverable errors, runtime_error for unrecoverable types
 **********************************************************************************************/

void TCPServer::listenSvr() 
{
    std::cout << "Entered object\n";
    fd_set all_sock, read_sock;
    int max_sock, new_sock;
    std::map<int, TCPConn*> connections;

    // Start listening on one socket
    if(listen(listener_sock, 3) < 0)
    {
        throw socket_error("Listen Error");
    }

    std::cout << "Started Listening..\n";

    // Add this listener socket to our master list
    FD_SET(listener_sock, &all_sock);
    max_sock = listener_sock;

    while(true)
    {
        // Read sock is modified by select so we must reset it each time
        read_sock = all_sock;
        std::cout << "Server: Looping\n";

        if(select(max_sock+1, &read_sock, NULL, NULL, NULL) < 0)
        {
            throw socket_error("Select error");
        }

        // Loop through all of our sockets and check which ones have data
        for(int i=0; i <= max_sock; i++)
        {
            if(FD_ISSET(i, &read_sock))
            {
                // New connection because the listener socket has data
                if(i == listener_sock)
                {
                    std::cout << "Server: New Connection!\n";
                    TCPConn* new_conn = new TCPConn();
                    if(new_conn->accept_conn(listener_sock) == false)
                    {
                        throw socket_error("Error: Accepting new Connection");
                    }

                    new_sock = new_conn->getSocket();

                    if(new_sock > max_sock) {max_sock = new_sock;}

                    connections.insert({new_sock, new_conn});
                }
                // Data from an existing connection
                else
                {
                    std::cout << "Existing Connection\n";
                    for(auto const& [key, val] : connections)
                    {
                        if(key == i)
                        {
                            val->handleConnection();
                        }
                    }
                }
            }
        }

    }

}

/**********************************************************************************************
 * shutdown - Cleanly closes the socket FD.
 *
 *    Throws: socket_error for recoverable errors, runtime_error for unrecoverable types
 **********************************************************************************************/

void TCPServer::shutdown() {
}
