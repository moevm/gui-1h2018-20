#include "mainwindow.h"
#include "riotapiloginscreen.h"
#include <QApplication>
#include "jsonrestlistmodel.h"
#include "riotapi.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    RiotApiLoginScreen w;
    w.show();

    return a.exec();
}
