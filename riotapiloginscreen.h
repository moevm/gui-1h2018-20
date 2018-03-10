#ifndef RIOTAPILOGINSCREEN_H
#define RIOTAPILOGINSCREEN_H

#include <QDialog>

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
private slots:
    void verifyInputAndOpenMainWindow();
};

#endif // RIOTAPILOGINSCREEN_H
