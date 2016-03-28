/////////////////////////////////////////////////////////////////////////////
// DataStore.cpp - To mock test DataStore store functionallity which stores//
//                  some random data and print those to console				/
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

#include "DataStore.h"
#include "../Display/Display.h"
#include <iostream>


//----< test stub >--------------------------------------------------------

#ifdef TEST_DATASTORE

int main()
{
	std::cout << "\n Testing DataStore\n";
	DataStore DataStore1;
	DataStore1.save_path("/Admissions/Syracuse");   //Passing dummy values to DataStore to test save functionality
	DataStore1.save_path("/Admissions/UNCC");
	DataStore1.save_path("/Admissions/ASU");
	DataStore1.save_path("/Admissions/SUNY");
	DataStore::itr_pathset itr_pathset2;
	DataStore::reflist reflist1, reflist2, reflist3;
	for (itr_pathset2 = DataStore1.begin_path(); itr_pathset2 != DataStore1.end_path(); ++itr_pathset2)
	{
		if (*itr_pathset2 == "/Admissions/Syracuse" || *itr_pathset2 == "/Admissions/UNCC")
			reflist1.push_back(itr_pathset2);
	}
	for (itr_pathset2 = DataStore1.begin_path(); itr_pathset2 != DataStore1.end_path(); ++itr_pathset2)
	{
		if (*itr_pathset2 == "/Admissions/ASU" || *itr_pathset2 == "/Admissions/SUNY")
			reflist2.push_back(itr_pathset2);
	}for (itr_pathset2 = DataStore1.begin_path(); itr_pathset2 != DataStore1.end_path(); ++itr_pathset2)
	{
		if (*itr_pathset2 == "/Admissions/SUNY" || *itr_pathset2 == "/Admissions/Syracuse")
			reflist3.push_back(itr_pathset2);
	}
	DataStore::filename filename1 = "Sai Krishna.txt", filename2 = "Pavan Kumar.txt", filename3 = "Sandesh.txt";
	DataStore1.save_map(filename1, reflist1);
	DataStore1.save_map(filename2, reflist2);
	DataStore1.save_map(filename3, reflist3);
	//Display ds(DataStore1);											//Displaying the stored contents using display class
	//ds.display(0);
}
#endif
