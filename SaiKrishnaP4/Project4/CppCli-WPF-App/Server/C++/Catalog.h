#ifndef CATALOG_H
#define CATALOG_H
/////////////////////////////////////////////////////////////////////////////
// Catalog.h - To process the textsearch entered and extracts files which  //
//	                has the text match and store						   //
//																		   //
// ver 1.0                                                                 //
// ----------------------------------------------------------------------- //
// copyright © Sai Krishna Bandaru, 2015                                   //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2013                             //
// Platform:    ASUS SonicMaster, Core i3, Windows 8.1                     //
// Application: Project 1, 2015		                                       //
// Author:      Sai Krishna,Syracuse University					           //
//              (832) 940-8083, sbandaru@syr.edu                           //
/////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
* This module is used to process the textsearch entered and extracts files which has the text match and store.
*
* Public Interface:
* =================
*search_text() function is used to find the files that matches text search and generates a datastore that matches the text search
*
*text_process() function is used to process the text entered by the user through which may contain search text, file extensions and file names
*
* Required Files:
* ===============
* DataStore.h ,Display.h ,Display.cpp, FileMgr.h, FileMgr.cpp, Catalog.h and Catalog.cpp
*
* Build Command:
* ==============
* cl /EHsc /DTEST_CATALOG Catalog.cpp ../FileMgr/FileMgr.cpp ../Display/Display.cpp ../Project1/DataStore.cpp ../FileSystem/FileSystem.cpp
*
* Maintenance History:
* ====================
* ver 1.0 Jan 30 2015
*
*/

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include "../FileSystem/FileSystem.h"
#include "../Project1/DataStore.h"

#include "../FileMgr/FileMgr.h"

class Catalog
{
	public:
		using filename = std::string;
		using path = std::string;
		using pathset = std::set < path >;
		using itr_pathset = pathset::iterator;
		using reflist = std::list < itr_pathset >;
		using filemap = std::map < filename, reflist>;
		using itr_filemap = filemap::iterator;
		
		itr_pathset itr_pathset1;
		reflist::iterator itr_reflist1;
		itr_filemap itr_filemap1;
		reflist reflist1;

		Catalog(DataStore& ds) :datastore1(ds){};
		DataStore text_process(std::string);
		DataStore search_text(std::string, int);
			
	private:
		DataStore& datastore1;
		std::vector<std::string> patt;
};

#endif