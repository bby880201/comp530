#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include "fstream"
#include "MyDB_BufferManager.h"

// these functions write a bunch of characters to a string... used to produce data
void writeNums (char *bytes, size_t len, int i) {
    for (size_t j = 0; j < len - 1; j++) {
        bytes[j] = '0' + (i % 10);
    }
    bytes[len - 1] = 0;
}

void writeLetters (char *bytes, size_t len, int i) {
    for (size_t j = 0; j < len - 1; j++) {
        bytes[j] = 'i' + (i % 10);
    }
    bytes[len - 1] = 0;
}

void writeSymbols (char *bytes, size_t len, int i) {
    for (size_t j = 0; j < len - 1; j++) {
        bytes[j] = '!' + (i % 10);
    }
    bytes[len - 1] = 0;
}


int main(){
//    strcpy(buf, "0123456789");
//    char* a = buf;
//    char* b = ++buf;
//    buf +=3;
//    char* c =buf;
//
//    cout << ++buf<<endl;
//    cout << a<<endl;
//    cout << b <<endl;
//    cout << c<<endl;
//    MyDB_PageHandle ptr(new MyDB_PageHandleBase::MyDB_PageHandleBase(buf,10,0,"temp",1));
//    printf("%d", ptr->isDirty);
//    ptr->wroteBytes();
//    printf("%d", ptr->isDirty);
//    char* pg = (char*) ptr->getBytes();
//    strcpy(pg, "0000000000000");
//    printf("%s", ptr->getBytes());
//    ptr->~MyDB_PageHandleBase();
//
    
//    MyDB_BufferPage a = MyDB_BufferPage :: MyDB_BufferPage(buf, 0, "", 0);
    
//    printf("%d", a.isDirty);

    MyDB_BufferManager* mgr = new MyDB_BufferManager(10, 3, "tmp");
    MyDB_TablePtr whichTb(new MyDB_Table("tmp", "./tmp.txt"));
    MyDB_PageHandle ph = mgr->getPage( whichTb, 0);
    
    char *bytes = (char *) ph->getBytes();
    writeNums (bytes, 10, 0);
    ph->wroteBytes();
}