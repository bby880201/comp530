
#ifndef PAGE_HANDLE_C
#define PAGE_HANDLE_C

#include <memory>
#include "fstream"
#include "MyDB_PageHandle.h"
#include "MyDB_BufferPage.h"



void *MyDB_PageHandleBase :: getBytes () {
    return nullptr;
}

void MyDB_PageHandleBase :: wroteBytes () {
}

MyDB_PageHandleBase :: ~MyDB_PageHandleBase () {
//    counter = 0;
//    if (isDirty) {
//        if (pageNumberOnDisk == -1) {
//            std::ofstream toTemp (filepPath,std::ofstream::app);
//            toTemp.write(page, length);
//            toTemp.flush();
//            toTemp.close();
//        }
//        else {
//            std::ofstream toFile (filepPath,std::ofstream::in);
//            toFile.seekp(pageNumberOnDisk * length, ios::beg);
//            toFile.write(page, length);
//            toFile.flush();
//            toFile.close();
//        }
//    }
}

MyDB_PageHandleBase :: MyDB_PageHandleBase (MyDB_BufferPage pg) : page(pg){
}

void MyDB_PageHandleBase :: pin() {
}

void MyDB_PageHandleBase:: unpin() {
}

#endif

