/////////////////////////////////////////////////////////////////////////////
// Window.cpp - C++\CLI implementation of WPF Application                  //
// Ver 1.0                                                                 //
// Sai Krishna, CSE#687 - Object Oriented Design, Spring 2015              //
// CST 4-187, Syracuse University, 832 940-8083, sbandaru@syr.edu	       //
//-------------------------------------------------------------------------//
// Sai Krishna (c) copyright 2015                                          //
// All rights granted provided this copyright notice is retained           //
//-------------------------------------------------------------------------//
// Application: Server for Project #4, WPF Application			           //
// Platform:    Dell 2720, Win Pro 8.1, Visual Studio 2013                 //
/////////////////////////////////////////////////////////////////////////////
/*
*  To run as a Windows Application:
*  - Set Project Properties > Linker > System > Subsystem to Windows
*  - Comment out int main(...) at bottom
*  - Uncomment int _stdcall WinMain() at bottom
*  To run as a Console Application:
*  - Set Project Properties > Linker > System > Subsytem to Console
*  - Uncomment int main(...) at bottom
*  - Comment out int _stdcall WinMain() at bottom
*/
#include "Window.h"
#include <string>
using namespace CppCliWindows;

WPFCppCliDemo::WPFCppCliDemo(String^ address, String^ path)
{
  // set up channel
  ObjectFactory* pObjFact = new ObjectFactory;
  pSendr1_ = pObjFact->createSendr();
  pRecvr1_ = pObjFact->createRecvr();
  pClient_ = pObjFact->createClient(pSendr1_, pRecvr1_);
  pClient_->startC(toStdString(address), toStdString(path));
  delete pObjFact;
  // client's receive thread
  //recvThread = gcnew Thread(gcnew ThreadStart(this, &WPFCppCliDemo::getMessage));
  //recvThread->Start();
  // set event handlers
  this->Loaded += gcnew System::Windows::RoutedEventHandler(this, &WPFCppCliDemo::OnLoaded);
  this->Closing +=gcnew CancelEventHandler(this, &WPFCppCliDemo::Unloading);
  hConnectButton1->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::Connect);
  hDisconnectButton1->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::Disconnect);
  hFolderBrowseButton_U->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::browseForFolder_U);
  hUploadButton_U->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::UploadFile_U);
  hDownload_Button->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::Download);
  hAddPattern->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::AddPattern);
  hAddPattern_TS->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::AddPattern_TS);
  hSerach_TS->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::Serach_TS);
  hAddPattern->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::AddPattern_FS);
  hSerach->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::Serach_FS);
  hSendButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::sendMessage);
  hClearButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::clear);
  hFolderBrowseButton->Click += gcnew RoutedEventHandler(this, &WPFCppCliDemo::browseForFolder);
  // set Window properties
  this->Title = "WPF C++/CLI Demo";
  this->Width = 800;
  this->Height = 600;
  // attach dock panel to Window
  this->Content = hDockPanel;
  hDockPanel->Children->Add(hStatusBar);
  hDockPanel->SetDock(hStatusBar, Dock::Bottom);
  hDockPanel->Children->Add(hGrid);
  // setup Window controls and views
  setUpTabControl();
  setUpStatusBar();
  setUpConnectionView();
  setUpUploadView();
  setDownload();
  setUpFileTextSerach();
  setUpTextSerach();
  setUpSendMessageView();
  setUpFileListView();
}

WPFCppCliDemo::~WPFCppCliDemo()
{
   delete pSendr1_;
  delete pRecvr1_;
}

//----< setUpStatusBar for the UI applocation >------------------------------------
void WPFCppCliDemo::setUpStatusBar()
{
  hStatusBar->Items->Add(hStatusBarItem);
  hStatus->Text = "very important messages will appear here";
  hStatusBarItem->Content = hStatus;
  hStatusBar->Padding = Thickness(10, 2, 10, 2);
}

//----< setup tab control for the UI applocation >------------------------------------
void WPFCppCliDemo::setUpTabControl()
{
  hGrid->Children->Add(hTabControl);
  //hConnectTab->Header = "Connect";
  hTabUpload->Header = "Upload File";
  hDownloadTab->Header = "Download File";
  hFileTextSearchTab->Header = "File Search";
  hTextSearchTab->Header = "Text Search";
  //hTabControl->Items->Add(hConnectTab);
  hTabControl->Items->Add(hTabUpload);
  hTabControl->Items->Add(hDownloadTab);
  hTabControl->Items->Add(hFileTextSearchTab);
  hTabControl->Items->Add(hTextSearchTab);
 }

//----< set up connection view for the UI applocation >------------------------------------
void WPFCppCliDemo::setUpConnectionView()
{
	Console::Write("\n  setting up Connection view");
	hStatus->Text = "Make Or Break Connection";
	hConnectionGrid->Margin = Thickness(20);
	hConnectTab->Content = hConnectionGrid;
	setTextBlockPropertiesC();
	setButtonsPropertiesC();
}

//----< set up search text block for the UI applocation >------------------------------------
void WPFCppCliDemo::setTextBlockPropertiesC()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(50);
	hConnectionGrid->RowDefinitions->Add(hRow1Def);
	RowDefinition^ hRow1Def1 = gcnew RowDefinition();
	hRow1Def1->Height = GridLength(50);
	hConnectionGrid->RowDefinitions->Add(hRow1Def1);
	textbox_server->Padding = Thickness(0);
	textbox_server->Text = "";
	textbox_server->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	textbox_server->FontWeight = FontWeights::Bold;
	textbox_server->FontSize = 12;
	textbox_server->Height = 20;
	textbox_server->Width = 200;
	textbox_server->LineLeft();

	textbox_port->Padding = Thickness(0);
	textbox_port->Text = "";
	textbox_port->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	textbox_port->FontWeight = FontWeights::Bold;
	textbox_port->FontSize = 12;
	textbox_port->Height = 20;
	textbox_port->Width = 200;
	textbox_port->LineLeft();
	Label^ label_address = gcnew Label();
	Label^ label_port = gcnew Label();
	label_address->Content = "Server Address:";
	label_address->Height = 30;
	label_address->Width = 400;
	label_port->Content = "Server Port:";
	label_port->Height = 30;
	label_port->Width = 400;

	hConnectionGrid->SetRow(label_address, 0);
	hConnectionGrid->Children->Add(label_address);
	hConnectionGrid->SetRow(textbox_server, 0);
	hConnectionGrid->Children->Add(textbox_server);
	hConnectionGrid->SetRow(label_port, 1);
	hConnectionGrid->Children->Add(label_port);
	hConnectionGrid->SetRow(textbox_port, 1);
	hConnectionGrid->Children->Add(textbox_port);
}

//----< set up button for the UI applocation >------------------------------------
void WPFCppCliDemo::setButtonsPropertiesC()
{
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(50);
	hConnectionGrid->RowDefinitions->Add(hRow2Def);
	hConnectButton1->Content = "Connect";
	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 120;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hDisconnectButton1->Content = "Disconnect";
	hBorder2->Child = hConnectButton1;
	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 120;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hDisconnectButton1;
	hStackPanel2->Children->Add(hBorder2);
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 10;
	hStackPanel2->Children->Add(hSpacer);
	hStackPanel2->Children->Add(hBorder3);
	hStackPanel2->Orientation = Orientation::Horizontal;
	hStackPanel2->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hConnectionGrid->SetRow(hStackPanel2, 2);
	hConnectionGrid->Children->Add(hStackPanel2);
}

//----< set up upload view for the UI applocation >------------------------------------
void WPFCppCliDemo::setUpUploadView()
{
	Console::Write("\n  setting up Upload view");
	hUploadGrid->Margin = Thickness(20);
	hTabUpload->Content = hUploadGrid;
	setServerstack();
	RowDefinition^ hRowU1Def = gcnew RowDefinition();
	hRowU1Def->Height = GridLength(75);
	hUploadGrid->RowDefinitions->Add(hRowU1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hListBox_U;


	RowDefinition^ hRowU2Def = gcnew RowDefinition();
	//hRowU2Def->Height = GridLength(75);
	hUploadGrid->RowDefinitions->Add(hRowU2Def);

	hUploadGrid->SetRow(hBorder1, 3);
	hUploadGrid->Children->Add(hBorder1);

	hFolderBrowseButton_U->Content = "Browse";
	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 120;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hUploadButton_U->Content = "Upload";
	hBorder2->Child = hFolderBrowseButton_U;
	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 120;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hUploadButton_U;
	hStackPanel_U->Children->Add(hBorder2);
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 10;
	hStackPanel_U->Children->Add(hSpacer);
	hStackPanel_U->Children->Add(hBorder3);
	hStackPanel_U->Orientation = Orientation::Horizontal;
	hStackPanel_U->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hUploadGrid->SetRow(hStackPanel_U, 2);
	hUploadGrid->Children->Add(hStackPanel_U);

	hFolderBrowserDialog_U->ShowNewFolderButton = false;
	hFolderBrowserDialog_U->SelectedPath = System::IO::Directory::GetCurrentDirectory();
}

void WPFCppCliDemo::setServerstack()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();hRow1Def->Height = GridLength(30);hUploadGrid->RowDefinitions->Add(hRow1Def);

	RowDefinition^ hRow2Def = gcnew RowDefinition();hRow2Def->Height = GridLength(30);hUploadGrid->RowDefinitions->Add(hRow2Def);

	checkBox_U1->Width = 20;checkBox_U1->Height = 20;
	hStackPanel_U1->Children->Add(checkBox_U1);

	TextBlock^ Srver_no = gcnew TextBlock();Srver_no->Width = 100;Srver_no->Height = 20;Srver_no->Text = "Server Address 1:";
	hStackPanel_U1->Children->Add(Srver_no);

	checkBox_U2->Width = 20;checkBox_U2->Height = 20;
	hStackPanel_U2->Children->Add(checkBox_U2);

	TextBlock^ Srver_no1 = gcnew TextBlock();Srver_no1->Width = 100;	Srver_no1->Height = 20;	Srver_no1->Text = "Server Address 2:";
	hStackPanel_U2->Children->Add(Srver_no1);


	textbox_U1->FontSize = 12;textbox_U1->Height = 20;	textbox_U1->Width = 200;
	hStackPanel_U1->Children->Add(textbox_U1);

	TextBlock^ Srver_no2 = gcnew TextBlock();Srver_no2->Width = 50;	Srver_no2->Height = 20;
	hStackPanel_U1->Children->Add(Srver_no2);

	TextBlock^ Srver_no3 = gcnew TextBlock();Srver_no3->Width = 150;	Srver_no3->Height = 20;	Srver_no3->Text = "Select Directory upload to:";
	hStackPanel_U1->Children->Add(Srver_no3);
	 
	DirectorycomboBox_U->Height = 20;	DirectorycomboBox_U->Width = 90;//	DirectorycomboBox_U->Height = 20;DirectorycomboBox_U->Width = 90;

	DirectorycomboBox_U->Items->Add("C#");	DirectorycomboBox_U->Items->Add("C++");	DirectorycomboBox_U->Items->Add("Java");
	DirectorycomboBox_U->SelectedIndex = 1;	DirectorycomboBox_U->VerticalAlignment = System::Windows::VerticalAlignment::Center;
	hStackPanel_U1->Children->Add(DirectorycomboBox_U);

	textbox_U2->FontSize = 12;	textbox_U2->Height = 20;	textbox_U2->Width = 200;
	hStackPanel_U2->Children->Add(textbox_U2);

	hStackPanel_U1->Orientation = Orientation::Horizontal;hStackPanel_U1->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;

	hStackPanel_U2->Orientation = Orientation::Horizontal;hStackPanel_U2->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;

	hUploadGrid->SetRow(hStackPanel_U1, 0);	hUploadGrid->Children->Add(hStackPanel_U1);
	hUploadGrid->SetRow(hStackPanel_U2, 1);	hUploadGrid->Children->Add(hStackPanel_U2);
}

//----< set up download view for the UI applocation >------------------------------------
void WPFCppCliDemo::setDownload()
{
	Console::Write("\n  setting up sendMessage view");
	hDownloadGrid->Margin = Thickness(75);
	hDownloadTab->Content = hDownloadGrid;
	setServerstack_D();
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hRow1Def->Height = GridLength(75);
	hDownloadGrid->RowDefinitions->Add(hRow1Def);
	hTextBox_D->Padding = Thickness(0);
	hTextBox_D->Text = "";
	hTextBox_D->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hTextBox_D->FontWeight = FontWeights::Bold;
	hTextBox_D->FontSize = 12;
	hTextBox_D->Height = 20;
	hTextBox_D->Width = 150;
	hTextBox_D->LineLeft();

	Label^ label_filename = gcnew Label();
	label_filename->Content = "Filename:";
	label_filename->Height = 30;
	label_filename->Width = 85;
	hStackPanel_D3->Children->Add(label_filename);
	hStackPanel_D3->Children->Add(hTextBox_D);

	hStackPanel_D3->Orientation = Orientation::Horizontal;
	hStackPanel_D3->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;

	hDownloadGrid->SetRow(hStackPanel_D3, 2);
	hDownloadGrid->Children->Add(hStackPanel_D3);

	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(50);
	hDownloadGrid->RowDefinitions->Add(hRow2Def);
	hDownload_Button->Content = "Download";
	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 120;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hBorder2->Child = hDownload_Button;

	hDownloadGrid->SetRow(hBorder2, 3);
	hDownloadGrid->Children->Add(hBorder2);
}

//----< set up server options of download view for the UI applocation >------------------------------------
void WPFCppCliDemo::setServerstack_D()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();hRow1Def->Height = GridLength(30);hDownloadGrid->RowDefinitions->Add(hRow1Def);

	RowDefinition^ hRow2Def = gcnew RowDefinition();hRow2Def->Height = GridLength(30);hDownloadGrid->RowDefinitions->Add(hRow2Def);

	checkBox_D1->Width = 20;checkBox_D1->Height = 20;
	hStackPanel_D1->Children->Add(checkBox_D1);

	TextBlock^ Srver_no = gcnew TextBlock();Srver_no->Width = 100;Srver_no->Height = 20;Srver_no->Text = "Server Address 1:";
	hStackPanel_D1->Children->Add(Srver_no);

	textbox_D1->FontSize = 12;textbox_D1->Height = 20;textbox_D1->Width = 200;
	hStackPanel_D1->Children->Add(textbox_D1);

	TextBlock^ Srver_no12 = gcnew TextBlock();Srver_no12->Width = 10;Srver_no12->Height = 20;
	hStackPanel_D1->Children->Add(Srver_no12);
	
	TextBlock^ Srver_no11 = gcnew TextBlock();Srver_no11->Width = 200;Srver_no11->Height = 20;Srver_no11->Text = "Select Directory to download from :";
	hStackPanel_D1->Children->Add(Srver_no11);

	DirectorycomboBox_D->Height = 20;DirectorycomboBox_D->Width = 90;//DirectorycomboBox_D->Height = 20;DirectorycomboBox_D->Width = 90;
	DirectorycomboBox_D->Items->Add("C#");DirectorycomboBox_D->Items->Add("C++");DirectorycomboBox_D->Items->Add("Java");
	DirectorycomboBox_D->SelectedIndex = 1;	DirectorycomboBox_D->VerticalAlignment = System::Windows::VerticalAlignment::Center;
	hStackPanel_D1->Children->Add(DirectorycomboBox_D);

	checkBox_D2->Width = 20;checkBox_D2->Height = 20;
	hStackPanel_D2->Children->Add(checkBox_D2);

	TextBlock^ Srver_no1 = gcnew TextBlock();Srver_no1->Width = 100;Srver_no1->Height = 20;Srver_no1->Text = "Server Address 2:";
	hStackPanel_D2->Children->Add(Srver_no1);


	textbox_D2->FontSize = 12;textbox_D2->Height = 20;textbox_D2->Width = 200;
	hStackPanel_D2->Children->Add(textbox_D2);

	hStackPanel_D1->Orientation = Orientation::Horizontal;hStackPanel_D1->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;

	hStackPanel_D2->Orientation = Orientation::Horizontal;hStackPanel_D2->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;

	hDownloadGrid->SetRow(hStackPanel_D1, 0);hDownloadGrid->Children->Add(hStackPanel_D1);
	hDownloadGrid->SetRow(hStackPanel_D2, 1);hDownloadGrid->Children->Add(hStackPanel_D2);
}

//----< set up file search view for the UI applocation >------------------------------------
void WPFCppCliDemo::setUpFileTextSerach()
{
	Console::Write("\n  Setting up File Text Serach view");
	hStatus->Text = "Choose your option and proceed!";
	hFileTextSerachGrid->Margin = Thickness(20);
	hFileTextSearchTab->Content = hFileTextSerachGrid;
	setServerstack_FS();
	setTextBlockPropertiesFS();
	setButtonsPropertiesFS();
	setListPropertiesFS();
}

//----< set up server options of file search view for the UI applocation >------------------------------------
void WPFCppCliDemo::setServerstack_FS()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();hRow1Def->Height = GridLength(30);
	hFileTextSerachGrid->RowDefinitions->Add(hRow1Def);

	RowDefinition^ hRow2Def = gcnew RowDefinition();hRow2Def->Height = GridLength(30);
	hFileTextSerachGrid->RowDefinitions->Add(hRow2Def);

	checkBox_FS1->Width = 20;checkBox_FS1->Height = 20;
	hStackPanel_FTS3->Children->Add(checkBox_FS1);

	TextBlock^ Srver_no = gcnew TextBlock();Srver_no->Width = 100;Srver_no->Height = 20;Srver_no->Text = "Server Address 1:";
	hStackPanel_FTS3->Children->Add(Srver_no);

	checkBox_FS2->Width = 20;checkBox_FS2->Height = 20;
	hStackPanel_FTS4->Children->Add(checkBox_FS2);

	TextBlock^ Srver_no1 = gcnew TextBlock();Srver_no1->Width = 100;Srver_no1->Height = 20;Srver_no1->Text = "Server Address 2:";
	hStackPanel_FTS4->Children->Add(Srver_no1);
	
	textbox_FS1->FontSize = 12;textbox_FS1->Height = 20;textbox_FS1->Width = 200;
	hStackPanel_FTS3->Children->Add(textbox_FS1);

	textbox_FS2->FontSize = 12;textbox_FS2->Height = 20;textbox_FS2->Width = 200;
	hStackPanel_FTS4->Children->Add(textbox_FS2);

	hStackPanel_FTS3->Orientation = Orientation::Horizontal;hStackPanel_FTS3->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;

	hStackPanel_FTS4->Orientation = Orientation::Horizontal;hStackPanel_FTS4->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;

	hFileTextSerachGrid->SetRow(hStackPanel_FTS3, 0);hFileTextSerachGrid->Children->Add(hStackPanel_FTS3);
	hFileTextSerachGrid->SetRow(hStackPanel_FTS4, 1);hFileTextSerachGrid->Children->Add(hStackPanel_FTS4);
}

//----< set up list properties of file search view for the UI applocation >------------------------------------
void WPFCppCliDemo::setListPropertiesFS()
{
	hFileTextSerachGrid->Margin = Thickness(20);
	hFileTextSearchTab->Content = hFileTextSerachGrid;
	RowDefinition^ hRowFS1Def = gcnew RowDefinition();
	//hRowU1Def->Height = GridLength(400);
	hFileTextSerachGrid->RowDefinitions->Add(hRowFS1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hListBox_FS;
	hFileTextSerachGrid->SetRow(hBorder1, 4);
	hFileTextSerachGrid->Children->Add(hBorder1);
}

//----< set up serch text block of file search view for the UI applocation >------------------------------------
void WPFCppCliDemo::setTextBlockPropertiesFS()
{
	RowDefinition^ hRowFS1Def = gcnew RowDefinition();
	hRowFS1Def->Height = GridLength(50);
	hFileTextSerachGrid->RowDefinitions->Add(hRowFS1Def);

	hAddPatternBox->FontSize = 12;
	hAddPatternBox->Height = 20;
	hAddPatternBox->Width = 250;
	hStackPanel_FTS1->Children->Add(hAddPatternBox);

	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 180;
	hStackPanel_FTS1->Children->Add(hSpacer);

	TextBlock^ Label_Directory = gcnew TextBlock();
	Label_Directory->Text = "Directory:";
	Label_Directory->Height = 20;
	Label_Directory->Width = 70;
	hStackPanel_FTS1->Children->Add(Label_Directory);

	
	DirectorycomboBox1->Height = 20;
	DirectorycomboBox1->Width = 90;
	DirectorycomboBox1->Height = 20;
	DirectorycomboBox1->Width = 90;

	DirectorycomboBox1->Items->Add("C#");
	DirectorycomboBox1->Items->Add("C++");
	DirectorycomboBox1->Items->Add("Java");
	DirectorycomboBox1->SelectedIndex = 1;
	DirectorycomboBox1->VerticalAlignment = System::Windows::VerticalAlignment::Center;
	hStackPanel_FTS1->Children->Add(DirectorycomboBox1);
	
	hStackPanel_FTS1->Orientation = Orientation::Horizontal;
	hStackPanel_FTS1->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hFileTextSerachGrid->SetRow(hStackPanel_FTS1, 2);
	hFileTextSerachGrid->Children->Add(hStackPanel_FTS1);
}

//----< set up button properties of file search view for the UI applocation >------------------------------------
void WPFCppCliDemo::setButtonsPropertiesFS()
{
	RowDefinition^ hRowFSDef = gcnew RowDefinition();
	hRowFSDef->Height = GridLength(50);
	hFileTextSerachGrid->RowDefinitions->Add(hRowFSDef);

	hAddPattern->Content = "Add Pattern";
	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 120;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hAddPattern;
	hStackPanel_FTS2->Children->Add(hBorder3);

	TextBlock^ pSpacer = gcnew TextBlock();
	pSpacer->Height = 30;
	pSpacer->Width = 40;
	hStackPanel_FTS2->Children->Add(pSpacer);

	AddedPatt->Height = 30;
	AddedPatt->Width = 360;
	AddedPatt->Text = AddedPatterns_FS;
	hStackPanel_FTS2->Children->Add(AddedPatt);

	hSerach->Content = "Search";
	Border^ hBorder4 = gcnew Border();
	hBorder4->Width = 120;
	hBorder4->Height = 30;
	hBorder4->BorderThickness = Thickness(1);
	hBorder4->BorderBrush = Brushes::Black;
	hBorder4->Child = hSerach;
	hStackPanel_FTS2->Children->Add(hBorder4);

	hStackPanel_FTS2->Orientation = Orientation::Horizontal;
	hStackPanel_FTS2->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hFileTextSerachGrid->SetRow(hStackPanel_FTS2, 3);
	hFileTextSerachGrid->Children->Add(hStackPanel_FTS2);
}

//----< set up text box properties of file search view for the UI applocation >------------------------------------
void WPFCppCliDemo::setUpTextSerach()
{
	Console::Write("\n  Setting up File Text Serach view");
	hStatus->Text = "Choose your option and proceed";
	hTextSerachGrid->Margin = Thickness(20);
	hTextSearchTab->Content = hTextSerachGrid;
	setServerstack_TS();
	setTextBlockPropertiesTS();
	setButtonsPropertiesTS();
	setListPropertiesTS();
}

//----< set up server text box properties of file search view for the UI applocation >------------------------------------
void WPFCppCliDemo::setServerstack_TS()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();hRow1Def->Height = GridLength(30);
	hTextSerachGrid->RowDefinitions->Add(hRow1Def);

	RowDefinition^ hRow2Def = gcnew RowDefinition();hRow2Def->Height = GridLength(30);
	hTextSerachGrid->RowDefinitions->Add(hRow2Def);

	checkBox_TS1->Width = 20;checkBox_TS1->Height = 20;
	hStackPanel_TS3->Children->Add(checkBox_TS1);

	TextBlock^ Srver_no = gcnew TextBlock();Srver_no->Width = 100;	Srver_no->Height = 20;	Srver_no->Text = "Server Address 1:";
	hStackPanel_TS3->Children->Add(Srver_no);

	checkBox_TS2->Width = 20;checkBox_TS2->Height = 20;
	hStackPanel_TS4->Children->Add(checkBox_TS2);

	TextBlock^ Srver_no1 = gcnew TextBlock();Srver_no1->Width = 100;Srver_no1->Height = 20;Srver_no1->Text = "Server Address 2:";
	hStackPanel_TS4->Children->Add(Srver_no1);

	textbox_TS1->FontSize = 12;textbox_TS1->Height = 20;textbox_TS1->Width = 200;
	hStackPanel_TS3->Children->Add(textbox_TS1);

	textbox_TS2->FontSize = 12;textbox_TS2->Height = 20;textbox_TS2->Width = 200;
	hStackPanel_TS4->Children->Add(textbox_TS2);

	hStackPanel_TS3->Orientation = Orientation::Horizontal;	hStackPanel_TS3->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;

	hStackPanel_TS4->Orientation = Orientation::Horizontal;hStackPanel_TS4->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;

	hTextSerachGrid->SetRow(hStackPanel_TS3, 0);hTextSerachGrid->Children->Add(hStackPanel_TS3);
	hTextSerachGrid->SetRow(hStackPanel_TS4, 1);hTextSerachGrid->Children->Add(hStackPanel_TS4);
}

//----< set up list properties of text search view for the UI applocation >------------------------------------
void WPFCppCliDemo::setListPropertiesTS()
{
	hTextSerachGrid->Margin = Thickness(20);
	hTextSearchTab->Content = hTextSerachGrid;
	RowDefinition^ hRowFS1Def = gcnew RowDefinition();
	//hRowU1Def->Height = GridLength(400);
	hTextSerachGrid->RowDefinitions->Add(hRowFS1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hListBox_TS;
	hTextSerachGrid->SetRow(hBorder1, 4);
	hTextSerachGrid->Children->Add(hBorder1);
}

//----< set up lisr properties of file search view for the UI applocation >------------------------------------
void WPFCppCliDemo::setTextBlockPropertiesTS()
{
	RowDefinition^ hRowFS1Def = gcnew RowDefinition();
	hRowFS1Def->Height = GridLength(50);
	hTextSerachGrid->RowDefinitions->Add(hRowFS1Def);

	hAddPatternBox_TS->FontSize = 12;
	hAddPatternBox_TS->Height = 20;
	hAddPatternBox_TS->Width = 250;
	hStackPanel_TS1->Children->Add(hAddPatternBox_TS);

	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 180;
	hStackPanel_TS1->Children->Add(hSpacer);

	TextBlock^ Label_TextSearch = gcnew TextBlock();
	Label_TextSearch->Text = "Search Text:";
	Label_TextSearch->Height = 20;
	Label_TextSearch->Width = 70;
	hStackPanel_TS1->Children->Add(Label_TextSearch);

	hTestBox->FontSize = 12;
	hTestBox->Height = 20;
	hTestBox->Width = 200;
	hStackPanel_TS1->Children->Add(hTestBox);

	hStackPanel_TS1->Orientation = Orientation::Horizontal;
	hStackPanel_TS1->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hTextSerachGrid->SetRow(hStackPanel_TS1, 2);
	hTextSerachGrid->Children->Add(hStackPanel_TS1);
}

//----< set up button properties of text search view for the UI applocation >------------------------------------
void WPFCppCliDemo::setButtonsPropertiesTS()
{
	RowDefinition^ hRowFSDef = gcnew RowDefinition();
	hRowFSDef->Height = GridLength(50);
	hTextSerachGrid->RowDefinitions->Add(hRowFSDef);

	hAddPattern_TS->Content = "Add Pattern";
	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 120;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hAddPattern_TS;
	hStackPanel_TS2->Children->Add(hBorder3);

	TextBlock^ hSpacer1 = gcnew TextBlock();
	hSpacer1->Width = 40;
	hStackPanel_TS2->Children->Add(hSpacer1);

	AddedPatt_TS->Height = 30;
	AddedPatt_TS->Width = 360;
	AddedPatt_TS->Text = AddedPatterns_TS;
	hStackPanel_TS2->Children->Add(AddedPatt_TS);



	hSerach_TS->Content = "Search";
	Border^ hBorder4 = gcnew Border();
	hBorder4->Width = 120;
	hBorder4->Height = 30;
	hBorder4->BorderThickness = Thickness(1);
	hBorder4->BorderBrush = Brushes::Black;
	hBorder4->Child = hSerach_TS;
	hStackPanel_TS2->Children->Add(hBorder4);

	hStackPanel_TS2->Orientation = Orientation::Horizontal;
	hStackPanel_TS2->HorizontalAlignment = System::Windows::HorizontalAlignment::Left;
	hTextSerachGrid->SetRow(hStackPanel_TS2, 3);
	hTextSerachGrid->Children->Add(hStackPanel_TS2);
}

//----< set up send message properties of the UI applocation >------------------------------------
void WPFCppCliDemo::setUpSendMessageView()
{
	Console::Write("\n  setting up sendMessage view");
	hSendMessageGrid->Margin = Thickness(20);
	hSendMessageTab->Content = hSendMessageGrid;

	setTextBlockProperties();
	setButtonsProperties();
}

//----< set up lisr properties of file search view for the UI applocation >------------------------------------
void WPFCppCliDemo::setTextBlockProperties()
{
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	hSendMessageGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hTextBlock1;
	hTextBlock1->Padding = Thickness(15);
	hTextBlock1->Text = "";
	hTextBlock1->FontFamily = gcnew Windows::Media::FontFamily("Tahoma");
	hTextBlock1->FontWeight = FontWeights::Bold;
	hTextBlock1->FontSize = 16;
	hScrollViewer1->VerticalScrollBarVisibility = ScrollBarVisibility::Auto;
	hScrollViewer1->Content = hBorder1;
	hSendMessageGrid->SetRow(hScrollViewer1, 0);
	hSendMessageGrid->Children->Add(hScrollViewer1);
}

//----< set up button properties of file search view for the UI applocation >------------------------------------
void WPFCppCliDemo::setButtonsProperties()
{
	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(75);
	hSendMessageGrid->RowDefinitions->Add(hRow2Def);
	hSendButton->Content = "Send Message";
	Border^ hBorder2 = gcnew Border();
	hBorder2->Width = 120;
	hBorder2->Height = 30;
	hBorder2->BorderThickness = Thickness(1);
	hBorder2->BorderBrush = Brushes::Black;
	hClearButton->Content = "Clear";
	hBorder2->Child = hSendButton;
	Border^ hBorder3 = gcnew Border();
	hBorder3->Width = 120;
	hBorder3->Height = 30;
	hBorder3->BorderThickness = Thickness(1);
	hBorder3->BorderBrush = Brushes::Black;
	hBorder3->Child = hClearButton;
	hStackPanel1->Children->Add(hBorder2);
	TextBlock^ hSpacer = gcnew TextBlock();
	hSpacer->Width = 10;
	hStackPanel1->Children->Add(hSpacer);
	hStackPanel1->Children->Add(hBorder3);
	hStackPanel1->Orientation = Orientation::Horizontal;
	hStackPanel1->HorizontalAlignment = System::Windows::HorizontalAlignment::Center;
	hSendMessageGrid->SetRow(hStackPanel1, 1);
	hSendMessageGrid->Children->Add(hStackPanel1);
}

//----< set up filelist view view for the UI applocation >------------------------------------
void WPFCppCliDemo::setUpFileListView()
{
	Console::Write("\n  setting up FileList view");
	hFileListGrid->Margin = Thickness(20);
	hFileListTab->Content = hFileListGrid;
	RowDefinition^ hRow1Def = gcnew RowDefinition();
	//hRow1Def->Height = GridLength(75);
	hFileListGrid->RowDefinitions->Add(hRow1Def);
	Border^ hBorder1 = gcnew Border();
	hBorder1->BorderThickness = Thickness(1);
	hBorder1->BorderBrush = Brushes::Black;
	hBorder1->Child = hListBox;
	hFileListGrid->SetRow(hBorder1, 0);
	hFileListGrid->Children->Add(hBorder1);

	RowDefinition^ hRow2Def = gcnew RowDefinition();
	hRow2Def->Height = GridLength(75);
	hFileListGrid->RowDefinitions->Add(hRow2Def);
	hFolderBrowseButton->Content = "Select Directory";
	hFolderBrowseButton->Height = 30;
	hFolderBrowseButton->Width = 120;
	hFolderBrowseButton->BorderThickness = Thickness(2);
	hFolderBrowseButton->BorderBrush = Brushes::Black;
	hFileListGrid->SetRow(hFolderBrowseButton, 1);
	hFileListGrid->Children->Add(hFolderBrowseButton);

	hFolderBrowserDialog->ShowNewFolderButton = false;
	hFolderBrowserDialog->SelectedPath = System::IO::Directory::GetCurrentDirectory();
}

//----< set up connect view for the UI applocation >------------------------------------
void WPFCppCliDemo::Connect(Object^ sender, RoutedEventArgs^ args)
{
	std::cout << "\n  Connection requested";
	hStatus->Text = "Connecting..."; 
	if (!textbox_server->Text->IsNullOrEmpty(textbox_server->Text) && !textbox_port->Text->IsNullOrEmpty(textbox_port->Text))
	{
		hStatus->Text = "Sent message";
		hStatus->Text = toSystemString(pRecvr1_->getMessage());
	}
	else hStatus->Text = "Address and port fields cannot be empty";
}

//----< set up disconnect view for the UI applocation >------------------------------------
void WPFCppCliDemo::Disconnect(Object^ sender, RoutedEventArgs^ args)
{
	//std::cout << "\n  Disconnect requested";
	//pSendr1_->postMessage(toStdString("DISCONNECT " + serveraddress));
	textbox_server->Clear();
	textbox_port->Clear();
	hStatus->Text = "Disconnecting...";// +":";// textbox_server + ":"+textbox_port;
	hStatus->Text = toSystemString(pRecvr1_->getMessage());
}

//----< set up browse folder properties of upload view for the UI applocation >------------------------------------
void WPFCppCliDemo::browseForFolder_U(Object^ sender, RoutedEventArgs^ args)
{
	//std::cout << "\n  Browsing for folder";
	hListBox_U->Items->Clear();
	System::Windows::Forms::DialogResult result;
	result = hFolderBrowserDialog_U->ShowDialog();
	if (result == System::Windows::Forms::DialogResult::OK)
	{
		String^ path = hFolderBrowserDialog_U->SelectedPath;
		//std::cout << "\n  opening folder \"" << toStdString(path) << "\"";
		array<String^>^ files = System::IO::Directory::GetFiles(path, L"*.*");
		for (int i = 0; i < files->Length; ++i)
			hListBox_U->Items->Add(files[i]);
		array<String^>^ dirs = System::IO::Directory::GetDirectories(path);
		for (int i = 0; i < dirs->Length; ++i)
			hListBox_U->Items->Add(L"<> " + dirs[i]);
	}
}

//----< definition upload  properties of file search view for the UI applocation >------------------------------------
void WPFCppCliDemo::UploadFile_U(Object^ sender, RoutedEventArgs^ args)
{
	//Console::Write("\n\nSelected File:" + hListBox_U->SelectedValue);
	if ((bool)checkBox_U1->IsChecked == true || (bool)checkBox_U2->IsChecked == true)
	{
		int i = 0; String^ address = "";
		if ((bool)checkBox_U1->IsChecked == true && !textbox_U1->Text->IsNullOrEmpty(textbox_U1->Text)){ address = serveraddress1 = textbox_U1->Text; ++i; }
		else { if (textbox_U1->Text->IsNullOrEmpty(textbox_U1->Text))hStatus->Text = "Checkned box!"; }
		if ((bool)checkBox_U2->IsChecked == true && !textbox_U2->Text->IsNullOrEmpty(textbox_U2->Text)){
			address = address + (" ");
			address += serveraddress2 = textbox_U2->Text; ++i;
		}
		else { if (textbox_U2->Text->IsNullOrEmpty(textbox_U2->Text))hStatus->Text = "Checkned box!"; }String^ str = (String^)hListBox_U->SelectedValue; 
		if ((int)address->Length > 0 && (!str->IsNullOrEmpty(str)  )){
			std::string strN = toStdString(str); if (!(strN.find("<>") == 1)){
				pSendr1_->postMessage("UPLOAD " + std::to_string(i) + " " + toStdString(address) + " " + toStdString((String^)(hListBox_U->SelectedValue)) + " " + toStdString(DirectorycomboBox_U->SelectedValue->ToString()));// toStdString(DirectorycomboBox_U->SelectedValue->ToString()) + "/" +
				hStatus->Text = toSystemString(pRecvr1_->getMessage());
			}else hStatus->Text = "Please select valid file from the list!";
		}
		else {if (!((int)address->Length > 0))
				hStatus->Text = "Please enter valid server address, like localhost:9000(ip address followed by port)";
			else hStatus->Text = "Please select valid file from the list!";
		}
	}
	else{ hStatus->Text = "Setelct atleast one server!"; }
}

//----< definition dowload  for the UI applocation >------------------------------------
void WPFCppCliDemo::Download(Object^ sender, RoutedEventArgs^ args)
{
	//Console::Write("\n\nSelected File:" + hListBox_U->SelectedValue);
	if ((bool)checkBox_D1->IsChecked == true || (bool)checkBox_D2->IsChecked == true)
	{
		int i = 0; String^ address = "";
		if ((bool)checkBox_D1->IsChecked == true && !textbox_D1->Text->IsNullOrEmpty(textbox_D1->Text)){ address = serveraddress1 = textbox_D1->Text; ++i; }
		else { if (textbox_D1->Text->IsNullOrEmpty(textbox_U1->Text))hStatus->Text = "Checkned box!"; }
		if ((bool)checkBox_D2->IsChecked == true && !textbox_D2->Text->IsNullOrEmpty(textbox_D2->Text)){
			address = address + (" ");
			address += serveraddress2 = textbox_D2->Text; ++i;
		}
		else { if (textbox_D2->Text->IsNullOrEmpty(textbox_D2->Text))hStatus->Text = "Checkned box!"; }
		if ((int)address->Length > 0 && !(hTextBox_D->Text->IsNullOrEmpty(hTextBox_D->Text))){
			pSendr1_->postMessage("DOWNLOAD " + std::to_string(i) + " " + toStdString(address) + " " + toStdString((String^)(hTextBox_D->Text)) + " " + toStdString(DirectorycomboBox_D->SelectedValue->ToString()));
			hStatus->Text = toSystemString(pRecvr1_->getMessage());
		}
		else hStatus->Text = "Checked server address  and filename fields has tobe filled!";
	}
	else{ hStatus->Text = "Setelct atleast one server!"; }

}

//----< added patterns functionality for the UI applocation >------------------------------------
void WPFCppCliDemo::AddPattern(Object^ obj, RoutedEventArgs^ args)
{

}

//----< added patterns functionality for the UI applocation >------------------------------------
void WPFCppCliDemo::AddPattern_TS(Object^ obj, RoutedEventArgs^ args)
{
	//std::cout << "\n  Add Text requested";
	if (!hAddPatternBox_TS->Text->IsNullOrEmpty(hAddPatternBox_TS->Text))
	{
		pSendr1_->postMessage("ADDPATTERN " + toStdString(hAddPatternBox_TS->Text));
		AddedPatterns_TS += hAddPatternBox_TS->Text;
		AddedPatt_TS->Text = AddedPatterns_TS;
		hStatus->Text = toSystemString(pRecvr1_->getMessage());
		hAddPatternBox_TS->Clear();
	}
	else hStatus->Text = "Pattern cannot be empty";
}

//----< Search text functionality for the UI applocation >------------------------------------
void WPFCppCliDemo::Serach_TS(Object^ obj, RoutedEventArgs^ args)
{
	hListBox_TS->Items->Clear();
	AddedPatt_TS->Text = AddedPatterns_TS;
	if ((bool)checkBox_TS1->IsChecked == true || (bool)checkBox_TS2->IsChecked == true)
	{
		int i = 0; String^ address = "";
		if ((bool)checkBox_TS1->IsChecked == true && !textbox_TS1->Text->IsNullOrEmpty(textbox_TS1->Text)){ address = serveraddress1 = textbox_TS1->Text; ++i; }
		//else {}// if (textbox_TS1->Text->IsNullOrEmpty(textbox_TS2->Text))hStatus->Text = "Checkned box!"; }
		if (((bool)checkBox_TS1->IsChecked == true && !textbox_TS1->Text->IsNullOrEmpty(textbox_TS1->Text)) && (bool)checkBox_TS2->IsChecked == true && !textbox_TS2->Text->IsNullOrEmpty(textbox_TS2->Text))address = address + (" ");
		if ((bool)checkBox_TS2->IsChecked == true && !textbox_TS2->Text->IsNullOrEmpty(textbox_TS2->Text)){
			address += serveraddress2 = textbox_TS2->Text; ++i;
		}
		//else {}//if (textbox_TS2->Text->IsNullOrEmpty(textbox_TS2->Text))hStatus->Text = "Checkned box!"; }
		if ((int)address->Length > 0 && !hTestBox->Text->IsNullOrEmpty(hTestBox->Text)){
			if (!hTestBox->Text->IsNullOrEmpty(hTestBox->Text))
			{
				String^ textsearch = hTestBox->Text;
				pSendr1_->postMessage("SEARCHTEXT " + std::to_string(i) + " " + toStdString(address) + " " + toStdString(hTestBox->Text));hTestBox->Clear();
				while (true)
				{
					AddedPatterns_TS = "Added Patterns: ";
					std::string str = pRecvr1_->getMessage(); if (str == "END")break;
					hListBox_TS->Items->Add(toSystemString(str));
				}
				hStatus->Text = toSystemString(pRecvr1_->getMessage()) + "\n Note: Add Patterns resets for every search";
			}
			else hStatus->Text = "Search text cannot be empty";
		}
		else hStatus->Text = "Checked server address and search text has tobe specified in the respective field!";
	}
	else{ hStatus->Text = "Setelct atleast one server!"; }
}

//----< Add pattern functionality for file search the UI applocation >------------------------------------
void WPFCppCliDemo::AddPattern_FS(Object^ obj, RoutedEventArgs^ args)
{
	//std::cout << "\n  Add Text requested";
	if (!hAddPatternBox->Text->IsNullOrEmpty(hAddPatternBox->Text))
	{
		pSendr1_->postMessage("ADDPATTERN " + toStdString(hAddPatternBox->Text));
		AddedPatterns_FS += hAddPatternBox->Text;
		AddedPatt->Text = AddedPatterns_FS;
		hStatus->Text = toSystemString(pRecvr1_->getMessage());
		hAddPatternBox->Clear();
	}
	else hStatus->Text = "Pattern cannot be empty";
}

//----< Search functionality for file search in the UI application >------------------------------------
void WPFCppCliDemo::Serach_FS(Object^ obj, RoutedEventArgs^ args)
{
	hListBox_FS->Items->Clear();
	AddedPatt->Text = AddedPatterns_FS;
	//std::cout << "\n  Serach Text requested";
	if ((bool)checkBox_FS1->IsChecked == true || (bool)checkBox_FS2->IsChecked == true)
	{
		int i = 0; String^ address = "";
		if ((bool)checkBox_FS1->IsChecked == true && !textbox_FS1->Text->IsNullOrEmpty(textbox_FS1->Text)){ address = serveraddress1 = textbox_FS1->Text; ++i; }
		//else { if (textbox_FS1->Text->IsNullOrEmpty(textbox_FS1->Text))hStatus->Text = "Checkned box!"; }
		if (((bool)checkBox_FS1->IsChecked == true && !textbox_FS1->Text->IsNullOrEmpty(textbox_FS1->Text)) && ((bool)checkBox_FS2->IsChecked == true && !textbox_FS2->Text->IsNullOrEmpty(textbox_FS2->Text)))address = address + (" ");
		if ((bool)checkBox_FS2->IsChecked == true && !textbox_FS2->Text->IsNullOrEmpty(textbox_FS2->Text)){
			//address = address + (" ");
			address += serveraddress2 = textbox_FS2->Text; ++i;
		}
		//else { if (textbox_FS2->Text->IsNullOrEmpty(textbox_FS2->Text))hStatus->Text = "Checkned box!"; }
		if ((int)address->Length > 0 ){
			String^ textsearch = hTestBox->Text;
			pSendr1_->postMessage("SEARCHFILE " + std::to_string(i) + " " + toStdString(address) + " " + toStdString(DirectorycomboBox1->SelectedValue->ToString()));
			while (true)
			{
				AddedPatterns_FS = "Added Patterns: ";
				std::string str = pRecvr1_->getMessage(); if (str == "END")break;
				hListBox_FS->Items->Add(toSystemString(str));
			}
			hStatus->Text = toSystemString(pRecvr1_->getMessage()) + "\n Note: Add Patterns resets for every search";
		}
		else hStatus->Text = "Checked server address has tobe specified in the address field!";
	}
	else{ hStatus->Text = "Setelct atleast one server!"; }
	
}

//----< Send message functionality for message send in the UI application >------------------------------------
void WPFCppCliDemo::sendMessage(Object^ obj, RoutedEventArgs^ args)
{
	//pSendr1_->postMessage(toStdString(msgText));
	////Console::Write("\n  sent message");
	//hStatus->Text = "Sent message";
}

//----< Clear functionality for send message in the UI application >------------------------------------
void WPFCppCliDemo::clear(Object^ sender, RoutedEventArgs^ args)
{
	//Console::Write("\n  cleared message text");
	hStatus->Text = "Cleared message";
	hTextBlock1->Text = "";
}

//----< browse functionality for file browse in the UI application >------------------------------------
void WPFCppCliDemo::browseForFolder(Object^ sender, RoutedEventArgs^ args)
{
	//std::cout << "\n  Browsing for folder";
	hListBox->Items->Clear();
	System::Windows::Forms::DialogResult result;
	result = hFolderBrowserDialog->ShowDialog();
	if (result == System::Windows::Forms::DialogResult::OK)
	{
		String^ path = hFolderBrowserDialog->SelectedPath;
		//std::cout << "\n  opening folder \"" << toStdString(path) << "\"";
		array<String^>^ files = System::IO::Directory::GetFiles(path, L"*.*");
		for (int i = 0; i < files->Length; ++i)
			hListBox->Items->Add(files[i]);
		array<String^>^ dirs = System::IO::Directory::GetDirectories(path);
		for (int i = 0; i < dirs->Length; ++i)
			hListBox->Items->Add(L"<> " + dirs[i]);
	}
}

//----< Add text functionality for file search in the UI application >------------------------------------
void WPFCppCliDemo::AddText(Object^ obj, RoutedEventArgs^ args)
{
	/*std::cout << "\n  Add Text requested";
	if (!->Text->IsNullOrEmpty(textbox_server->Text))
	{
		pSendr1_->postMessage("ADDTEST " + toStdString(textbox_server->Text));
		hStatus->Text = "Given Search test added!";
	}
	else hStatus->Text = "Search test cannot be empty";*/
}					///NA

//----< Search functionality for file search in the UI application >------------------------------------
void WPFCppCliDemo::Serach(Object^ obj, RoutedEventArgs^ args)
{

}

//----< Converts String^ to std::string >------------------------------------
std::string WPFCppCliDemo::toStdString(String^ pStr)
{
  std::string dst;
  for (int i = 0; i < pStr->Length; ++i)
    dst += (char)pStr[i];
  return dst;
}

//----< Converts std::string to String^ >------------------------------------
String^ WPFCppCliDemo::toSystemString(std::string& str)
{
  StringBuilder^ pStr = gcnew StringBuilder();
  for (size_t i = 0; i < str.size(); ++i)
    pStr->Append((Char)str[i]);
  return pStr->ToString();
}

//----< Adds text to send messsage screen >------------------------------------
void WPFCppCliDemo::addText(String^ msg)
{
  hTextBlock1->Text += msg + "\n\n";
}

//----< DeQueues the message from in the UI application >------------------------------------
void WPFCppCliDemo::getMessage()
{
  // recvThread runs this function

  //while (true)
  //{
  //  std::cout << "\n  receive thread calling getMessage()";
  //  std::string msg = pRecvr_->getMessage();
  //  String^ sMsg = toSystemString(msg);
  //  array<String^>^ args = gcnew array<String^>(1);
  //  args[0] = sMsg;

  //  Action<String^>^ act = gcnew Action<String^>(this, &WPFCppCliDemo::addText);
  //  Dispatcher->Invoke(act, args);  // must call addText on main UI thread
  //}
}


















void WPFCppCliDemo::OnLoaded(Object^ sender, RoutedEventArgs^ args)
{
  Console::Write("\n  Window loaded");
}
void WPFCppCliDemo::Unloading(Object^ sender, System::ComponentModel::CancelEventArgs^ args)
{
  Console::Write("\n  Window closing");
}

[STAThread]
//int _stdcall WinMain()
int main(array<System::String^>^ args)
{
	Console::WriteLine(L"\n Starting WPFCppCliDemo");

	Application^ app = gcnew Application();
	
	app->Run(gcnew WPFCppCliDemo(args[0], args[1]));
	Console::WriteLine(L"\n\n");
}