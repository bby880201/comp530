#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <unordered_map>
#include "fstream"
#include "MyDB_BufferManager.h"
#include "MyDB_PageHandle.h"
#include "MyDB_Table.h"
#include "QUnit.h"
#include <unistd.h>
#include <vector>

class table {
    
public:
    
    // creates a new table with the given name, at the given storage location
    table (string name, string storageLocIn) {
        tableName = name;
        storageLoc = storageLocIn;
    };
    
    // get the name of the table
    string &getName ();
    
    // get the storage location of the table
    string &getStorageLoc ();
    
    // kill the dude
    table ();
    
private:
    
    // the name of the table
    string tableName;
    
    // the location where it is stored
    string storageLoc;
};

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

void printMap(unordered_map<string,MyDB_BufferPage*> myMap){
    for(auto it = myMap.cbegin(); it != myMap.cend(); ++it)
    {
        std::cout << it->first << " " << it->second->page << " p:" << it->second<< "\n";
    }
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

////    MyDB_BufferManager* mgr = new MyDB_BufferManager(10, 3, "tmp");
//    MyDB_BufferManager mgr (10, 3, "tmp");
//    MyDB_TablePtr whichTb = make_shared <MyDB_Table> ("tempTable", "foobar");
//
//    MyDB_PageHandle ph = mgr.getPinnedPage(whichTb,0);
//    printMap(mgr.pageLookUp);
//    
//    char *bytes = (char *) ph->getBytes();
//    writeNums (bytes, 10, 0);
//    ph->wroteBytes();
//    
//    mgr.unpin(ph);
//    
//    printMap(mgr.pageLookUp);
//    
//    MyDB_PageHandle ph2 = mgr.getPinnedPage(whichTb,0);
//    
//    printMap(mgr.pageLookUp);
//
    
//    list<table*> a;
//    list<table*> b;
//    table* a1 = new table("","");
//    table* a2 = new table("","");
//    table* a3 = new table("","");
//    table* b1 = new table("","");
//    a.push_back(a1);
//    a.push_back(a2);
//    a.push_back(a3);
//    b.push_back(b1);
//    a.remove(b1);

    // create a buffer manager
    
    QUnit::UnitTest qunit(cerr, QUnit::verbose);
    
    // UNIT TEST 1: A BIG ONE!!
//    {
//        
//        // create a buffer manager
//        MyDB_BufferManager myMgr (64, 16, "tempDSFSD");
//        MyDB_TablePtr table1 = make_shared <MyDB_Table> ("tempTable", "foobar");
//        
//        // allocate a pinned page
//        cout << "allocating pinned page\n";
//        MyDB_PageHandle pinnedPage = myMgr.getPinnedPage (table1, 0);
//        char *bytes = (char *) pinnedPage->getBytes ();
//        writeNums (bytes, 64, 0);
//        pinnedPage->wroteBytes ();
//        
//        
//        // create a bunch of pinned pages and remember them
//        vector <MyDB_PageHandle> myHandles;
//        for (int i = 1; i < 10; i++) {
//            cout << "allocating pinned page\n";
//            MyDB_PageHandle temp = myMgr.getPinnedPage (table1, i);
//            char *bytes = (char *) temp->getBytes ();
//            writeNums (bytes, 64, i);
//            temp->wroteBytes ();
//            myHandles.push_back (temp);
//        }
//
//    
//        // now forget the pages we created
//        vector <MyDB_PageHandle> temp;
//        myHandles = temp;
//        
//        // now remember 8 more pages
//        for (int i = 0; i < 8; i++) {
//            cout << "allocating pinned page\n";
//            MyDB_PageHandle temp = myMgr.getPinnedPage (table1, i);
//            char *bytes = (char *) temp->getBytes ();
//            
//            // write numbers at the 0th position
//            if (i == 0)
//                writeNums (bytes, 64, i);
//            else
//                writeSymbols (bytes, 64, i);
//            temp->wroteBytes ();
//            myHandles.push_back (temp);
//        }
//        
//        // now correctly write nums at the 0th position
//        cout << "allocating unpinned page\n";
//        MyDB_PageHandle anotherDude = myMgr.getPage (table1, 0);
//        bytes = (char *) anotherDude->getBytes ();
//        writeSymbols (bytes, 64, 0);
//        anotherDude->wroteBytes ();
//        
//        // now do 90 more pages, for which we forget the handle immediately
//        for (int i = 10; i < 100; i++) {
//            cout << "allocating unpinned page\n";
//            MyDB_PageHandle temp = myMgr.getPage (table1, i);
//            char *bytes = (char *) temp->getBytes ();
//            writeNums (bytes, 64, i);
//            temp->wroteBytes ();
//        }
//
//
//        // now forget all of the pinned pages we were remembering
//        vector <MyDB_PageHandle> temp2;
//        myHandles = temp2;
//        
//        printMap(myMgr.pageLookUp);
//        
//        // now get a pair of pages and write them
//        for (int i = 0; i < 100; i++) {
//            cout << "allocating pinned page\n";
//            MyDB_PageHandle oneHandle = myMgr.getPinnedPage ();
//            char *bytes = (char *) oneHandle->getBytes ();
//            writeNums (bytes, 64, i);
//            oneHandle->wroteBytes ();
//            cout << "allocating pinned page\n";
//            MyDB_PageHandle twoHandle = myMgr.getPinnedPage ();
//            writeNums (bytes, 64, i);
//            twoHandle->wroteBytes ();
//        }
//        
//        printMap(myMgr.pageLookUp);
//
//        // make a second table
//        MyDB_TablePtr table2 = make_shared <MyDB_Table> ("tempTable2", "barfoo");
//        for (int i = 0; i < 100; i++) {
//            cout << "allocating unpinned page\n";
//            MyDB_PageHandle temp = myMgr.getPage (table2, i);
//            char *bytes = (char *) temp->getBytes ();
//            writeLetters (bytes, 64, i);
//            temp->wroteBytes ();
//        }
//        printMap(myMgr.pageLookUp);
//    }
    

    // UNIT TEST 2
    {
        MyDB_BufferManager myMgr (64, 16, "tempDSFSD");
        MyDB_TablePtr table1 = make_shared <MyDB_Table> ("tempTable", "foobar");
        
        // look up all of the pages, and make sure they have the correct numbers
        for (int i = 0; i < 100; i++) {
            MyDB_PageHandle temp = myMgr.getPage (table1, i);
            char answer[64];
            if (i < 8)
                writeSymbols (answer, 64, i);
            else
                writeNums (answer, 64, i);
            char *bytes = (char *) temp->getBytes ();
            QUNIT_IS_EQUAL (string (answer), string (bytes));
        }
    }

    
    
    
}