#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "filedownloader.h"
#include "matchitem.h"
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
    MatchItem* createMatchItem(QString summoner, MatchInfo* match);
    QPixmap scaleIcon(QPixmap icon, int size);
private slots:
    void profileIconDownloadAndSet(const QString& iconId);
    void setLeagueInfo(LeagueInfo& info);
    void openLoginScreen();
    void fillMatches(const QVector<MatchInfo*>&);
signals:
    void allContentFinished();
};

#endif // MAINWINDOW_H
