#ifndef SENDERTEST
#define SENDERTEST
/////////////////////////////////////////////////////////////////////////////
// Sender.h   - Defines a  Sender instance for each peer(Client/Server)    //
// Ver 1.0                                                                 //
// Sai Krishna, CSE#687 - Object Oriented Design, Spring 2015              //
// CST 4-187, Syracuse University, 832 940-8083, sbandaru@syr.edu	       //
//-------------------------------------------------------------------------//
// Sai Krishna (c) copyright 2015                                          //
// All rights granted provided this copyright notice is retained           //
//-------------------------------------------------------------------------//
// Application: Sender for Project #3, Communication Channel               //
// Platform:    Dell 2720, Win Pro 8.1, Visual Studio 2013                 //
/////////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package provides sender operations listed below:
*  - can upload files to a server with known port
*  - can download files from a listening
*
* Public Interface:
* =================
*void fileUpload(Socket& , std::string , std::string )						Transfer file from sender to listner
*
*void fileDownload(Socket& , std::string , std::string )					Transfer file from listner of peer to this sender
*
*void uploadfileAck(Socket&, std::string, std::string,std::string)			To acknowledge the upload request at the client side
*
*void sendMsg(Socket&, std::string, std::string,std::string)				Send a message through socket 
*
*void searchText(Socket&, std::string, std::string, std::string)			For Searching text in the server req. Project4
*
*void searchFile(Socket&, std::string, std::string, std::string)			For Searching file in the server req. Project4
*
*void sendString(Socket&, std::string, std::string, std::string)			Send whole result of searching req Proj 4
*
*std::string hex_to_string(const std::string& input)						Hex to String conversion
*
*std::string string_to_hexSend(const std::string& input)					String to hex conversion
*
*  Required Files:
*  ---------------
*  Sender.cpp, Sender.h.cpp
*  Sockets.h, Sockets.cpp, AppHelpers.h, AppHelpers.cpp, Message.h, Message.cpp, Display.h and Display.cpp
*
*  Build Command:
*  --------------
*  devenv CppCli-WPF-App_original.sln /rebuild debug
*
*  Maintenance History:
*  --------------------
*  ver 1.0 : 06 Apr 15

*  - first release
*/

#include "../sockets/sockets.h"
#include "../ApplicationHelpers/AppHelpers.h"
#include "../XmlDocument/XmlDocument.h"
#include "../Message/Message.h"
#include "../FileMgr/FileMgr.h"
#include "../Project1/DataStore.h"
#include <iostream>
#include <string>
#include <sstream>
#include <vector>
/////////////////////////////////////////////////////////////////////////////
// Sender class - Sender module of Peer is defined in this class 

class Sender
{
public:
	Sender(int portI,std::string addressI):port(portI),address(addressI){};
	~Sender(){};
	void fileUpload(Socket& , std::string , std::string,std::string,std::string );					//send file to receiver
	void fileDownload(Socket& , std::string , std::string );										//send download request to server
	void uploadfileAck(Socket&, std::string, std::string,std::string);								//send acknowledgement message to clinet
	void sendMsg(Socket&, std::string, std::string,std::string);
	void searchText(Socket&, std::string, std::string, std::string);
	void searchFile(Socket&, std::string, std::string, std::string);
	void sendString(Socket&, std::string, std::string, std::string);
	std::string hex_to_string(const std::string& input);
	std::string string_to_hexSend(const std::string& input);
private:
	int port;										//port of the listner of the peer
	std::string address;							//address of the listner of the peer
};
#endif