/////////////////////////////////////////////////////////////////////////////
// Sender.cpp - Defines the sender instance for each peer	               //
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

#include "../Display/Display.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include "Sender.h"


using namespace ApplicationHelpers;

//----< test stub starts here >----------------------------------------------

struct Cosmetic1
{
	~Cosmetic1()
	{
		Display::displayString("  press key to exit : ");
		std::cin.get();
		std::cout << "\n\n";
	}
} aGlobalForCosmeticAction;

//----< To transfer file from sender to listner >--------------------------------------------------------------------------------

void Sender::fileUpload(Socket& si, std::string filename,std::string receiverAddr,std::string commandp,std::string path)
{
	std::string filename1 = filename.substr(0, filename.find_last_of(" ")), path2 = filename.substr(filename.find_last_of(" ")+1),filename2;
	filename2 = path2 + "\\" + filename1.substr(filename1.find_last_of("\\") + 1); std::string uploadpath="";
	std::string sendfilename, openfile;
	if (commandp == "DOWNLOAD"){
		uploadpath = path;
		openfile = filename2; sendfilename = filename1;
	}
	else{ openfile = filename1; sendfilename = filename2; }
	Message block; std::ifstream myfile(uploadpath + openfile, std::ios::in | std::ios::binary); char* c;
	if (!myfile.good()){
		
		if (commandp == "DOWNLOAD"){				//Request from server to upload
			c = block.makeMessage("NOFILE", this->address + ":" + std::to_string(this->port), receiverAddr, filename1.size(), filename1);
			si.send(block.getMessageSize(), c);
		}
		else
			Verbose::show("Doesn't have file with " + this->address + ":" + std::to_string(this->port));
		return;
	}
	size_t sizebolock; char  buf[1025]; bool endoffile = false;
	std::string   sender, reciver, body, command; int  i = 0; unsigned int size_stream;
	c = block.makeMessage(commandp, this->address + ":" + std::to_string(this->port), receiverAddr, sendfilename.size(), sendfilename);
	sizebolock = block.getMessageSize();
	si.send(sizebolock, c);																//send block first uploaf message
	myfile.seekg(0, myfile.end);	
	size_stream = (unsigned int)myfile.tellg();
	myfile.seekg(0, myfile.beg);
	while (true){													
		if (size_stream > 1024)	{
			myfile.read(buf, 1024); buf[1024] = '\0';
			c = block.makeHeader(commandp, this->address + ":" + std::to_string(this->port), receiverAddr, 1024);
			si.send(block.getHeaderSize(), c);si.send(1024, buf);
		}
		else{																			//for last block of message <=1024
			myfile.read(buf, size_stream); buf[size_stream] = '\0';
			c = block.makeHeader(commandp+"END", this->address + ":" + std::to_string(this->port), receiverAddr, size_stream);
			si.send(block.getHeaderSize(), c);si.send(size_stream, buf);endoffile = true;
		}
		if (endoffile == true) break;
		i += 1024;size_stream -= 1024;
	}
}

void Sender::sendMsg(Socket& si, std::string operation, std::string receiverAddr,std::string body)
{
	Message block;
	char* c = block.makeMessage(operation, this->address + ":" + std::to_string(this->port), receiverAddr, body.size(), body);
	si.send(block.getMessageSize(), c);
}

//----< File transfer request from sender to receiver >--------------------------------------------------------------------------------

void Sender::fileDownload(Socket& si, std::string filename, std::string receiverAddr)
{
	Message block;
	char* c = block.makeMessage("DOWNLOAD", this->address + ":" + std::to_string(this->port), receiverAddr, filename.size(), filename);
	si.send(block.getMessageSize(), c);
}

//----< File transfer request acknowledge after the operation based on the status >--------------------------------------------------------------------------------

void Sender::uploadfileAck(Socket& si, std::string filename, std::string receiverAddr,std::string command)
{
	Message block;
	char* c = block.makeMessage(command, this->address + ":" + std::to_string(this->port), receiverAddr, filename.size(), filename);
	si.send(block.getMessageSize(), c);
}

//----< Hex to String conversion >-----------------------------

std::string Sender::hex_to_string(const std::string& input)
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

//----< String hex conversion >-----------------------------

std::string Sender::string_to_hexSend(const std::string& input)
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

void Sender::searchText(Socket& si, std::string body, std::string receiverAddr, std::string command)
{
	Message block;	
	XmlProcessing::XmlDocument XML;
	std::vector<std::string> parsed = XML.parse(body, false);
	XML.BuildTree(parsed);
	std::string str = XML.display();
	std::vector<std::string> patterns = XML.Values(XML.elements("PATTERN").allChildren().select());
	std::vector<std::string> textV = XML.Values(XML.elements("TEXT").allChildren().select());
	std::vector<std::string>::iterator iter; std::string text = hex_to_string(*textV.begin());
	for (iter = patterns.begin(); iter != patterns.end(); iter++)  
	{
		 *iter = hex_to_string(*iter);
	}
	DataStore ds;
	FileMgr fm(".", ds);
	std::map<std::string, std::string>result = fm.SerachText(text, patterns,false); std::map<std::string, std::string>::iterator itr;
	XmlProcessing::XmlDocument XML2;
	parsed = XML2.parse("<ROOT><FILES></FILES></ROOT>", false);
	XML2.BuildTree(parsed);
	for (itr = result.begin(); itr != result.end(); itr++)
	{XML2.addChildDoc("FILES", "<" + string_to_hexSend(itr->second+"  "+itr->first) + ">");	}
	std::string sendmsg = XML2.display();
	sendString(si, sendmsg, receiverAddr, command);
}

void Sender::searchFile(Socket& si, std::string body, std::string receiverAddr, std::string command)
{
	Message block;
	XmlProcessing::XmlDocument XML;
	std::vector<std::string> parsed = XML.parse(body, false);
	XML.BuildTree(parsed);
	std::string str = XML.display();
	std::vector<std::string> patterns = XML.Values(XML.elements("PATTERN").allChildren().select());
	std::vector<std::string> DirV = XML.Values(XML.elements("DIRECTORY").allChildren().select());
	std::vector<std::string>::iterator iter; std::string Dir = hex_to_string(*DirV.begin());
	for (iter = patterns.begin(); iter != patterns.end(); iter++)
	{
		*iter = hex_to_string(*iter);
	}
	DataStore ds;
	FileMgr fm(Dir, ds);
	std::map<std::string, std::string>result = fm.SerachText(Dir, patterns,true); std::map<std::string, std::string>::iterator itr;
	XmlProcessing::XmlDocument XML2;
	parsed = XML2.parse("<ROOT><FILES></FILES></ROOT>", false);
	XML2.BuildTree(parsed);
	for (itr = result.begin(); itr != result.end(); itr++)
	{
		XML2.addChildDoc("FILES", "<" + string_to_hexSend(itr->second + "  " + itr->first) + ">");
	}
	std::string sendmsg = XML2.display();
	sendString(si, sendmsg, receiverAddr, command);

}


void Sender::sendString(Socket& si, std::string body, std::string receiverAddr, std::string command)
{
	int size_stream = body.size(); char  buf[1025]; Message block; char* c; bool endofstring = false;
	c = block.makeHeader(command, this->address + ":" + std::to_string(this->port), receiverAddr, 0);
	si.send(block.getHeaderSize(), c);// si.send(0, buf);
	while (true){
		if (size_stream > 1024)	{
			for (int i = 0; i < 1024; i++)buf[i] = body[i];
			buf[1024] = '\0';
			c = block.makeHeader(command, this->address + ":" + std::to_string(this->port), receiverAddr, 1024);
			si.send(block.getHeaderSize(), c); si.send(1024, buf);
		}
		else{																			//for last block of message <=1024
			for (int i = 0; i < size_stream; i++)buf[i] = body[i]; buf[size_stream] = '\0';
			c = block.makeHeader(command + "END", this->address + ":" + std::to_string(this->port), receiverAddr, size_stream);
			si.send(block.getHeaderSize(), c); si.send(size_stream, buf); endofstring = true;
		}
		if (endofstring == true) break;
		size_stream -= 1024; body = body.substr(1024);
	}
}

#ifdef TEST_SENDER
//----< test stub for sender >------------------------------------------------------

int main()
{
	title("Testing Socket Client", '=');

	try
	{
		Verbose v(true);
		SocketSystem ss;
		SocketConnecter si;
		Message msg;
		while (!si.connect("localhost", 9080))
		{
			Display::displayString("client waiting to connect");
			::Sleep(100);
		}

		title("File Upload");
		Sender CT(2020,"testport");
		CT.fileUpload(si, "Sender.pdf","dummy:2020","UPLOAD","");
		CT.fileDownload(si, "Pavan.txt","dummy:2020");
		si.shutDownSend();
	}
	catch (std::exception& ex)
	{
		Display::displayString("  Exception caught:");
		Display::displayString(std::string("\n  ") + ex.what() + "\n\n");
	}
}
#endif