#ifndef TCPCONN_H
#define TCPCONN_H

#pragma once

#include <string>
#include <map>
// #include "FileDesc.h"

// const int max_attempts = 2;

class TCPConn 
{
public:
   TCPConn();
   ~TCPConn();

   bool accept_conn(int server);

   int sendText(const char *msg);
   int sendText(const char *msg, int size);
   int getSocket();

   void handleConnection();
   void startAuthentication();
   void getUsername();
   void getPasswd();
   void sendMenu();
   void getMenuChoice();
   void setPassword();
   void changePassword();
   
   bool getUserInput(std::string &cmd);

   void disconnect();
   bool isConnected();

   // unsigned long getIPAddr() { return _connfd.getIPAddr(); };

private:


   enum statustype { s_username, s_changepwd, s_confirmpwd, s_passwd, s_menu };

   // statustype _status = s_username;

   // SocketFD _connfd;
 
   std::string _username; // The username this connection is associated with

   std::string _inputbuf;

   std::string _newpwd; // Used to store user input for changing passwords
   std::map<std::string, std::string> menu = 
   {
      {"hello", "Receive a greeting"},
      {"1", ""},
      {"2", ""},
      {"3", ""},
      {"4", ""},
      {"5", ""},
      {"passwd", "Change your password"},
      {"exit", "Disconnect"},
      {"menu", "Display list of commands"}
      };

   int _pwd_attempts = 0;

   int my_sock = 0;

};


#endif
