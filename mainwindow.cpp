#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "riotapiloginscreen.h"
#include "riotapi.h"
#include "matchitem.h"
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
    ui->verticalLayout_2->setAlignment(Qt::AlignHCenter);
    connect(&RiotApi::Instance(), &RiotApi::summonerNameUpdated, ui->label_3, &QLabel::setText);
    connect(&RiotApi::Instance(), &RiotApi::summonerProfileIconIdUpdated, [=](const QString& x){
        QString url = "http://ddragon.leagueoflegends.com/cdn/8.5.2/img/profileicon/" + x + ".png";
        qDebug() << url;
        QUrl imageUrl(url);
        iconDownloader = new FileDownloader(imageUrl, this);
        connect(iconDownloader, SIGNAL (downloaded()), this, SLOT (loadProfileIcon()));
    });
    connect(&RiotApi::Instance(), &RiotApi::summonerLeagueInfoUpdated, [=](LeagueInfo& info) {
        ui->label->setText(info.rank + " " + info.tier + "\n" +
                           info.leaguePoints + " LP\n" +
                           "W: " + info.wins + " L: " + info.losses);
    });
    connect(&RiotApi::Instance(), &RiotApi::accountIdUpdated, [=](const QString& x) {
        RiotApi::Instance().requestRecentMatches(x);
    });
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

void MainWindow::loadProfileIcon()
{
    profileIcon.loadFromData(iconDownloader->downloadedData());
    profileIcon = roundImage(profileIcon);

    ui->label_2->setPixmap(profileIcon);
    emit allContentFinished();
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

void MainWindow::fillMatches(const QVector<MatchInfo>& matches)
{
    for(auto start = matches.begin(); start != matches.end(); start++) {
        QDateTime matchTime;
        matchTime.setTime_t(start->timestamp.toLong());

        QListWidgetItem* item = new QListWidgetItem( ui->listWidget );
        MatchItem* match = (new MatchItem())->date(matchTime.toString("hh:mm\t    dd MMM yyyy "));
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
        disconnect();
        close();
    } else if (resBtn == QMessageBox::Close) {
        event->accept();
    } else {
        event->ignore();
    }
}
