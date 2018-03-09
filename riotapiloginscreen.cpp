#include "riotapiloginscreen.h"
#include "ui_riotapiloginscreen.h"

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
