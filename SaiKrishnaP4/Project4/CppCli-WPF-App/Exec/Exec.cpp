/////////////////////////////////////////////////////////////////////////////
// Exec.cpp - This is the module that interacts with the user through i/o  //
// ver 1.0                                                                 //
// ----------------------------------------------------------------------- //
// copyright © Sai Krishna Bandaru, 2015                                   //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2013                             //
// Platform:    ASUS SonicMaster, Core i3, Windows 8.1                     //
// Application: Project 1, 2015		                                       //
// Author:      Sai Krishna,Syracuse University					           //
//              (832) 940-8083, sbandaru@syr.edu			               //
/////////////////////////////////////////////////////////////////////////////
/*
* Build Command :
*== == == == == == ==
cl / EHsc / DEXEC Exec.cpp .. / Display / Display.cpp .. / Project1 / DataStore.cpp .. / FileMgr / FileMgr.cpp .. / FileSystem / FileSystem.cpp .. / Catalog / Catalog.cpp
*
*/
#include <iostream>
#include <string>
#include "../FileMgr/FileMgr.h"
#include "../Display/Display.h"
#include "../Catalog/Catalog.h"
#include "../FileSystem/FileSystem.h"
#include "../Project1/DataStore.h"

#ifdef EXEC
int main(int argc, char* argv[])      //we pass command line arguments
{
	//try{
	//	std::string input_text, str;
	//	DataStore ds;
	//	FileMgr fm(".", ds);
	//	Display::display_args(argc, argv);
	//	Display::display_text("\t\t\t\tCatalog");
	//	fm.initilize(argc, argv);		//process command line arguments ,built catalog and prints the output to output. This internally calls
	//	do{																//to do multiple text search(s) until user opts to exit!
	//		str = "Enter the search text OR hit ENTER to exit";
	//		Display::display_text(str);
	//		std::getline(std::cin, input_text);
	//		if (input_text.size()){
	//			Catalog cs(ds);
	//			cs.text_process(input_text);
	//		}
	//	} while (input_text.size());
	//}
	//catch (std::exception e) {
	//	std::cout << e.what();
	//}

	DataStore ds;
	FileMgr fm(".", ds);
	std::vector<std::string> input; input.push_back("*.cpp");
	std::map<std::string, std::string>result= fm.SerachText("include", input);
	/*fm.Search();
	Catalog cs(ds);
	cs.text_process(std::string("\" \"  *.h"));*/
	
	return 0;
}

#endif