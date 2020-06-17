#include "mainwindow.h"
#include <QApplication>
#include <string>
#include <sstream>
#include <stdio.h>
#include <fstream>
#include <QDebug>

using namespace std;

//int runow=0;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();


    return a.exec();
}
