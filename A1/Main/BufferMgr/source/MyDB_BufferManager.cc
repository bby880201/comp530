
#ifndef BUFFER_MGR_C
#define BUFFER_MGR_C

#include "MyDB_BufferManager.h"
#include "fstream"
#include <string>
#include <stdlib.h>
#include <unordered_map>
#include <iostream>

using namespace std;

MyDB_PageHandle MyDB_BufferManager :: getPage (MyDB_TablePtr whichTable, long i) {
    //have to check if i is existing. If so, return its handle directly

    MyDB_PageHandle ptr(new MyDB_PageHandleBase(whichTable, i, this));
    
    if (whichTable) {
        string loc = whichTable->getStorageLoc()+"-"+to_string(i);
        if (pageLookUp.count(loc)>0) {
            MyDB_BufferPage* pg = pageLookUp[loc];
            pg->numReference++;
            
            refreshPage(pg);
        }
        else {
            //apply for an available buffer space
            if (pinnedPage.size()>=numPages){
                ptr = nullptr;
                cerr << "All Buffer Pages Are Pinned, Try To Request Later!";
            }else {
                getNewPage(whichTable, i);
            }
        }
    }
    
    counter++;
	return ptr;
}

MyDB_PageHandle MyDB_BufferManager :: getPage () {

    counter++;
	return nullptr;
}

MyDB_PageHandle MyDB_BufferManager :: getPinnedPage (MyDB_TablePtr, long) {

    counter++;
	return nullptr;
}

MyDB_PageHandle MyDB_BufferManager :: getPinnedPage () {
    
    counter++;
	return nullptr;
}

void MyDB_BufferManager :: unpin (MyDB_PageHandle unpinMe) {
    if (unpinMe){
    }
}

MyDB_BufferManager :: MyDB_BufferManager (size_t pageSize, size_t numPages, string tempFileName) {
    
    this->counter = 0;
    this->pageSize = pageSize;
    this->numPages = numPages;
    this->bufPool = (char*) malloc(pageSize * numPages);
    this->tempFile = MyDB_TablePtr(new MyDB_Table("Temp", tempFileName));
    
    char* pgBeg;
    for (size_t i = 0; i<numPages; i++) {
        pgBeg = bufPool + (i*pageSize);
        MyDB_BufferPage* pg = new MyDB_BufferPage(pgBeg, 0, tempFile, 0);
        LRUMgr.push_back(pg);
    }

}

//deconstructor
MyDB_BufferManager :: ~MyDB_BufferManager () {
    while (!LRUMgr.empty()) {
        evictPage(LRUMgr.front());
    }
    while (!pinnedPage.empty()) {
        evictPage(pinnedPage.front());
    }
    
    free(bufPool);
}


void MyDB_BufferManager :: refreshPage(MyDB_BufferPage* pg){
    LRUMgr.remove(pg);
    pg->counter = counter;
    LRUMgr.push_back(pg);
}

MyDB_BufferPage* MyDB_BufferManager :: getNewPage(MyDB_TablePtr ptr, long pn){
    MyDB_BufferPage* pg = LRUMgr.front();
    evictPage(pg);
    pg->tablePtr = ptr;
    pg->pageNumberOnDisk = pn;
    pg->counter = counter;
    pg->isDirty = 0;
    pg->numReference = 0;
    
    pageLookUp[ptr->getStorageLoc() + "-" + to_string(pn)] = pg;
    pg->numReference++;
    LRUMgr.push_back(pg);

    return pg;
}

void MyDB_BufferManager:: evictPage(MyDB_BufferPage* pg){
    string filePath = pg->tablePtr->getStorageLoc();
    string key = filePath + "-" + to_string(pg->pageNumberOnDisk);
    if (pg->isDirty) {
        std::ofstream toFile (filePath,std::ofstream::in);
        toFile.seekp(pg->pageNumberOnDisk * pageSize, ios::beg);
        toFile.write(pg->page, pageSize);
        toFile.flush();
        toFile.close();
    }
    
    pageLookUp.erase(key);
    LRUMgr.remove(pg);

}

MyDB_BufferPage* MyDB_BufferManager:: findPage(MyDB_TablePtr tablePtr, long i) {
    MyDB_BufferPage* pg;
    
    if (tablePtr) {
        string loc = tablePtr->getStorageLoc()+"-"+to_string(i);
        if (pageLookUp.count(loc)>0) {
            pg = pageLookUp[loc];
            refreshPage(pg);
        }
        else {
            if (pinnedPage.size()>=numPages){
                pg = nullptr;
                cerr << "All Buffer Pages Are Pinned, Try To Request Later!";
            }
            else {
                pg = getNewPage(tablePtr, i);
            }
        }
    }
    
    return pg;
}


#endif


