#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "filedownloader.h"
#include "riotapi.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    void closeEvent(QCloseEvent *event);

    QPixmap profileIcon;
    FileDownloader* iconDownloader;

    QPixmap roundImage(QPixmap& img);
private slots:
    void openLoginScreen();
    void fillMatches(const QVector<MatchInfo*>&);
signals:
    void allContentFinished();
};

#endif // MAINWINDOW_H
