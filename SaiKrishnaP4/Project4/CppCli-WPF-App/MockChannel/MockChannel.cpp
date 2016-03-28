/////////////////////////////////////////////////////////////////////////////
// MockChannel.cpp - channel for UI and Native client		               //
// - MockChannel reads from sendQ and writes to recvQ                      //
// Ver 1.0                                                                 //
// Sai Krishna, CSE#687 - Object Oriented Design, Spring 2015              //
// CST 4-187, Syracuse University, 832 940-8083, sbandaru@syr.edu	       //
//-------------------------------------------------------------------------//
// Sai Krishna (c) copyright 2015                                          //
// All rights granted provided this copyright notice is retained           //
//-------------------------------------------------------------------------//
// Application: Channel for Project #4, WPF Application			           //
// Platform:    Dell 2720, Win Pro 8.1, Visual Studio 2013                 //
/////////////////////////////////////////////////////////////////////////////

#define IN_DLL
#include "MockChannel.h"
#include "Cpp11-BlockingQueue.h"
#include "../XmlDocument/XmlDocument.h"
#include <string>
#include <thread>
#include <iostream>
#include "../Client/Client.h"

using BQueue = BlockingQueue < Message1 >;

/////////////////////////////////////////////////////////////////////////////
// Sendr class
// - accepts messages from client for consumption by MockChannel
//
class Sendr : public ISendr
{
public:
  void postMessage(const Message1& msg);
  BQueue& queue();
private:
  BQueue sendQ_;
};

//----< enqueue msg to sender >--------------------------------------------------

void Sendr::postMessage(const Message1& msg)
{
  sendQ_.enQ(msg);
}

//----< extract sender queue >--------------------------------------------------

BQueue& Sendr::queue() { return sendQ_; }

/////////////////////////////////////////////////////////////////////////////
// Recvr class
// - accepts messages from MockChanel for consumption by client
//
class Recvr : public IRecvr
{
public:
  Message1 getMessage();
  BQueue& queue();
private:
  BQueue recvQ_;
};

//----< dequeue msg from receiver >--------------------------------------------------

Message1 Recvr::getMessage()
{
  return recvQ_.deQ();
}

//----< retrive receiver queue >--------------------------------------------------

BQueue& Recvr::queue()
{
  return recvQ_;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
// ClientUI class - implements IClient interface that acts as channel between UI and Native code

class ClientUI:public IClient
{
public:
	ClientUI(ISendr* pSendr, IRecvr* pRecvr);
	void startC(std::string,std::string);
	void stopC();
	void startClient();
	void searchText(std::string , IRecvr* );
	XmlProcessing::XmlDocument BuildTree(bool,std::string);
	void handle(std::string, IRecvr*);
	std::string Raddress();
	std::vector<std::string> XmlParse(std::string);
	void Upload(std::string msg, IRecvr* pRecvr);
	void Download(std::string msg, IRecvr* pRecvr);
	void SerachText(std::string, IRecvr*);
	void SerachFile(std::string, IRecvr*);
	std::string string_to_hex(const std::string& );
	std::string hex_to_stringR(const std::string& input);
private:
	bool connection;
	std::thread thread_;
	std::string address;
	std::string serveraddress;
	std::vector<std::string> pattern;
	Client *client;
	ISendr* pISendr_;
	IRecvr* pIRecvr_;
	bool stop_ = false;
};

//----< pass pointers to Sender and Receiver >-------------------------------

ClientUI::ClientUI(ISendr* pSendr, IRecvr* pRecvr) : pISendr_(pSendr), pIRecvr_(pRecvr) { connection = false; }

//----< starts the channel client that intern starts our client at backend >-------

void ClientUI::startClient()
{
	client->start();
}

//----< Connect operation for the UI and Native client >-------

void ClientUI::handle(std::string msg, IRecvr* pRecvr)
{
	BlockingQueue<std::string> &q1 = QServer<std::string, 1>().get();
	BQueue& recvQ = ((Recvr*)pRecvr)->queue();
	bool conn_flag = false;
	if (msg.substr(0, msg.find(" ")) == "CONNECT")conn_flag = true;
	client->parse(msg);
	msg = q1.deQ();
	if (conn_flag == true && msg.substr(0, 3) == "Fai")
	{
		connection = false; serveraddress = "";
	}
	recvQ.enQ(msg);
}

//----< Upload decode and encode operation for the UI and Native client >-------

void ClientUI::Upload(std::string msg, IRecvr* pRecvr)
{
	msg = msg.substr(msg.find(" "));
	std::string number_s = msg.substr(1, 1);
	int number = std::stoi(number_s);
	msg = msg.substr(msg.find(" ")+1);
	msg = msg.substr(msg.find(" ")+1);
	std::vector<std::string> serveradd;
	BlockingQueue<std::string> &q1 = QServer<std::string, 1>().get();
	while (number)
	{
		serveradd.push_back(msg.substr(0, msg.find(" ")));
		//msg = msg.substr(msg.find(" ") + 1);
		msg = msg.substr(msg.find(" ")+1); number--;
	}
	bool serverfail = false; std::string message = "", file = msg;//.substr(msg.find(" "));
	for (auto itr : serveradd){
		serverfail = false;
		msg = "CONNECT " + itr + " NOFILE";
		client->parse(msg);
		msg = q1.deQ();
		if (msg.substr(0, 3) == "Fai"){
			message += "\n Failed to connect to " + itr; serverfail = true;
		}
		if (serverfail == false)
		{
			msg = "UPLOAD " + itr + " " + file;
			client->parse(msg);
			message += "\n"+q1.deQ();
		}
	}
	
	BQueue& recvQ = ((Recvr*)pRecvr)->queue();
	recvQ.enQ(message);
}

//----< Download decode and encode operation for the UI and Native client >-------

void ClientUI::Download(std::string msg, IRecvr* pRecvr)
{
	msg = msg.substr(msg.find(" "));
	std::string number_s = msg.substr(1, 1);
	int number = std::stoi(number_s);
	msg = msg.substr(msg.find(" ") + 1);
	msg = msg.substr(msg.find(" ") + 1);
	std::vector<std::string> serveradd;
	BlockingQueue<std::string> &q1 = QServer<std::string, 1>().get();
	while (number)
	{
		serveradd.push_back(msg.substr(0, msg.find(" ")));
		//msg = msg.substr(msg.find(" ") + 1);
		msg = msg.substr(msg.find(" ") + 1); number--;
	}
	bool serverfail = false; std::string message = "", file = msg;//.substr(msg.find(" "));
	for (auto itr : serveradd){
		serverfail = false;
		msg = "CONNECT " + itr + " NOFILE";
		client->parse(msg);
		msg = q1.deQ();
		if (msg.substr(0, 3) == "Fai"){
			message += "\n Failed to connect to " + itr; serverfail = true;
		}
		if (serverfail == false)
		{
			msg = "DOWNLOAD " + itr + " " + file;
			client->parse(msg);
			message +="\n"+ q1.deQ();
		}
	}

	BQueue& recvQ = ((Recvr*)pRecvr)->queue();
	recvQ.enQ(message);
}

//----< Serach decode and encode operation for the UI and Native client >-------

void ClientUI::searchText(std::string msg, IRecvr* pRecvr)
{

	std::string element = msg.substr(msg.find(" ") + 1);
	XmlProcessing::XmlDocument XML = this->BuildTree(true, element);
	msg = msg.substr(0, msg.find(element));
	client->parse("TEXTSEARCH " + msg + " " + XML.display());
	BlockingQueue<std::string> &q1 = QServer<std::string, 1>().get();
	msg = q1.deQ();
	std::vector<std::string> files = this->XmlParse(msg);
	BQueue& recvQ = ((Recvr*)pRecvr)->queue();
	for (auto itr : files){ recvQ.enQ(itr); }
	recvQ.enQ("END");
}

//----< Search text decode and encode operation for the UI and Native client >-------

void ClientUI::SerachText(std::string msg, IRecvr* pRecvr)
{
	msg = msg.substr(msg.find(" "));
	std::string number_s = msg.substr(1, 1);
	int number = std::stoi(number_s);
	msg = msg.substr(msg.find(" ") + 1);
	msg = msg.substr(msg.find(" ") + 1);
	std::vector<std::string> serveradd;
	BlockingQueue<std::string> &q1 = QServer<std::string, 1>().get();
	while (number)
	{
		serveradd.push_back(msg.substr(0, msg.find(" ")));
		//msg = msg.substr(msg.find(" ") + 1);
		msg = msg.substr(msg.find(" ") + 1); number--;
	}
	bool serverfail = false; std::string message = "", searchtext1 = msg;//.substr(msg.find(" "));
	BQueue& recvQ = ((Recvr*)pRecvr)->queue();
	for (auto itr : serveradd){
		serverfail = false;
		msg = "CONNECT " + itr + " NOFILE";
		client->parse(msg);
		msg = q1.deQ();
		if (msg.substr(0, 3) == "Fai"){
			message += "\n Failed to connect to " + itr; serverfail = true;
		}
		if (serverfail == false)
		{
			XmlProcessing::XmlDocument XML = this->BuildTree(true, searchtext1);
			client->parse("TEXTSEARCH " + itr + " " + XML.display());
			BlockingQueue<std::string> &q1 = QServer<std::string, 1>().get();
			msg = q1.deQ();
			std::vector<std::string> files = this->XmlParse(msg);
			for (auto itr2 : files){ recvQ.enQ(itr + "::" + itr2); }
			message += std::string("\n" + std::string("Search result of files that match \"") + searchtext1 + "\" text in" + itr + " address");
		}
	}recvQ.enQ("END");	recvQ.enQ(message); pattern.clear();
}

//----< Search file decode and encode operation for the UI and Native client >-------

void ClientUI::SerachFile(std::string msg, IRecvr* pRecvr)
{
	msg = msg.substr(msg.find(" "));
	std::string number_s = msg.substr(1, 1);
	int number = std::stoi(number_s);
	msg = msg.substr(msg.find(" ") + 1);
	msg = msg.substr(msg.find(" ") + 1);
	std::vector<std::string> serveradd;
	BlockingQueue<std::string> &q1 = QServer<std::string, 1>().get();
	while (number)
	{
		serveradd.push_back(msg.substr(0, msg.find(" ")));
		//msg = msg.substr(msg.find(" ") + 1);
		msg = msg.substr(msg.find(" ") + 1); number--;
	}
	bool serverfail = false; std::string message = "", folder = msg;//.substr(msg.find(" "));
	BQueue& recvQ = ((Recvr*)pRecvr)->queue();
	for (auto itr : serveradd){
		serverfail = false;
		msg = "CONNECT " + itr + " NOFILE";
		client->parse(msg);
		msg = q1.deQ();
		if (msg.substr(0, 3) == "Fai"){
			message += "\n Failed to connect to " + itr; serverfail = true;
		}
		if (serverfail == false)
		{
			XmlProcessing::XmlDocument XML = this->BuildTree(false, folder);
			client->parse("FILESEARCH " + itr + " " + XML.display());
			BlockingQueue<std::string> &q1 = QServer<std::string, 1>().get();
			msg = q1.deQ();
			std::vector<std::string> files = this->XmlParse(msg);
			for (auto itr2 : files){ recvQ.enQ(itr + "::" + itr2); }
			message += std::string("\n" +std::string("Search result of files found in ")+ folder + " directory of " + itr + " address");
		}
	}recvQ.enQ("END"); recvQ.enQ(message); pattern.clear();
}

//----< Encoading from string to hex for using data in XMLDoc >-------

std::string ClientUI::string_to_hex(const std::string& input)
{
	static const char* const lut = "0123456789ABCDEF";
	size_t len = input.length();

	std::string output;
	output.reserve(2 * len);
	for (size_t i = 0; i < len; ++i)
	{
		const unsigned char c = input[i];
		output.push_back(lut[c >> 4]);
		output.push_back(lut[c & 15]);
	}
	return output;
}

//----< Encoading from string to hex for using data in XMLDoc >-------

XmlProcessing::XmlDocument  ClientUI::BuildTree(bool Text_File,std::string msg)	//   text true/file false
{
	XmlProcessing::XmlDocument xmldoc; std::string element;
	if (Text_File)element = "TEXT"; else element = "DIRECTORY";
	std::string prereq = "<ROOT><PATTERN></PATTERN><"  + element +  "></" + element + '>' + "</ROOT>";
	std::vector<std::string> parsed=xmldoc.parse(prereq, false);
	xmldoc.BuildTree(parsed);
	if (msg.size())
		xmldoc.addChildDoc(element, "<" + string_to_hex(msg) + ">");// string_to_hex(msg));
	for (auto itr : pattern)
		xmldoc.addChildDoc("PATTERN", "<" + string_to_hex(itr) + ">");// string_to_hex(itr));
	return xmldoc;
}

//----< Decoading from hex to string for using data in XMLDoc >-------

std::string ClientUI::hex_to_stringR(const std::string& input)
{
	static const char* const lut = "0123456789ABCDEF";
	size_t len = input.length();
	if (len & 1) throw std::invalid_argument("odd length");

	std::string output;
	output.reserve(len / 2);
	for (size_t i = 0; i < len; i += 2)
	{
		char a = input[i];
		const char* p = std::lower_bound(lut, lut + 16, a);
		if (*p != a) throw std::invalid_argument("not a hex digit");

		char b = input[i + 1];
		const char* q = std::lower_bound(lut, lut + 16, b);
		if (*q != b) throw std::invalid_argument("not a hex digit");

		output.push_back(((p - lut) << 4) | (q - lut));
	}
	return output;
}

//----< Parse patterns and text/file to build XMLDoc >-------

std::vector<std::string> ClientUI::XmlParse(std::string input)
{
	XmlProcessing::XmlDocument XML; std::vector<std::string>parsed = XML.parse(input, false);
	XML.BuildTree(parsed);
	std::vector<std::string> files = XML.Values(XML.elements("FILES").allChildren().select());
	std::vector<std::string>::iterator iter;
	for (iter = files.begin(); iter != files.end(); iter++)
	{
		*iter = hex_to_stringR(*iter);
		//std::cout << *iter;
	}
	return files;
}

//----< Return address of client >-------

std::string ClientUI::Raddress()
{
	return this->address;
}

//----< Start Native Client >-------

void ClientUI::startC(std::string address, std::string path)
{
	this->address = address; client = new Client(address, path); std::cout << "\n  Client starting up";
	thread_ = std::thread(
		[this] {
		Sendr* pSendr = dynamic_cast<Sendr*>(pISendr_);	Recvr* pRecvr = dynamic_cast<Recvr*>(pIRecvr_);	this->startClient();
		if (pSendr == nullptr || pRecvr == nullptr)	{
			std::cout << "\n  failed to start Client Channel\n\n";return;
		}
		BQueue& sendQ = pSendr->queue();
		BQueue& recvQ = pRecvr->queue();
		while (!stop_){
			std::cout << "\n  channel deQing message";
			Message1 msg = sendQ.deQ();  // will block here so send quit message when stopping
			if (msg.substr(0, msg.find(" ")) == "CONNECT" && connection == false){
				msg = msg + " NOFILE"; connection = true;  
				handle(msg, pRecvr);
			}
			else if (msg.substr(0, msg.find(" ")) == "DISCONNECT" && connection == true)
			{msg = msg + " NOFILE"; connection = false; handle(msg, pRecvr);}
			else if (msg.substr(0, msg.find(" ")) == "UPLOAD")
			{msg = msg; 	 Upload(msg, pRecvr);}
			else if (msg.substr(0, msg.find(" ")) == "DOWNLOAD" )
			{msg = msg;  Download(msg, pRecvr);}
			else if (msg.substr(0, msg.find(" ")) == "ADDPATTERN")
			{msg = msg.substr(msg.find(" ") + 1);  pattern.push_back(msg); recvQ.enQ("Pattern \""+msg+"\" added successfully!");}
			else if (msg.substr(0, msg.find(" ")) == "SEARCHTEXT")
			{SerachText(msg, pRecvr);}
			else if (msg.substr(0, msg.find(" ")) == "SEARCHFILE")
			{SerachFile(msg, pRecvr);}
			else
			{	BlockingQueue<std::string> &q1 = QServer<std::string, 1>().get();BQueue& recvQ = pRecvr->queue();////if (connection == false)recvQ.enQ("Server hasn't been made");//else 
				recvQ.enQ("Flow Problem!!!");
			}
		}
		std::cout << "\n  Server stopping\n\n";
	});
}

//----<  Stop Native Client  >---------------------------------------
void ClientUI::stopC() { stop_ = true; }

//----< factory functions for sender >--------------------------------------------------

ISendr* ObjectFactory::createSendr() { return new Sendr; }

//----< factory functions for receiver >--------------------------------------------------

IRecvr* ObjectFactory::createRecvr() { return new Recvr; }

//----< factory functions for clieny >--------------------------------------------------

IClient* ObjectFactory::createClient(ISendr* pISendr, IRecvr* pIRecvr) 
{ 
	return new ClientUI(pISendr, pIRecvr); 
}

#ifdef TEST_MOCKCHANNEL

//----< test stub >----------------------------------------------------------

int main()
{
  ObjectFactory objFact;
  ISendr* pSendr = objFact.createSendr();
  IRecvr* pRecvr = objFact.createRecvr();
  IClient* client = objFact.createClient(pSendr, pRecvr);
  client->startC();
  client->stop();
  std::cin.get();
}
#endif
