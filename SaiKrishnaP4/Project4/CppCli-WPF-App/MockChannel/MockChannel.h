#ifndef MOCKCHANNEL_H
#define MOCKCHANNEL_H

/////////////////////////////////////////////////////////////////////////////
// MockChannel.h - channel for UI and Native client			               //
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
/*
*  Package Operations:
*  -------------------
*IClient- acts as the channel in between UI and Native Client takes in SendeI and RecevI
*ISendr - client extracts messahes passed by UI through ISendr
*IRecev - client sends messahes passed to UI through IRecev
*ObjectFactoryI - creates instances for SendeI, RecevI and IClient
*
* Public Interface:
* =================
*
* void startC(std::string, std::string) starts Native client
* void stopC()							stops thread spanned for Native client
* void startClient()					start Native client
* void postMessage(const Message1& msg)	post message enqueues sende queue
* virtual std::string getMessage()		get message dequeues receiver queue
* ISendr* createSendr()					creates and returns Sendr
* IRecvr* createRecvr()					creates and returns Recvr
* IClient* createClient(ISendr* pISendr, IRecvr* pIRecvr)	creates and returns Client
*
*  Required Files:
*  ---------------
*  Sockets.h, Sockets.cpp
*  AppHelpers.h, AppHelpers.cpp, WindowsHelpers.h, WindowsHelpers.cpp, Sender.h. Sender.cpp, Reveicer.h, Receiver.cpp, Display.h , Display.cpp,
*			Message.h and Message.cpp
*
*  Build Command:
*  --------------
*  devenv CppCli-WPF-App_original.sln /rebuild debug
*
*
*  Maintenance History:
*  --------------------
*  Version 2.0 27 April 15
*/
#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif

#include <string>
using Message1 = std::string;

/////////////////////////////////////////////////////////////////////////////
// ISendr Interface
// - accepts messages from client for consumption by MockChannel
//
struct ISendr
{
  virtual void postMessage(const Message1& msg) = 0;
};

/////////////////////////////////////////////////////////////////////////////
// IRecvr Interface
// - accepts messages from UI for consumption by MockChannel
//
struct IRecvr
{
  virtual std::string getMessage() = 0;
};

/////////////////////////////////////////////////////////////////////////////
// IClient Interface
// - channel between UI and Naive client
//
struct IClient
{
public:
	virtual void startC(std::string, std::string) = 0;
	virtual void stopC() = 0;
	virtual void startClient() = 0;
};

/////////////////////////////////////////////////////////////////////////////
// ObjectFactory Interface
// - creates instances of Iclient, SendeI a,d RecevI
extern "C" {
  struct ObjectFactory
  {
    DLL_DECL ISendr* createSendr();
    DLL_DECL IRecvr* createRecvr();
   	DLL_DECL IClient* createClient(ISendr* pISendr, IRecvr* pIRecvr);
  };
}

#endif


