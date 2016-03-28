/////////////////////////////////////////////////////////////////////////////
// Client.cpp   - Defines a  Client instance						       //
// Ver 1.0                                                                 //
// Sai Krishna, CSE#687 - Object Oriented Design, Spring 2015              //
// CST 4-187, Syracuse University, 832 940-8083, sbandaru@syr.edu	       //
//-------------------------------------------------------------------------//
// Sai Krishna (c) copyright 2015                                          //
// All rights granted provided this copyright notice is retained           //
//-------------------------------------------------------------------------//
// Application: Client for Project #3, Communication Channel               //
// Platform:    Dell 2720, Win Pro 8.1, Visual Studio 2013                 //
/////////////////////////////////////////////////////////////////////////////

#include "../sockets/sockets.h"
#include "../ApplicationHelpers/AppHelpers.h"
#include "../Message/Message.h"
#include "../Display/Display.h"
#include "../MockChannel/MockChannel.h"
#include <cctype>
#include <iostream>
#include <fstream>
#include <sstream>
#include <thread>
#include "Client.h"

using namespace ApplicationHelpers;

//----< helps client's sender to connect to requested address and port >------------------------------------------------------

bool  Client::connectadd(SocketConnecter& si, std::string address, int port)
{
	int count = 0;
	while (!si.connect(address, port))
	{
		Display::displayString("client waiting to connect");
		::Sleep(100); count++;
		if (count == 2 || port==this->port)return false;
	}
	Display::displayString("Client connected to \"" + address + std::to_string(port) + "\"");
	return true;
}

//----< Parse the property file of the client for the client operations >------------------------------------------------------

void Client::parse(std::string line)
{
	std::string command, receiverI, filenameI;
	std::istringstream linestream(line);
	linestream >> command;
	linestream >> receiverI;
	linestream >> filenameI; Message msg;
	filenameI = line.substr(line.find(filenameI), line.length());

	senderQ1.enQ(msg.makeMessage(command, this->address + std::to_string(this->port), receiverI, filenameI.size(), filenameI));
	
}

//----< Client sender is started here and goes through the series of operations given in property file >------------------------------------------------------

void Client::sender()
{
	try	{
		Verbose v(true);SocketSystem ss;SocketConnecter si;Message msg,msgQparse;
		std::string popQ, addressR, filename, commandI, senderI, receiverI, bodyI; int portR = 0, pastportR = 0, contentLI;		//not using bodyI
		Sender CT(this->port, this->address);BlockingQueue<std::string> &q1 = QServer<std::string, 1>().get();
		bool first = false;
		while (true){
			popQ = this->senderQ1.deQ();pastportR = 0000;
			msgQparse.getMessage(&(*popQ.begin()), commandI, senderI, receiverI, contentLI, filename); 
			portR =std::stoi(receiverI.substr((receiverI.find(':') + 1), receiverI.size()));addressR = receiverI.substr(0,receiverI.find(':'));
			if (commandI == "CONNECT"){
				if (pastportR != portR){
					if (first){	CT.sendMsg(si, "DISCONNECT", receiverI,"");si.shutDownSend();
					}
					if (connectadd(si, addressR, portR)){ q1.enQ("Connected to " + receiverI); pastportR = portR; first = true; }
					else q1.enQ("Failed to connect to " + receiverI);//si.shutDownSend();
				}
				else q1.enQ("Already connected to" + receiverI);
			}
			//else if (commandI == "DISCONNECT"){
			//	first = false;	pastportR = 0000;CT.sendMsg(si, "DISCONNECT", receiverI,""); q1.enQ("Disconnected from " + receiverI);
			//	si.shutDownSend();
			//}
			else if (commandI == "UPLOAD"){	CT.fileUpload(si, filename, receiverI, commandI,this->path);}
			else if (commandI == "DOWNLOAD"){CT.fileDownload(si, filename, receiverI);}
			else if (commandI == "TEXTSEARCH"){	CT.sendMsg(si, commandI, receiverI, filename);}
			else if (commandI == "FILESEARCH"){	CT.sendMsg(si, commandI, receiverI, filename);}
			//else if (commandI == "TERMINATE_SERVER"){
			//	char *C = msg.makeMessage("TERMINATE", this->address + ":" + std::to_string(this->port), addressR, 0, "");
			//	si.send(msg.getMessageSize(), C);
			//}
			else{
				char *C = msg.makeMessage("ENDTHREAD", this->address + ":" + std::to_string(this->port), addressR, 0, "");
				si.send(msg.getMessageSize(), C);si.shutDownSend(); return;
			}
		}
	}
	catch (std::exception& ex){
		Display::displayString("  Exception caught:");
		Display::displayString(std::string("\n  ") + ex.what() + "\n\n");
	}
}

//----< Client receiver is started here and goes through the series of operations as the requests comes >------------------------------------------------------

void  Client::receiver()
{
	try
	{
		Verbose v(true);
		SocketSystem ss;
		SocketListener sl(port, Socket::IP6);
		Receiver cp(port, address,false,path);
		sl.start(cp);
		std::cout.flush();
		std::cin.get();
	}
	catch (std::exception& ex)
	{
		Display::displayString("  Exception caught:");
		Display::displayString(std::string("\n  ") + ex.what() + "\n\n");
	}
}

//----< for enQ the sender's queue >------------------------------------------------------

void Client::push(std::string input)
{
	this->senderQ1.enQ(input);
}

//----< starts sender and receiver modules for client>------------------------------------------------------

void Client::start()
{
	Display::displaytitle("Client \n\nAddress:\t\t" + this->peerAddress() + "\n\nLocated at path:\t" + this->returnpath());
	std::thread Receiver(&Client::receiver, this);
	std::thread Sender(&Client::sender, this);
	Sender.detach();
	Receiver.detach();
}



#ifdef TEST_CLIENT
//----< demonstration >------------------------------------------------------

int main(int argc, char* argv[])
{
	if (argc < 2){
		Display::displayString("Should have atleast address and filepath"); return 0;
	}
	else{
		Client p1(argv[1], argv[2]);
		Message msg; std::string filename;
		Display::displaytitle("Client \n\nAddress:\t\t" + p1.peerAddress() + "\n\nLocated at path:\t" + p1.returnpath());
		std::thread Receiver(&Client::receiver, p1);
		std::thread Sender(&Client::sender, p1);
		p1.parse("properties_Client.txt");
		Sender.join();
		Receiver.join();
	}
}
#endif