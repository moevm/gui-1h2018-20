#include "cachemanager.h"
#include "filedownloader.h"
#include "matchform.h"
#include "ui_matchform.h"

#include <QLabel>
#include <QList>
#include <QPixmap>

MatchForm::MatchForm(QMap<int, QList<QString>>& participants) : QWidget(0), ui(new Ui::MatchForm)
{
    ui->setupUi(this);
    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    for(int i = 1; i < 6; i++) {
        QLabel* nickname = new QLabel(participants[i][0]);
        QLabel* kda = new QLabel(participants[i][2] + "/" + participants[i][3] + "/" + participants[i][4]);
        QLabel* icon = new QLabel();
        getAndSetIcon(participants[i][1], icon);
        nickname->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        kda->setAlignment(Qt::AlignVCenter | Qt::AlignRight);
        icon ->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
        QHBoxLayout* item = new QHBoxLayout();
        item->addWidget(nickname);
        item->addWidget(kda);
        item->addWidget(icon);
        ui->teamBlue->addLayout(item);
    }

    for(int i = 6; i < 11; i++) {
        QLabel* nickname = new QLabel(participants[i][0]);
        QLabel* kda = new QLabel(participants[i][2] + "/" + participants[i][3] + "/" + participants[i][4]);
        QLabel* icon = new QLabel();
        getAndSetIcon(participants[i][1], icon);
        nickname->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
        kda->setAlignment(Qt::AlignVCenter | Qt::AlignLeft);
        QHBoxLayout* item = new QHBoxLayout();
        item->addWidget(icon);
        item->addWidget(kda);
        item->addWidget(nickname);
        ui->teamRed->addLayout(item);
    }
}

void MatchForm::getAndSetIcon(QString name, QLabel *label)
{
    qDebug() << name;
    int size = 32;
    if(CacheManager::Instance().cached(name+".png")) {
        QPixmap img(CacheManager::Instance().getPath(name+".png"));
        label->setPixmap(img.scaled(size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    } else {
        QUrl url("http://ddragon.leagueoflegends.com/cdn/8.9.1/img/champion/" + name + ".png");
        FileDownloader* fd = new FileDownloader(url);
        connect(fd, &FileDownloader::downloaded, [=]() {
            QPixmap img;
            img.loadFromData(fd->downloadedData());
            CacheManager::Instance().cacheImg(name+".png", img);
            label->setPixmap(img.scaled(size, size, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        });
    }
}

MatchForm::~MatchForm()
{
    delete ui;
}
