#ifndef FILEMANAGER_H
#define FILEMANAGER_H
/////////////////////////////////////////////////////////////////////////////
// FileMgr.h - To process command line arguments and generate catalog      //
// ver 1.1                                                                 //
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
* This module is used to process command line arguments and generate catalog and is stored in datastore
*
* Public Interface:
* =================
*initilize() function is used process command line arguments
*
*search() function is used to distinguish all file search and search in the current path for files while building catalog
*
*Search(bool) searches for text and file search depending up on the bool
*
*std::map<std::string, std::string> SerachText(std::string, std::vector<std::string>,bool) for search text in a folder than is passed 
*
*Private Member Functions
*=========================
*find_files() function is used to store file and path in the datastore
*
*addPattern() function is used to store the patterns that are used to generate catalog
*
*find_dir() function is used to search directories in the current directory
*
* Required Files:
* ===============
* DataStore.h ,Display.h , Catalog.h, Catalog.cpp, FileSystem.h, FileSyetm.cpp, FileMgr.h, FileMgr.cppand Display.cpp
*
* Build Command:
* ==============
*  devenv CppCli-WPF-App_original.sln /rebuild debug
*
* Maintenance History:
* ====================
* ver 1.1 Jan 30 2015 Built the updated datastore logic over professor's mock code along with handling command line input
*
*/

#include "../FileSystem/FileSystem.h"
#include "../Project1/DataStore.h"
#include "../Display/Display.h"
#include "../Catalog/Catalog.h"

class FileMgr
{
public:
	using filename = std::string;
	using path = std::string;
	using pathset = std::set < path >;
	using itr_pathset = pathset::iterator;
	using reflist = std::list < itr_pathset >;
	using filemap = std::map < filename, reflist>;
	using filemap1 = std::map < filename&, reflist>;
	using patterns = std::vector < std::string > ;

	//itr_pathset itr_pathset1;
	//reflist::iterator itr_reflist1;
	FileMgr(const std::string& path, DataStore& ds) : path_(path), DataStore1(ds)
	{
		patterns_.push_back("*.*");
	}

	void initilize(int argc, char* argv[]);
	void search(void);
	std::map<std::string, std::string> SearchResults(std::vector<std::string>, std::string);
	void Search(bool);
	std::map<std::string, std::string> SerachText(std::string, std::vector<std::string>,bool);
	
private:
	void addPattern(const std::string&);
	void find_files (const std::string &);
	void find_dir(std::string &);

	std::string path_;
	DataStore& DataStore1;
	patterns patterns_;
	int flag_fm = 0;					//fm for recursive directory search
	bool flip = 0;						//for setting absolute path
};

#endif