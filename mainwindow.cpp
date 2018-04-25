#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "riotapiloginscreen.h"
#include "riotapi.h"
#include "matchitem.h"
#include "cachemanager.h"
#include <QCloseEvent>
#include <QMessageBox>
#include <iostream>
#include <QPixmap>
#include <QBitmap>
#include <QPainter>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setFixedSize(size());
    statusBar()->setSizeGripEnabled(false);
    ui->verticalLayout_2->setAlignment(Qt::AlignHCenter);

    connect(&RiotApi::Instance(), &RiotApi::summonerNameUpdated, ui->label_3, &QLabel::setText);
    connect(&RiotApi::Instance(), &RiotApi::summonerProfileIconIdUpdated, [=](const QString& x) {
        if(CacheManager::Instance().cached(x+".png")) {
            QPixmap img(CacheManager::Instance().getPath(x+".png"));
            profileIcon = roundImage(img);
            ui->label_2->setPixmap(profileIcon);
            emit allContentFinished();
        } else {
            QUrl url("http://ddragon.leagueoflegends.com/cdn/8.5.2/img/profileicon/" + x + ".png");
            iconDownloader = new FileDownloader(url);
            connect(iconDownloader, &FileDownloader::downloaded, [=]() {
                profileIcon.loadFromData(iconDownloader->downloadedData());
                CacheManager::Instance().cacheImg(x+".png", profileIcon);
                profileIcon = roundImage(profileIcon);
                ui->label_2->setPixmap(profileIcon);
                emit allContentFinished();
            });
        }
    });
    connect(&RiotApi::Instance(), &RiotApi::summonerLeagueInfoUpdated, [=](LeagueInfo& info) {
        ui->label->setText(info.tier + " " + info.rank + "\n" +
                           info.leaguePoints + " LP\n" +
                           "W: " + info.wins + " L: "  + info.losses);
    });
    connect(&RiotApi::Instance(), &RiotApi::accountIdUpdated, [=](const QString& x) {RiotApi::Instance().requestRecentMatches(x);});
    connect(&RiotApi::Instance(), &RiotApi::recentMatchesUpdated, this, &MainWindow::fillMatches);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::openLoginScreen()
{
    (new RiotApiLoginScreen())->open();
}

QPixmap MainWindow::roundImage(QPixmap& img) {
    QImage imageOut(img.size(),QImage::Format_ARGB32);

    imageOut.fill(Qt::transparent);
    QPainter painter( &imageOut );
    painter.setRenderHints(QPainter::HighQualityAntialiasing);
    QRegion r(QRect(0, 0, img.width(), img.width()),
              QRegion::Ellipse);
    painter.setClipRegion(r);
    painter.drawPixmap(0, 0, img);
    return QPixmap::fromImage(imageOut.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
}

void MainWindow::fillMatches(const QVector<MatchInfo*>& matches)
{
    ui->listWidget->clear();
    QString summoner = RiotApi::Instance().getNickname();

    for (auto start = matches.begin(); start != matches.end(); start++) {
        QDateTime matchTime;
        matchTime.setTime_t((*start)->timestamp.toLong());
        MatchItem* match = (new MatchItem())->date(matchTime.toString("dd MMM yyyy"/* + "\t    hh:mm"*/))
                                            ->duration((*start)->duration)
                                            ->kda((*start)->getKills(summoner),
                                                  (*start)->getDeaths(summoner),
                                                  (*start)->getAssists(summoner))
                                            ->gold((*start)->getGold(summoner))
                                            ->minions((*start)->getMinions(summoner))
                                            ->win((*start)->isWin(summoner))
                                            ->participants((*start)->getParticipants());

        if (CacheManager::Instance().cached((*start)->champion+".png")) {
            QPixmap icon(CacheManager::Instance().getPath((*start)->champion+".png"));
            match->icon(icon.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        } else {
            FileDownloader* downloader = new FileDownloader(QUrl("http://ddragon.leagueoflegends.com/cdn/6.24.1/img/champion/"+(*start)->champion+".png"));

            connect(downloader, &FileDownloader::downloaded, [=](){
                QPixmap icon;
                icon.loadFromData(downloader->downloadedData());
                CacheManager::Instance().cacheImg((*start)->champion+".png", icon);
                match->icon(icon.scaled(64, 64, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            });
        }
        QListWidgetItem* item = new QListWidgetItem( ui->listWidget );
        item->setSizeHint( match->size() );
        ui->listWidget->setItemWidget(item, match);
    }
}

void MainWindow::closeEvent (QCloseEvent *event)
{
    QMessageBox::StandardButton resBtn = QMessageBox::question
                                        (this, "Выйти или искать",
                                         tr("Выполнить поиск по другому никнейму?\n"),
                                         QMessageBox::Cancel | QMessageBox::Close | QMessageBox::Yes,
                                         QMessageBox::Yes);

    if (resBtn == QMessageBox::Yes) {
        (new RiotApiLoginScreen())->open();
        RiotApi::Instance().disconnect();
        close();
    } else if (resBtn == QMessageBox::Close) {
        event->accept();
    } else {
        event->ignore();
    }
}
