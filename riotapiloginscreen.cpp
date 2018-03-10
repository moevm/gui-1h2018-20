#include "riotapiloginscreen.h"
#include "ui_riotapiloginscreen.h"
#include "mainwindow.h"
#include "riotapi.h"

RiotApiLoginScreen::RiotApiLoginScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RiotApiLoginScreen)
{
    ui->setupUi(this);
}

RiotApiLoginScreen::~RiotApiLoginScreen()
{
    delete ui;
}

void RiotApiLoginScreen::verifyInputAndOpenMainWindow()
{
    MainWindow* mw = new MainWindow();
    RiotApi::Instance().requestSummonerInfo(ui->lineEdit_2->text());
    connect(mw, &MainWindow::allContentFinished, [=]() {
        mw->show();
        close();
    });
}
