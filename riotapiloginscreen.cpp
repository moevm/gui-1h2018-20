#include "riotapiloginscreen.h"
#include "ui_riotapiloginscreen.h"
#include "mainwindow.h"
#include "riotapi.h"
#include <QMessageBox>
#include <QSettings>
#include <QTimer>

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

QString RiotApiLoginScreen::getErrorString(QNetworkReply::NetworkError error)
{
    switch (error) {
        case QNetworkReply::ContentAccessDenied:
            return "Riot API key is expired\n or just invalid";
        case QNetworkReply::ContentNotFoundError:
            return "Wrong nickname";
        default:
            return "unknown error";
    }
}

void RiotApiLoginScreen::verifyInputAndOpenMainWindow()
{
    RiotApi& riotApi = RiotApi::Instance();
    riotApi.setApiKey(ui->lineEdit->text());
    riotApi.setServer(ui->serverComboBox->currentText());
    MainWindow* mw = new MainWindow();
    riotApi.requestSummonerInfo(ui->lineEdit_2->text());

    err = 0;
    connect(&riotApi, &RiotApi::replyErrorSig, [=](QNetworkReply::NetworkError error) {
        err = error;
        if (err != 0) {
            QMessageBox* window = new QMessageBox;
            window->resize(320, 240);
            window->setWindowTitle("Error");
            window->setText(getErrorString(error));
            window->show();
        }
    });
    connect(mw, &MainWindow::allContentFinished, [=]() {
        if (err == 0) {
            saveSettings();
            mw->show();
            close();
        }
        disconnect(mw, 0, 0, 0);
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
