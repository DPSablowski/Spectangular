#include "mainwindow.h"
#include <QApplication>
#include <QDir>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <string>

extern int runow;
extern int batchOverwrite;
extern int batchThreads;

// Usage:
//   Spectangular                          GUI
//   Spectangular --batch <configuration>   load configuration (without .sps) from the
//                                         current folder, optimise
//   Spectangular --batch                  read configuration name from the console
//   --overwrite                           overwrite existing start files
//   --threads N                           use N cores (default: 2)
int main(int argc, char *argv[])
{
    // OpenMP threads wait passively: if several programs run at the same time (e.g. several
    // batch runs), actively waiting threads would otherwise burn the others' CPU time
    // (measured: 90 times slower). Must be set before the first parallel region.
    setenv("OMP_WAIT_POLICY", "PASSIVE", 0);
    bool batch = false;
    QString config;
    for(int i=1; i<argc; i++){
        if(std::strcmp(argv[i], "--batch")==0){
            batch = true;
            if(i+1<argc && argv[i+1][0]!='-'){
                config = QString::fromLocal8Bit(argv[++i]);
            }
        }
        else if(std::strcmp(argv[i], "--overwrite")==0){
            batchOverwrite = 1;   // overwrite existing start files without asking
        }
        else if(std::strcmp(argv[i], "--threads")==0 && i+1<argc){
            batchThreads = std::atoi(argv[++i]);
            if(batchThreads<1){
                std::cerr<<"--threads needs a number >= 1"<<std::endl;
                return 1;
            }
        }
        else if(std::strcmp(argv[i], "--help")==0){
            std::cout<<"Spectangular [--batch [config]] [--overwrite] [--threads N]"<<std::endl;
            return 0;
        }
    }
    if(batch && qEnvironmentVariableIsEmpty("QT_QPA_PLATFORM")){
        qputenv("QT_QPA_PLATFORM", "offscreen");   // no display needed
    }

    QApplication a(argc, argv);
    if(batch){
        runow = 1;
        MainWindow w;
        if(config.isEmpty()){
            std::string name;
            std::cout<<"Name of config file:"<<std::endl;
            if(!(std::cin>>name)) return 1;
            config = QString::fromStdString(name);
        }
        return w.runBatch(config);
    }
    MainWindow w;
    w.show();
    return a.exec();
}
