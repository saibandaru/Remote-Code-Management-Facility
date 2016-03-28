/////////////////////////////////////////////////////////////////////////////
// FileMgr.cpp - To mock test DataStore store functionallity               //
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

#include <iostream>
#include "FileMgr.h"


//----< To process the command line arguments and calls automatically builts catalog and displays --------------------------------------------------------

void FileMgr::initilize(int argc, char* argv[]){
	try{
		if (argc >= 2){
			int temp_int = 1, flag_d = 0, flag_f = 0;
			std::string temp, test_store;
			++argv;
			temp = *argv;
			if (temp.find('/') != 0 && (temp.find('*') == -1)){									 //to know if path is present at the starting
				path_ = temp;
				++argv;
				++temp_int;
			}
			for (int j = temp_int; j < argc; j++){												//looping through the extensions and filenames
				temp = *argv;
				if (temp.find('/') == -1 && temp.find('.') != -1){ addPattern(temp); }	        //saving the pattern
				else{
					if (temp == "/s" || temp == "/S"){ flag_fm = 1; }
					else if (temp == "/d" || temp == "/D"){ flag_d = 1; }						//identifing options
					else if ((temp == "/f" || temp == "/F") && j != argc - 1){
						++argv;
						test_store = (*argv);
						flag_f = 1;
						++j;
					}
					else{																	//ignoring invalid option
					}
				}
				++argv;
			}
			search();																			//building catalog
			if (flag_d == 1){  }									 //to display duplicates flag_d is set when /d option is given
			if (flag_f == 1){
				Catalog cat(DataStore1);														//to check for /f optoin 
				cat.search_text(test_store, 0);
			}
		}
		else{
			path_ = ".";
			search();
			/*Display dis(DataStore1);
			dis.display(3);*/																	//if no command line argument was given
		}
 }
	catch (std::exception e) {
		std::cout << e.what();
	}
}

//----< duplicate function for Searchtext() performs the same operation >--------------------------------------------------------

std::map<std::string, std::string> FileMgr::SearchResults(std::vector<std::string> patterns,std::string path)
{
	patterns_ = patterns; flag_fm = 1; path_ = path; search();
	int i = 0; std::map<std::string, std::string> returnResult;
	filemap::iterator itr_filemap1;
	pathset::iterator itr_pathset1 = (DataStore1.begin_path());
	for (itr_filemap1 = DataStore1.begin_map(); itr_filemap1 != DataStore1.end_map(); itr_filemap1++){
		reflist reflist_display = itr_filemap1->second;
		reflist::iterator itr_reflist_display;
		for (itr_reflist_display = reflist_display.begin(); itr_reflist_display != reflist_display.end(); itr_reflist_display++)
		{ 
			returnResult.insert(make_pair(itr_filemap1->first, (*(*itr_reflist_display))));
		}
	}
	return returnResult;
}

//----< searches for both file and text >--------------------------------------------------------

void  FileMgr::Search(bool file_text)
{
	if (file_text){ flag_fm = 0; search(); }
	else {
		flag_fm = 1;
		search();
		FileSystem::Directory::setCurrentDirectory("../");
	}
	
}

//----< searches for text/ files depending up on bool >--------------------------------------------------------

std::map<std::string, std::string> FileMgr::SerachText(std::string text_dir, std::vector<std::string> patterns,bool FileOrText)//File=true  Text=false
{
	std::string prev_path=FileSystem::Directory::getCurrentDirectory();    
	std::string Dir, input="" ;
	if (FileOrText){ Dir = text_dir; }
	else {
		Dir = "."; input = "\"" + text_dir + "\" ";
	}
	//std::cout << "\n\n\nSerach" << text_dir << "\n\n";
	for (auto itr : patterns)
		if (input == "")input = itr;else input = input + " " + itr;
	//std::cout << "\n\n\nSerach text" << input << "\n\n";
	
	DataStore ds;
	DataStore ds2;
	FileMgr fm(Dir, ds);
	if (FileOrText){ 
		for (auto itr : patterns)
			fm.addPattern(itr);
		fm.Search(true); ds2 = ds;
	}
	else {
		fm.Search(false);
		Catalog cs(ds);
		ds2 = cs.text_process(std::string(input));
	}
	std::map<std::string, std::string> returnResult;
	filemap::iterator itr_filemap1;
	pathset::iterator itr_pathset1 = (ds2.begin_path());
	for (itr_filemap1 = ds2.begin_map(); itr_filemap1 != ds2.end_map(); itr_filemap1++){
		reflist reflist_display = itr_filemap1->second;
		reflist::iterator itr_reflist_display;
		for (itr_reflist_display = reflist_display.begin(); itr_reflist_display != reflist_display.end(); itr_reflist_display++)
		{
			returnResult.insert(make_pair(itr_filemap1->first, (*(*itr_reflist_display))));
		}
	}
	FileSystem::Directory::setCurrentDirectory(prev_path);
	return returnResult;
}
//----< To store patterns that are to be used while building catalog >--------------------------------------------------------

void FileMgr::addPattern(const std::string& patt){
		if (patterns_.size() == 1 && patterns_[0] == "*.*"){
		patterns_.pop_back();
	}
	patterns_.push_back(patt);
}

//----< To distinguish recursive file search or normal search in the current directory >--------------------------------------------------------

void FileMgr::search(){	
	if (flag_fm == 1){											//For directory file search
		if (flip == 0 && path_ != "."){
			FileSystem::Directory::setCurrentDirectory(path_);
			flip = 1;
		}
		find_files(path_); 
		find_dir(path_);
	}
	else{														//non  directory file search
		if (flip == 0 && path_ != "."){
			bool path_set=FileSystem::Directory::setCurrentDirectory(path_);
			if (path_set == false)
				bool path_set2=FileSystem::Directory::setCurrentDirectory("../" + path_);
			flip = 1;
		}
		find_files(path_);
	}
}

//----< To implement recursive file search in current path >--------------------------------------------------------

void FileMgr::find_dir(std::string & path){
	std::string directory;
	std::string new_path, temp_path;
	std::vector<std::string> Dirs = FileSystem::Directory::getDirectories(path, "*.*");
	std::vector<std::string>::iterator itr_Dirs;
	temp_path = FileSystem::Directory::getCurrentDirectory();
	for (itr_Dirs = Dirs.begin(); itr_Dirs != Dirs.end(); itr_Dirs++){
		if (!(*itr_Dirs == "." || *itr_Dirs == "..")){							//ommit . and .. directories 
			directory = *itr_Dirs;
			new_path = temp_path + "/" + directory;
			FileSystem::Directory::setCurrentDirectory(new_path);
			path = new_path;
			search();
		}
		path = temp_path;
	}
}

//----< To search all the files in that current directory and store in the datastore >--------------------------------------------------------

void FileMgr::find_files(const std::string & path){
	itr_pathset  itr_pathset1;
	filemap::iterator itr_filemap;
	int flag = 0, flag_fn = 0;
	std::string temp_path,full_path = FileSystem::Directory::getCurrentDirectory();
	path_ = full_path;									
	for (auto patt : patterns_){
		flag = 0;
		flag_fn = 0;
		std::vector<std::string> files = FileSystem::Directory::getFiles(path, patt);
		std::vector<std::string>::iterator itr_filename;
		DataStore1.save_path(full_path);
		for (itr_filename = files.begin(); itr_filename != files.end(); itr_filename++){
			reflist reflist_push;
			itr_pathset1 = DataStore1.path_find(full_path);
			if (itr_pathset1 == DataStore1.end_path()){										//new path detected
				DataStore1.save_path(full_path);
				itr_filemap = DataStore1.filemap_find(*itr_filename);
				if (itr_filemap == DataStore1.end_map()){									//new file detected
					reflist_push.push_back(itr_pathset1);
					DataStore1.save_map((*itr_filename), reflist_push);
				}
				else{																		//existing file and new path
					(itr_filemap->second).push_back(itr_pathset1);
					DataStore1.save_map((*itr_filename), reflist_push);
				}
			}
			else{																			//path already existd
				itr_filemap = DataStore1.filemap_find(*itr_filename);
				if (itr_filemap == DataStore1.end_map()){									//new file detected
					reflist_push.push_back(itr_pathset1);
					DataStore1.save_map((*itr_filename), reflist_push);
				}
				else {																		//existing file and existing file
					(itr_filemap->second).push_back(itr_pathset1);
					DataStore1.save_map((*itr_filename), reflist_push);
				}
			}
		}
	}
}

//----< test stub >--------------------------------------------------------

#ifdef TEST_FILEMGR
int main(int argc, char* argv[])
{
	
	DataStore ds;
	FileMgr fm(".", ds);
	fm.initilize(argc, argv);
	return 0;
}

#endif