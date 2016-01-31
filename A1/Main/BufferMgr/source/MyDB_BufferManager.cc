
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
        string loc = getKey(whichTable, i);
        if (pageLookUp.count(loc)>0) {
            MyDB_BufferPage* pg = pageLookUp[loc];
            pg->numReference++;
            
            refreshPage(pg);
        }
        else {
            //apply for an available buffer space
            if (!getNewPage(whichTable, i)) ptr = nullptr;
        }
    }
    counter++;
	return ptr;
}

MyDB_PageHandle MyDB_BufferManager :: getPage () {
    
    long i;
    if (!availTempSlot.empty()) {
        i = availTempSlot.front();
        availTempSlot.pop_front();
    } else {
        i = tempFileOffset;
        markTempSpace();
        tempFileOffset++;
    }
    
    MyDB_PageHandle ptr(new MyDB_PageHandleBase(tempFile, i, this));
    
    string loc = getKey(tempFile, i);
    if (!getNewPage(tempFile, i)) ptr = nullptr;

    counter++;
	return ptr;
}

MyDB_PageHandle MyDB_BufferManager :: getPinnedPage (MyDB_TablePtr whichTable, long i) {
    MyDB_PageHandle ptr = getPage(whichTable, i);
    pin(ptr);
    
	return ptr;
}

MyDB_PageHandle MyDB_BufferManager :: getPinnedPage () {
    
    MyDB_PageHandle ptr = getPage();
    pin(ptr);
    
	return ptr;
}

void MyDB_BufferManager :: unpin (MyDB_PageHandle unpinMe) {
    if (unpinMe){
        string key = getKey(unpinMe->DBTable, unpinMe->numPageOnDisk);
        if (pageLookUp.count(key)>0) {
            MyDB_BufferPage* pg = pageLookUp[key];
            if (pg->counter<0) {
                pinnedPage.remove(pg);
                pg->counter = counter;
                LRUMgr.push_back(pg);
            }
        }
    }
}

MyDB_BufferManager :: MyDB_BufferManager (size_t pageSize, size_t numPages, string tempFileName) {
    
    this->counter = 0;
    this->pageSize = pageSize;
    this->numPages = numPages;
    this->bufPool = (char*) malloc(pageSize * numPages);
    this->tempFile = MyDB_TablePtr(new MyDB_Table("Temp", tempFileName));
    this->tempFileOffset = 0;
    
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
        delete LRUMgr.front();
        LRUMgr.pop_front();
    }
    while (!pinnedPage.empty()) {
        evictPage(pinnedPage.front());
        delete pinnedPage.front();
        pinnedPage.pop_front();
    }
    
    free(bufPool);
    cout<<"Everything is gone!"<<endl;
}


void MyDB_BufferManager :: refreshPage(MyDB_BufferPage* pg){
    if (pg->counter>=0) {
        LRUMgr.remove(pg);
        pg->counter = counter;
        LRUMgr.push_back(pg);
    }
}

MyDB_BufferPage* MyDB_BufferManager :: getNewPage(MyDB_TablePtr ptr, long pn){
    MyDB_BufferPage* pg;
    
    if (LRUMgr.empty()){
        cerr << "All Buffer Pages Are Pinned, Try To Request Later!";
        pg = nullptr;
    }else {
        pg = LRUMgr.front();
        evictPage(pg);
        pg->tablePtr = ptr;
        pg->pageNumberOnDisk = pn;
        pg->counter = counter;
        pg->isDirty = 0;
        pg->numReference = 0;
        
        pageLookUp[ptr->getStorageLoc() + "-" + to_string(pn)] = pg;
        pg->numReference++;
        LRUMgr.pop_front();
        LRUMgr.push_back(pg);
        
    }
    return pg;
}

void MyDB_BufferManager:: evictPage(MyDB_BufferPage* pg){
    string filePath = pg->tablePtr->getStorageLoc();
    string key = getKey(pg->tablePtr, pg->pageNumberOnDisk);
    if (pg->isDirty) {
        std::ofstream toFile (filePath,std::ofstream::in);
        toFile.seekp(pg->pageNumberOnDisk * pageSize, ios::beg);
        toFile.write(pg->page, pageSize);
        toFile.flush();
        toFile.close();
    }
    
    pageLookUp.erase(key);
}

MyDB_BufferPage* MyDB_BufferManager :: findPage(MyDB_TablePtr tablePtr, long i) {
    MyDB_BufferPage* pg;
    
    if (tablePtr) {
        string loc = getKey(tablePtr, i);
        if (pageLookUp.count(loc)>0) {
            pg = pageLookUp[loc];
            refreshPage(pg);
        }
        else {
            pg = getNewPage(tablePtr, i);
        }
    }
    
    counter++;
    return pg;
}

string MyDB_BufferManager :: getKey(MyDB_TablePtr tablePtr, long i) {
    if (tablePtr) {
        return tablePtr->getStorageLoc()+"-"+to_string(i);
    }
    else {
        return "";
    }
}

void MyDB_BufferManager :: markTempSpace () {
    std::ofstream toFile (tempFile->getStorageLoc(),std::ofstream::app);
    char* junk = (char*) malloc(pageSize);
    toFile.write(junk, pageSize);
    toFile.flush();
    toFile.close();
    free(junk);
}

void MyDB_BufferManager:: pin (MyDB_PageHandle pinMe){
    if (pinMe){
        string key = getKey(pinMe->DBTable, pinMe->numPageOnDisk);
        if (pageLookUp.count(key)>0) {
            MyDB_BufferPage* pg = pageLookUp[key];
            if (pg->counter>=0) {
                LRUMgr.remove(pg);
                pg->counter = -1;
                pinnedPage.push_back(pg);
            }
        }
    }

}

void MyDB_BufferManager:: dereference(MyDB_TablePtr whichTable, long i){
    if (whichTable) {
        string key = getKey(whichTable, i);
        if (pageLookUp.count(key)>0) {
            MyDB_BufferPage* pg = pageLookUp[key];
            pg->numReference--;
            if ((pg->numReference <=0))  {
                if (pg->counter>=0) {
                    LRUMgr.remove(pg);
                } else {
                    pinnedPage.remove(pg);
                }
                if (pg->tablePtr==tempFile) {
                    availTempSlot.push_back(pg->pageNumberOnDisk);
                }
                pg->counter=counter;
                LRUMgr.push_back(pg);
            }
        }
    }
}


#endif


