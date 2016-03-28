#ifndef EXECUTIVE_H
#define EXECUTIVE_H
//File Manager..

#include "../FileSystem/FileSystem.h"
#include "../Project1/DataStore.h"
#include "../FileMgr/FileMgr.h"

class Exec
{
	void display_full();
	void display_duplicate();
	Exec(int i,std::string* arg)
	{
		fm.initalize(i,arg);
	}
private:
	FileMgr fm;

};
#endif