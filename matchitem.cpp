#include "matchitem.h"
#include "ui_matchitem.h"
#include "string"
#include "cachemanager.h"
#include "filedownloader.h"
#include "matchform.h"

#include <QPainter>
#include <QWidget>
MatchItem::MatchItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MatchItem)
{
    ui->setupUi(this);
    getAndSetIcon("score", ui->kdaIconLabel);
    getAndSetIcon("gold", ui->goldIconLabel);
    getAndSetIcon("minion", ui->minionsIconLabel);
}

MatchItem::~MatchItem()
{
    delete ui;
}

MatchItem* MatchItem::date(QString date)
{
    ui->dateLabel->setText(date);
    return this;
}

MatchItem *MatchItem::duration(int m)
{
    ui->durationLabel->setText("Duration: " + QString::number(m) + " minutes");
    return this;
}

MatchItem *MatchItem::icon(QPixmap icon)
{
    qDebug() << icon;
    qDebug() << ui;
    qDebug() << ui->iconLabel;
    ui->iconLabel->setPixmap(icon);
    return this;
}

MatchItem *MatchItem::kda(int k, int d, int a)
{
    ui->kdaLabel->setText(QString::number(k) + "/" + QString::number(d) + "/" + QString::number(a));
    return this;
}

MatchItem *MatchItem::gold(int g)
{
    ui->goldLabel->setText(QString::number(g));
    return this;
}

MatchItem *MatchItem::minions(int m)
{
    ui->minionsLabel->setText(QString::number(m));
    return this;
}

MatchItem *MatchItem::win(bool w)
{

    if (w) {
        setObjectName("win");
        setStyleSheet("QWidget#win {background-color: qlineargradient(x1:0, y1:0.4, x2:0, y2:1, stop:0 rgba(204, 255, 204, 255), stop:1 rgba(102, 255, 102, 255));}");
    } else {
        setObjectName("lose");
        setStyleSheet("QWidget#lose {background-color: qlineargradient(x1:0, y1:0.4, x2:0, y2:1, stop:0 rgba(255, 204, 204, 255), stop:1 rgba(255, 77, 77, 255));}");
    }
    return this;
}

MatchItem *MatchItem::participants(QMap<int, QList<QString>> pcs)
{
    this->pcs = pcs;
    return this;
}

void MatchItem::getAndSetIcon(QString name, QLabel *label)
{
    if(CacheManager::Instance().cached(name+".png")) {
        QPixmap img(CacheManager::Instance().getPath(name+".png"));
        label->setPixmap(img);
    } else {
        QUrl url("http://ddragon.leagueoflegends.com/cdn/8.9.1/img/ui/" + name + ".png");
        FileDownloader* fd = new FileDownloader(url);
        connect(fd, &FileDownloader::downloaded, [=]() {
            QPixmap img;
            img.loadFromData(fd->downloadedData());
            CacheManager::Instance().cacheImg(name+".png", img);
            label->setPixmap(img);
        });
    }
}

void MatchItem::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void MatchItem::mouseDoubleClickEvent(QMouseEvent *event)
{
    (new MatchForm(pcs))->show();
}
