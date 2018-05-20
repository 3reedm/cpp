#include <iostream>
#include <cstdlib>
#include "mytable.h"
#include "leveldb.h"
#include "kyotocabinet.h"
#include "hamsterdb.h"
#include "berkeleydb.h"
#include "tester.h"
#include "mainwindow.h"
#include <QApplication>

int main() //(int argc, char* argv[])
{
    int bitsKey = 128, bitsValue = 300;
    /*Store** myStore = new Store*[5];
    myStore[0] = new MyTable(bitsKey, bitsValue);
    myStore[1] = new LevelDB;
    myStore[2] = new KyotoCabinet;
    myStore[3] = new BerkeleyDB;
    myStore[4] = new Hamsterdb;*/
    Store* myStore = new Hamsterdb;

    Tester tester;
    tester.writeTest(myStore, 1000000, bitsKey, bitsValue);
    //tester.readTest(myStore, 1000000, bitsKey);
    //tester.overwriteTest(myStore, 500000, bitsKey, bitsValue);

    /*int i;
    for (i = 0; i < 5; ++i)
        myStore[i]->~Store();*/
    myStore->~Store();

    /*QApplication a(argc, argv);
    MainWindow window;
    window.show();
    window.paintEvent();*/

    return 0;
    //return a.exec();
}

