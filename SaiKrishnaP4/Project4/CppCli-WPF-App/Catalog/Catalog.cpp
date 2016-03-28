/////////////////////////////////////////////////////////////////////////////
// Catalog.cpp - To process input text by user and search text in datastore//
//				 files													   //
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

#include "Catalog.h"
#include <iostream>

//----< processes the text given by user and calls search text function with appropriate extensions and filenames >------------------------------------

DataStore Catalog::text_process(std::string input_text){
	int flag = 0;
	int flag_case = 1;                               //used to distinguish call from filemanager and text_process
	std::string search_txt, sub1, sub2;
	std::size_t pos;
	std::vector<std::string>::iterator itr_patt;
	if (input_text.find("\"") == 0){				//Search text is saperated if given in "quotes"
		int i = input_text.find("\"", 1);
		sub1 = input_text.substr(1, i - 1);
		int len = input_text.length();
		sub2 = input_text.substr(i + 1, len);
		search_txt = sub1;
		flag = 1;
	}
	else{ sub2 = input_text; }
	std::istringstream iss(sub2);
	do{
		std::string sub;                          //search text is filtered if the text is only one word
		iss >> sub;
		if (flag == 0){
			search_txt = sub;
			flag = 1;
		}
		else{
			if (sub.find("*") == 0){			//storing valit extensions 
				if (sub.size() != 0){
					pos = sub.find(".");
					sub = sub.substr(pos + 1);
					if (sub == "*")flag_case = 0;
					patt.push_back(sub);
				}
			}
			else{
				if (int(sub.find(".")) > 0)		//To store filename that may contain text
					patt.push_back(sub);
			}
		}
	} while (iss);
	if (patt.size() == 0)
		flag_case = 0;
	return search_text(search_txt, flag_case);
}

//----< searches for the given text and also makes sure that the search text is present in the given extension and filename >------------------------------------

DataStore Catalog::search_text(std::string s_text, int flag_case){
	std::string text;
	std::vector<std::string>::iterator itr_patt;
	std::string filename;
	DataStore datastore2;
	itr_pathset itr_pathset1;
	reflist::iterator itr_reflist1;
	itr_filemap itr_filemap1;
	reflist reflist1;
	for (itr_filemap1 = datastore1.begin_map(); itr_filemap1 != datastore1.end_map(); itr_filemap1++){
		filename = itr_filemap1->first;
		reflist1 = itr_filemap1->second;
		reflist reflist2;
		int flag = 0;
		for (auto itr_patt : patt){											//when the mathced files needed to be filtered
			if (itr_patt == FileSystem::Path::getExt(filename) || itr_patt == filename)
			flag = 1;
		}			//Generate new DataStore
		if (flag_case == 0 || flag == 1){
			for (itr_reflist1 = reflist1.begin(); itr_reflist1 != reflist1.end(); ++itr_reflist1){
				FileSystem::Directory::setCurrentDirectory(*(*itr_reflist1));
				FileSystem::File fl(filename);
				if (fl.open(FileSystem::File::in, FileSystem::File::text)){              //open file and read contents into a string
					text = fl.readAll(false);
					if (text.find(s_text) != std::string::npos){
						reflist2.push_back(*itr_reflist1);					// when string matches copy the reference
					}
				}
			}
			if (reflist2.size() >= 1){
				datastore2.save_map(filename, reflist2);								//when no constraint to filter the matched files
				
			}
		}
	}
	
	return datastore2;
}

//----< test stub >---------------------------------------------------------------------------------------------------------------------------

#ifdef TEST_CATALOG

int main()
{
	DataStore ds;
	FileMgr fm(".",ds);							 
	fm.search();								//including only the files present in the current path
	Catalog cat(ds);							
	cat.search_text("Catalog", 0);				//search Catalog of all the patterns
	std::string str = "Catalog.h *.cpp";
	cat.text_process(str);						//search catalog with specific input text and extensions
}
#endif