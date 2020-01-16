#include "TCPClient.h"
#include "exceptions.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <netdb.h> 
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>


/**********************************************************************************************
 * TCPClient (constructor) - Creates a Stdin file descriptor to simplify handling of user input. 
 *
 **********************************************************************************************/

TCPClient::TCPClient() {
    std::cout << "Making Client\n";
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
}

/**********************************************************************************************
 * TCPClient (destructor) - No cleanup right now
 *
 **********************************************************************************************/

TCPClient::~TCPClient() {

}

/**********************************************************************************************
 * connectTo - Opens a File Descriptor socket to the IP address and port given in the
 *             parameters using a TCP connection.
 *
 *    Throws: socket_error exception if failed. socket_error is a child class of runtime_error
 **********************************************************************************************/

void TCPClient::connectTo(const char *ip_addr, unsigned short port) 
{
    std::cout << "Preparing Connection\n";
    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(ip_addr);
    address.sin_port = htons( port );
    std::cout << "Client: Connecting to server...\n";
    if(connect(sock_fd, (struct  sockaddr *)&address, sizeof(address)) < 0)
    {
        shutdown(sock_fd, 2); 
        throw socket_error("Connection error");
    }
}

/**********************************************************************************************
 * handleConnection - Performs a loop that checks if the connection is still open, then 
 *                    looks for user input and sends it if available. Finally, looks for data
 *                    on the socket and sends it.
 * 
 *    Throws: socket_error for recoverable errors, runtime_error for unrecoverable types
 **********************************************************************************************/

void TCPClient::handleConnection() {
    std::cout << "Handling connection\n";
    char msg[300];
    int rec_len;
    bool conn = true;
    while(conn)
    {
        if((rec_len = recv(sock_fd, msg, 300, 0)) <= 0) {throw socket_error("Could not receive");}
        
        msg[rec_len] = '\0';
        std::cout << "Received: " << rec_len << " bytes\n";
        std::cout << "Message:\n";
        std::cout << msg << "\n";

        std::cout << "Send Command: ";

        std::string cmd;
        std::cin >> cmd;

        if(cmd == "exit")
        {
            closeConn();
            conn = false;    
        }
        else
        {   
            int bytes_sent;

            bytes_sent = send(sock_fd, cmd.c_str(), strlen(cmd.c_str()), 0);
            std::cout << "Sent: " << bytes_sent << " bytes\n";
        }

    }

}

/**********************************************************************************************
 * closeConnection - Your comments here
 *
 *    Throws: socket_error for recoverable errors, runtime_error for unrecoverable types
 **********************************************************************************************/

void TCPClient::closeConn() {
}


