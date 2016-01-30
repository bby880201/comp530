
#ifndef BUFFER_MGR_C
#define BUFFER_MGR_C

#include "MyDB_BufferManager.h"
#include "fstream"
#include <string>
#include <stdlib.h>
#include <unordered_map>

using namespace std;

MyDB_PageHandle MyDB_BufferManager :: getPage (MyDB_TablePtr whichTable, long i) {
    //have to check if i is existing. If so, return its handle directly
    string loc = whichTable->getStorageLoc() + to_string(i);
    if (pageLookUp.count(loc)) {
        MyDB_BufferPage pg = pageLookUp[loc];
    }
    
    counter++;
	return nullptr;		
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

MyDB_BufferManager :: MyDB_BufferManager (size_t pageSize, size_t numPages, string tempFile) {
    
    pageLookUp = unordered_map<string, MyDB_BufferPage>();
    bufPool = (char*) malloc(pageSize * numPages);
    

    //maintain a handle pool that has all page handles.
//    char* handleStart = bfPool;
//    for (size_t i = 0; i<numPages; i++) {
//        MyDB_PageHandle a (handleStart,pageSize,0);
//        handlePool[i] = a;
//        handleStart += pageSize;
    
//    }
    //maintain a heap to find LRU page
    //need a map to keep tracking non-anonymous pages
}

//deconstructor
MyDB_BufferManager :: ~MyDB_BufferManager () {
    
    free(bufPool);
}

#endif


