//
//  MyDB_BufferPage.cpp
//  comp530
//
//  Created by Boyang Bai on 1/30/16.
//  Copyright © 2016 Boyang Bai. All rights reserved.
//

#include "MyDB_BufferPage.h"
#include <string>

MyDB_BufferPage :: MyDB_BufferPage(char* buf, long count, MyDB_TablePtr ptr, long pn) {
    isDirty = 0;
    page = buf;
    counter = count;
    tablePtr = ptr;
    pageNumberOnDisk = pn;
    numReference = 0;
    
}

MyDB_BufferPage :: ~MyDB_BufferPage() {
    
}

void MyDB_BufferPage :: isWriten(){
    isDirty = 1;
}
