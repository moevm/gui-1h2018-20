#include "riotapiloginscreen.h"
#include "ui_riotapiloginscreen.h"
#include "mainwindow.h"
#include "riotapi.h"
#include <QSettings>

RiotApiLoginScreen::RiotApiLoginScreen(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RiotApiLoginScreen)
{
    ui->setupUi(this);
    m_sSettingsFile = "./settings.ini";
    loadSettings();
}

RiotApiLoginScreen::~RiotApiLoginScreen()
{
    delete ui;
}

void RiotApiLoginScreen::verifyInputAndOpenMainWindow()
{
    RiotApi::Instance().setApiKey(ui->lineEdit->text());
    RiotApi::Instance().setServer(ui->serverComboBox->currentText());
    MainWindow* mw = new MainWindow();
    RiotApi::Instance().requestSummonerInfo(ui->lineEdit_2->text());
    connect(mw, &MainWindow::allContentFinished, [=]() {
        saveSettings();
        mw->show();
        close();
    });
}

void RiotApiLoginScreen::loadSettings()
{
    QSettings settings(m_sSettingsFile, QSettings::NativeFormat);
    QString riot_api = settings.value("riotapi", "").toString();
    QString nickname = settings.value("nickname", "").toString();
    QString server = settings.value("server", "RU").toString();
    ui->lineEdit->setText(riot_api);
    ui->lineEdit_2->setText(nickname);
    ui->serverComboBox->setCurrentText(server);
}

void RiotApiLoginScreen::saveSettings()
{
    QSettings settings(m_sSettingsFile, QSettings::NativeFormat);
    settings.setValue("riotapi", ui->lineEdit->text());
    settings.setValue("nickname", ui->lineEdit_2->text());
    settings.setValue("server", ui->serverComboBox->currentText());
}
