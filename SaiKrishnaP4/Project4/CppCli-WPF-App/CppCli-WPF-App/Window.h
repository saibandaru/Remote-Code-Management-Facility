#ifndef WINDOW_H
#define WINDOW_H
/////////////////////////////////////////////////////////////////////////////
// Window.h - C++\CLI implementation of WPF Application                    //
// Ver 1.0                                                                 //
// Sai Krishna, CSE#687 - Object Oriented Design, Spring 2015              //
// CST 4-187, Syracuse University, 832 940-8083, sbandaru@syr.edu	       //
//-------------------------------------------------------------------------//
// Sai Krishna (c) copyright 2015                                          //
// All rights granted provided this copyright notice is retained           //
//-------------------------------------------------------------------------//
// Application: UI for Project #4, WPF Application				           //
// Platform:    Dell 2720, Win Pro 8.1, Visual Studio 2013                 //
/////////////////////////////////////////////////////////////////////////////
/*
*  Package Operations:
*  -------------------
*  This package demonstrates how to build a C++\CLI WPF application.  It
*  provides one class, WPFCppCliDemo, derived from System::Windows::Window
*  that is compiled with the /clr option to run in the Common Language
*  Runtime, and another class MockChannel written in native C++ and compiled
*  as a DLL with no Common Language Runtime support.
*
*  The window class hosts, in its window, a tab control with three views, two
*  of which are provided with functionality that are required for Project #4.
*  It loads the DLL holding MockChannel.  MockChannel hosts a send queue, a
*  receive queue, and a C++11 thread that reads from the send queue and writes
*  the deQ'd message to the receive queue.
*
*  The Client can post a message to the MockChannel's send queue.  It hosts
*  a receive thread that reads the receive queue and dispatches any message
*  read to a ListBox in the Client's FileList tab.
*
*  The application can upload file to the server into selected folder.
*
*  The application can download a file from the server of selected folder.
*  
*  The applocation can perform file searches on the selected folders, based on the patterns
*
*  The applocation can perform file searches on the selected folders, based on the patterns
*
* Public Interface:
* =================
*
* WPFCppCliDemo(String^,String^)		-Constructs that takes listner port and client download folder as input
*
*  Required Files:
*  ---------------
*  Windows.cpp
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

using namespace System;
using namespace System::Text;
using namespace System::Windows;
using namespace System::Windows::Input;
using namespace System::Windows::Markup;
using namespace System::Windows::Media;                   
using namespace System::Windows::Controls;                
using namespace System::Windows::Controls::Primitives;    
using namespace System::Threading;
using namespace System::Threading::Tasks;
using namespace System::Windows::Threading;
using namespace System::ComponentModel;

#include "../MockChannel/MockChannel.h"
#include <iostream>

namespace CppCliWindows
{
  ref class WPFCppCliDemo : Window
  {
    // Client communication references

   	ISendr* pSendr1_;
	IRecvr* pRecvr1_;
	IClient* pClient_;

    // Controls for Window

    DockPanel^ hDockPanel = gcnew DockPanel();      // support docking statusbar at bottom
    Grid^ hGrid = gcnew Grid();                    
    TabControl^ hTabControl = gcnew TabControl();
	TabItem^ hConnectTab = gcnew TabItem();
	TabItem^ hTabUpload = gcnew TabItem();
	TabItem^ hDownloadTab = gcnew TabItem();
	TabItem^ hSendMessageTab = gcnew TabItem();
	TabItem^ hFileTextSearchTab = gcnew TabItem();
	TabItem^ hTextSearchTab = gcnew TabItem();
    TabItem^ hFileListTab = gcnew TabItem();
    StatusBar^ hStatusBar = gcnew StatusBar();
    StatusBarItem^ hStatusBarItem = gcnew StatusBarItem();
    TextBlock^ hStatus = gcnew TextBlock();

	// Controls for Connection View

	Grid^ hConnectionGrid = gcnew Grid();
	Button^ hConnectButton1 = gcnew Button();
	Button^ hDisconnectButton1 = gcnew Button();
	TextBox^ textbox_server = gcnew TextBox();
	TextBox^ textbox_port = gcnew TextBox();
	ScrollViewer^ hScrollViewer2 = gcnew ScrollViewer();
	StackPanel^ hStackPanel2 = gcnew StackPanel();

	// Controls for Upload View

	Grid^ hUploadGrid = gcnew Grid();
	Forms::FolderBrowserDialog^ hFolderBrowserDialog_U = gcnew Forms::FolderBrowserDialog();
	CheckBox^ checkBox_U1 = gcnew CheckBox();
	CheckBox^ checkBox_U2 = gcnew CheckBox();
	TextBox^ textbox_U1 = gcnew TextBox();
	TextBox^ textbox_U2 = gcnew TextBox();
	ComboBox^ DirectorycomboBox_U = gcnew ComboBox();
	ListBox^ hListBox_U = gcnew ListBox();
	Button^ hFolderBrowseButton_U = gcnew Button();
	Button^ hUploadButton_U = gcnew Button();
	StackPanel^ hStackPanel_U = gcnew StackPanel();
	StackPanel^ hStackPanel_U1 = gcnew StackPanel();
	StackPanel^ hStackPanel_U2 = gcnew StackPanel();
	Grid^ hGrid_U = gcnew Grid();
	
	// Controls for Download View

	Grid^ hDownloadGrid = gcnew Grid();
	CheckBox^ checkBox_D1 = gcnew CheckBox();
	CheckBox^ checkBox_D2 = gcnew CheckBox();
	TextBox^ textbox_D1 = gcnew TextBox();
	TextBox^ textbox_D2 = gcnew TextBox();
	ComboBox^ DirectorycomboBox_D = gcnew ComboBox();
	Button^ hDownload_Button = gcnew Button();
	TextBox^ hTextBox_D = gcnew TextBox();
	TextBox^ hTextBox_D3 = gcnew TextBox();
	StackPanel^ hStackPanel_D3 = gcnew StackPanel();
	StackPanel^ hStackPanel_D1 = gcnew StackPanel();
	StackPanel^ hStackPanel_D2 = gcnew StackPanel();
	TextBlock^ hTextBlock_D = gcnew TextBlock();
	
	// Controls for File Text Serach View

	Grid^ hFileTextSerachGrid = gcnew Grid();
	CheckBox^ checkBox_FS1 = gcnew CheckBox();
	CheckBox^ checkBox_FS2 = gcnew CheckBox();
	TextBox^ textbox_FS1 = gcnew TextBox();
	TextBox^ textbox_FS2 = gcnew TextBox();
	TextBlock^ AddedPatt = gcnew TextBlock();
	ListBox^ hListBox_FS = gcnew ListBox();
	TextBox^ hAddPatternBox = gcnew TextBox();
	ComboBox^ DirectorycomboBox1 = gcnew ComboBox();
	TextBox^ hDirectoryBox = gcnew TextBox();
	Button^ hAddPattern = gcnew Button();
	Button^ hSerach = gcnew Button();
	StackPanel^ hStackPanel_FTS1 = gcnew StackPanel();
	StackPanel^ hStackPanel_FTS2 = gcnew StackPanel();
	StackPanel^ hStackPanel_FTS3 = gcnew StackPanel();
	StackPanel^ hStackPanel_FTS4 = gcnew StackPanel();
	
	// Controls for Text Serach View

	Grid^ hTextSerachGrid = gcnew Grid();
	CheckBox^ checkBox_TS1 = gcnew CheckBox();
	CheckBox^ checkBox_TS2 = gcnew CheckBox();
	TextBox^ textbox_TS1 = gcnew TextBox();
	TextBox^ textbox_TS2 = gcnew TextBox();
	ListBox^ hListBox_TS = gcnew ListBox();
	TextBlock^ AddedPatt_TS = gcnew TextBlock();
	TextBox^ hAddPatternBox_TS = gcnew TextBox();
	TextBox^ hTestBox = gcnew TextBox();
	Button^ hAddPattern_TS = gcnew Button();
	Button^ hSerach_TS = gcnew Button();
	StackPanel^ hStackPanel_TS3 = gcnew StackPanel();
	StackPanel^ hStackPanel_TS4 = gcnew StackPanel();
	StackPanel^ hStackPanel_TS1 = gcnew StackPanel();
	StackPanel^ hStackPanel_TS2 = gcnew StackPanel();

	// Controls for SendMessage View

    Grid^ hSendMessageGrid = gcnew Grid();
    Button^ hSendButton = gcnew Button();
    Button^ hClearButton = gcnew Button();
    TextBlock^ hTextBlock1 = gcnew TextBlock();
    ScrollViewer^ hScrollViewer1 = gcnew ScrollViewer();
    StackPanel^ hStackPanel1 = gcnew StackPanel();

    String^ msgText 
      = "Command:ShowMessage\n"   // command
      + "Sendr:localhost@8080\n"  // send address
      + "Recvr:localhost@8090\n"  // receive address
      + "Content-length:44\n"     // body length attribute
      + "\n"                      // end header
      + "Hello World\nCSE687 - Object Oriented Design";  // message body

    // Controls for FileListView View

    Grid^ hFileListGrid = gcnew Grid();
    Forms::FolderBrowserDialog^ hFolderBrowserDialog = gcnew Forms::FolderBrowserDialog();
    ListBox^ hListBox = gcnew ListBox();
    Button^ hFolderBrowseButton = gcnew Button();
    Grid^ hGrid2 = gcnew Grid();
	
    // receive thread

    Thread^ recvThread;

  public:
	  WPFCppCliDemo(String^,String^);
	  ~WPFCppCliDemo();

    
  private:
	  String^ serveraddress1 = "DEFAULT";
	  String^ serveraddress2 = "DEFAULT";
	  String^ AddedPatterns_TS = "Added Patterns: ";
	  String^ AddedPatterns_FS = "Added Patterns: ";
	std::string toStdString(String^ pStr);
	String^ toSystemString(std::string& str);
	void setServerstack();		//server tabs for upload screen
	void setServerstack_D();		//server tabs for download screen
	void setServerstack_TS();		//server tabs for textsearch screen
	void setServerstack_FS();		//server tabs for file search screen
	void setTextBlockProperties();		//text block properties for search text screen
	void setButtonsProperties();		//buttons properties for upload screen
	void setTextBlockPropertiesC();		//text block properties for connection screen
	void setButtonsPropertiesC();		//buttons properties for search text screen
	void setListPropertiesFS();			//list properties for file search screen
	void setTextBlockPropertiesFS();	//text properties for file search screen
	void setButtonsPropertiesFS();      //button properties for file search screen
	void setListPropertiesTS();      //list properties for text search screen
	void setTextBlockPropertiesTS();      //text block properties for text search screen
	void setButtonsPropertiesTS();      //button properties for text search screen

	void setUpStatusBar();		 //to set up status bar
	void setUpTabControl();      //to setup tabs
	void setUpConnectionView();	//to connection tabs
	void setUpUploadView();		//to setup upload view
	void setDownload();			//to setup download view
	void setUpFileTextSerach(); //to setup file search view
	void setUpTextSerach();		//to setup text search view
    void setUpSendMessageView();//to setup message view
    void setUpFileListView();	//to setup filelist view
    

    void sendMessage(Object^ obj, RoutedEventArgs^ args);	//for operation of message click
	void addText(String^ msg);	//for operation of add test click
	void getMessage();	//for operation of get message click
	void clear(Object^ sender, RoutedEventArgs^ args);	//for operation of clear click
	void browseForFolder_U(Object^ sender, RoutedEventArgs^ args);	//for operation of browde folder click
	void UploadFile_U(Object^ sender, RoutedEventArgs^ args);	//for operation of upload file click
	void browseForFolder(Object^ sender, RoutedEventArgs^ args);	//for operation of browse upload click
	void Connect(Object^ sender, RoutedEventArgs^ args);	//for operation of connect click
	void Disconnect(Object^ sender, RoutedEventArgs^ args);	//for operation of disconnect click
	void Download(Object^ sender, RoutedEventArgs^ args);	//for operation of download click
	void AddText(Object^ sender, RoutedEventArgs^ args);	//for operation of add text click
	void AddPattern(Object^ sender, RoutedEventArgs^ args);	//for operation of add pattern for file and text search click
	void Serach(Object^ sender, RoutedEventArgs^ args);	//for operation of file search click
	void AddPattern_TS(Object^ sender, RoutedEventArgs^ args);	//for operation of text search add pattern click
	void Serach_TS(Object^ sender, RoutedEventArgs^ args);	//for operation of search text click
	void Serach_FS(Object^ sender, RoutedEventArgs^ args);	//for operation of file search click
	void AddPattern_FS(Object^ sender, RoutedEventArgs^ args);	//for operation of file search add pattern click
	void OnLoaded(Object^ sender, RoutedEventArgs^ args);	//for operation of onloaded click
	void Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args);	//for operation of unloading click
  };
}


#endif
