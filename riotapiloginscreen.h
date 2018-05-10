#ifndef RIOTAPILOGINSCREEN_H
#define RIOTAPILOGINSCREEN_H

#include <QDialog>
#include <QNetworkReply>

namespace Ui {
class RiotApiLoginScreen;
}

class RiotApiLoginScreen : public QDialog
{
    Q_OBJECT

public:
    explicit RiotApiLoginScreen(QWidget *parent = 0);
    ~RiotApiLoginScreen();

private:
    Ui::RiotApiLoginScreen *ui;
    QString m_sSettingsFile;
    int err = 0;
    QString getErrorString(QNetworkReply::NetworkError error);
private slots:
    void verifyInputAndOpenMainWindow();
    void loadSettings();
    void saveSettings();
};

#endif // RIOTAPILOGINSCREEN_H
