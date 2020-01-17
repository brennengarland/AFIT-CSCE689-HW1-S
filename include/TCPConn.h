#ifndef TCPCONN_H
#define TCPCONN_H

#pragma once

#include <string>
#include <map>
#include <vector>
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

   bool handleConnection();
   void startAuthentication();
   void getUsername();
   void getPasswd();
   void sendMenu();
   void getMenuChoice();
   void setPassword();
   void changePassword();
   char* getResponse(const char* msg);
   
   bool getUserInput(std::string &cmd);
   std::vector<std::string> parseCmd(const std::string msg);

   void disconnect();
   bool isConnected();

   // unsigned long getIPAddr() { return _connfd.getIPAddr(); };

private:


   enum statustype { s_username, s_changepwd, s_confirmpwd, s_passwd, s_menu };

   enum cmd_type {unkown, greeting, opt1, opt2, opt3, opt4, opt5, password, exit, menu, First = unkown};

   statustype _status = s_username;

   // SocketFD _connfd;
 
   std::string _username; // The username this connection is associated with

   std::string _inputbuf;

   std::string _newpwd; // Used to store user input for changing passwords
   std::map<std::string, std::string> menu_def = 
   {
      {"hello",   "Receive a greeting"},
      {"1",       "Press to find out!"},
      {"2",       "Similar to 1 but not really"},
      {"3",       "You're probably seeing a trend"},
      {"4",       "Fooled you"},
      {"5",       "But not really"},
      {"passwd",  "Change your password"},
      {"exit",    "Disconnect"},
      {"menu",    "Display list of commands"}
      };

   std::map<std::string, cmd_type> cmd_table
   {
      {"hello",   greeting},
      {"1",       opt1},
      {"2",       opt2},
      {"3",       opt3},
      {"4",       opt4},
      {"5",       opt5},
      {"passwd",  password},
      {"exit",    exit},
      {"menu",    menu}
   };

   int _pwd_attempts = 0;

   int my_sock = 0;

   std::vector<std::string> cat_facts = 
   {"Cats make about 100 different sounds. Dogs make only about 10.\n",
    "Every year, nearly four million cats are eaten in Asia.\n",
    "Approximately 24 cat skins can make a coat.\n",
    "A cat usually has about 12 whiskers on each side of its face.\n"
   };

   std::vector<std::string> dog_facts =  {

   };

   std::vector<std::string> elephant_facts =  {

   };

   bool cats, dogs, elephants;

};


#endif
