//FileManager.cpp

#include "../FileSystem/FileSystem.h"
#include "../Project1/DataStore.h"
#include <iostream>
#include "FileMgr.h"

int main()
{
	DataStore ds;
	FileMgr fm(".", ds);
	fm.addPattern("*.*");
	fm.search();
	fm.display_fm();
	//std::cout << "\n\n";

	return 0;
}