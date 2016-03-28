#ifndef DATASTORE_H
#define DATASTORE_H
/////////////////////////////////////////////////////////////////////////////
// DataStore.h - To Store files and corresponding paths                    //
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
* This module provides DataStore class that stores filename and path in a map and set
*
* Public Interface:
* =================
*save_path(const path file_path) member function to store path in a set 
*
*save_map(const filename& filename1, const reflist reflist1) member function to store map of filename and reference of paths stored in the set
*
*begin_path()--member fuctiion returns an iterator pointing to begnning of the pathset
*
*end_path()--member fuctiion returns an iterator pointing to end of the pathset
*
*begin_map()--member fuctiion returns an iterator pointing to begnning of the map
*
*end_map()--member fuctiion returns an iterator pointing to end of the map
*
* Required Files:
* ===============
* DataStore.h and DataStore.cpp
*
* Build Command:
* ==============
*  devenv CppCli-WPF-App_original.sln /rebuild debug
*
* Maintenance History:
* ====================
* ver 1.0 Jan 30 2015
*
*/
#include<iostream>
#include<set>
#include<map>
#include<string>
#include <list>
#include <iomanip>

class DataStore
{
public:
	using filename = std::string;
	using path = std::string;
	using pathset = std::set < path >;
	using itr_pathset = pathset::iterator;
	using reflist = std::list < itr_pathset >;
	using filemap = std::map < filename, reflist>;
	using itr_filemap=filemap::iterator;

	//itr_pathset itr_pathset1;
	//reflist::iterator itr_reflist1;
	//itr_filemap itr_filemap1;
	//reflist reflist1;

	//----< To save path to pathset >--------------------------------------------------------

	inline void save_path(const path file_path){ pathset1.insert(file_path); }

	//----< To save map to filemap >--------------------------------------------------------

	inline void save_map(const filename& filename1, const reflist reflist1){ filemap1.insert(make_pair(filename1, reflist1)); }

	//----< To get iterator that points to the beginning of pathset >--------------------------------------------------------

	inline itr_pathset begin_path(){ return pathset1.begin(); }

	//----< To get iterator that points to the end of pathset >--------------------------------------------------------

	inline itr_pathset end_path(){ return pathset1.end(); }

	//----< To get iterator that points to the beginning of pathset >--------------------------------------------------------

	inline itr_pathset path_find(std::string path){ return pathset1.find(path); }
	
	//----< To get iterator that points to the begnning  of filemap >--------------------------------------------------------

	inline filemap::iterator begin_map(){ return filemap1.begin(); }
	
	//----< To get iterator that points to the the search text of key in filemap >--------------------------------------------------------

	inline itr_filemap filemap_find(std::string filename){ return filemap1.find(filename); }

	//----< To get iterator that points to the end of filemap >--------------------------------------------------------

	inline filemap::iterator end_map(){ return filemap1.end(); }
private:
	filemap filemap1;
	pathset pathset1;
};

#endif
