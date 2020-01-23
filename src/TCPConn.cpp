#include "TCPConn.h"
#include <sys/types.h>
#include <netinet/in.h> 
#include <sys/socket.h>
#include <iostream>
#include <string.h>
#include <sstream>
#include "exceptions.h"
#include <cstdlib>
#include <fstream>


/*
Author: Brennen Garland
Reference: https://beej.us/guide/bgnet/html, https://www.geeksforgeeks.org/socket-programming-in-cc-handling-multiple-clients-on-server-without-multi-threading/
*/

TCPConn::TCPConn() {}
   
bool TCPConn::accept_conn(int server)
{
    // std::cout << "Entered accept\n";
    // Random seed for our options
    srand(time(0));

    struct sockaddr_in client_addr;
    socklen_t addrsize = sizeof(client_addr);
    my_sock = accept(server, (struct sockaddr *) &client_addr, &addrsize );
    if(my_sock < 0){ return false;}

    sendMenu();
    return true;

}

int TCPConn::getSocket()
{
    return my_sock;
}

bool TCPConn::handleConnection()
{
    // std::cout << "Handling connection...\n";
    char msg[20];
    int msg_len;
    msg_len = recv(my_sock, msg, 20, 0);
    // If we received a 0, the connection closed
    if( msg_len == 0 ) 
    {
        // std::cout << "Client has disconnected automatically\n";
        disconnect();
        return false;
    } else if(msg_len < 0)
    {
        // Recoverable: not uncommon for it to have a receive error
        std::cout << "Recieve Error!";
        return false;
    }

    // Add a null operator to the end of the message
    msg[msg_len] = '\0';
    // std::cout << "Received Message: " << msg << "\n";
    // std::cout << "Bytes received: " << strlen(msg) << "\n";

    // Conver to the char array to string for security and ease of use
    std::string msg_str(msg, strlen(msg));
    // std::vector cmds = parseCmd(msg);
    cmd_type cmd = cmd_type::unkown;
    // Loop through our cmd_table to find which command the client sent
    for(auto const& [key, val] : cmd_table)
    {
        // std::cout << "Key: " << key << "\tMsg: " << msg_str << "\n";
    if(key == msg_str) 
    {
        //    std::cout << "Cmd = " << val << "\n";
        cmd = val;
        }
    }
    switch(cmd)
    {
        case cmd_type::greeting:
            sendText("WHAT IS UP, Welcome to the Server. I hope you will have as much fund as we do.\n");
            break;
        case cmd_type::menu:
            sendMenu();
            break;
        case cmd_type::exit:
            disconnect();
            break;
        case cmd_type::password:
            sendText("Changing Password...\n");
            changePassword();
            break;
        case cmd_type::opt1:
            cats = true;
            sendText("Wow! You have signed up for cat facts (provided by factretriever.com). You will receive a cat fact after each command!\nHere is your first cat fact! I hope youre excited, because we are!\n");
            break;
        case cmd_type::opt2:
            dogs = true;
            sendText("Wow! You have signed up for dog facts (provided by factretriever.com). You will receive a dog fact after each command!\nHere is your first dog fact! I hope you're excited, because we are!\n");
            break;
        case cmd_type::opt3:
            elephants = true;
            sendText("Wow! You have signed up for elephant facts (provided by factretriever.com). You will receive a elephant fact after each command!\nHere is your first elephant fact! I hope youre excited, because we are!\n");
            break;
        case cmd_type::opt4:
            cats = false;
            dogs = false;
            elephants = false;
            sendText("You've cancelled all of your facts!\n");
            break;
        case cmd_type::opt5:
            cats = true;
            dogs = true;
            elephants = true;
            sendText("All facts are back on!\n");
            break;
        default:
            sendText("That didn't seem to be on the Menu! Take a gander at what we sent earlier and get back to me.\n");
            break;
    }
    
    
    return true;

}

int TCPConn::sendText(const char *msg)
{
    // std::cout << "Sending msg\n";

    int bytes_sent = 0, msg_len = 1, total_sent = 0;
    // Make a strstream so it's easier to append facts
    std::stringstream sendmsg;
    sendmsg << msg;
    // Check if the facts should be included
    if(cats) { sendmsg << "Cat Fact: " << cat_facts.at(rand() % std::size(cat_facts)) << "\n";}
    if(dogs) {sendmsg << "Dog Fact: " << dog_facts.at(rand() % std::size(dog_facts))  << "\n";}
    if(elephants) { sendmsg << "Elephant Fact: " <<  elephant_facts.at(rand() % std::size(elephant_facts)) << "\n"; }
    msg_len = strlen(sendmsg.str().c_str());
    while(total_sent < msg_len)
    {
        // Keeps sending the string until the whole message arrives
        bytes_sent = send(my_sock, sendmsg.str().substr(bytes_sent, msg_len - total_sent).c_str(), msg_len - total_sent, 0);
        if(bytes_sent == -1)
        {
            std::cout << "Send Error!\n";
            return -1;    
        }

        total_sent += bytes_sent;
    }
    std::cout << "Sent: " << total_sent << " bytes" << "\t Msg Len: " << msg_len << "\n";
    // std::cout << "Message Sent: " << sendmsg.str();
    return bytes_sent;
}

void TCPConn::sendMenu()
{
    std::stringstream menu_str;
    menu_str << "COMMAND MENU\n";
    for(auto const& [key, val] : menu_def)
    {
        menu_str << key << ":\t" << val << "\n";
    }

    sendText(menu_str.str().c_str());
}

void TCPConn::disconnect()
{
    shutdown(my_sock,2);
}

void TCPConn::changePassword()
{
    std::cout << "Changing Password...\n";
}

std::vector<std::string> TCPConn::parseCmd(const std::string msg)
{
    std::vector<std::string> cmds;
    std::stringstream cmd;
    // std::cout << "Character sequence start: \n";
    for(auto charc : msg)
    {
        // std::cout << charc << "\n";
        if(charc == '\n')
        {
            cmds.push_back(cmd.str());
            cmd.str("");
            cmd.clear();
        }
        cmd << charc;
    }
    // std::cout << "Commands: \n";
    // for(auto command : cmds)
    // {
    //     std::cout << command << "\n";
    // }

    return cmds;
}
