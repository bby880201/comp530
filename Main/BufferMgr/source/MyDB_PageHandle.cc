
#ifndef PAGE_HANDLE_C
#define PAGE_HANDLE_C

#include <memory>
#include "fstream"
#include "MyDB_PageHandle.h"
#include "MyDB_BufferManager.h"
#include <iostream>

void *MyDB_PageHandleBase :: getBytes () {
    
    tempLinkToPage =  bufferMgr->findPage(DBTable,numPageOnDisk);
    return tempLinkToPage->page;
}

void MyDB_PageHandleBase :: wroteBytes () {
    if (tempLinkToPage) {
        tempLinkToPage->isWriten();
    }
}

MyDB_PageHandleBase :: ~MyDB_PageHandleBase () {
    bufferMgr->dereference(DBTable, numPageOnDisk);
}

MyDB_PageHandleBase :: MyDB_PageHandleBase (MyDB_TablePtr table, long i, MyDB_BufferManager* bufMgr){
    this->tempLinkToPage = nullptr;
    this->DBTable = table;
    this->numPageOnDisk = i;
    this->bufferMgr = bufMgr;
}


#endif

