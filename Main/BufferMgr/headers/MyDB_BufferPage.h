//
//  MyDB_BufferPage.hpp
//  comp530
//
//  Created by Boyang Bai on 1/30/16.
//  Copyright © 2016 Boyang Bai. All rights reserved.
//

#ifndef MyDB_BufferPage_h
#define MyDB_BufferPage_h

#include "MyDB_Table.h"
#include <stdio.h>
#include <string>

using namespace std;

class MyDB_BufferPage {
    
public:
    bool isDirty;
    long counter;
    char* page;
    MyDB_TablePtr tablePtr;
    long pageNumberOnDisk;
    int numReference;
        
    MyDB_BufferPage (char* buf, long count, MyDB_TablePtr ptr, long pn);
    
    ~MyDB_BufferPage ();
    
    void isWriten();
        
};


#endif /* MyDB_BufferPage_h */
