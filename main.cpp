#include "mainwindow.h"
#include "riotapiloginscreen.h"
#include <QApplication>
#include "cachemanager.h"
#include "riotapi.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CacheManager::Instance();
    RiotApiLoginScreen w;
    w.show();

    return a.exec();
}
