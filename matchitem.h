#ifndef MATCHITEM_H
#define MATCHITEM_H

#include <QWidget>
#include <QString>
#include <QLabel>
#include <QMap>

namespace Ui {
class MatchItem;
}

class MatchItem : public QWidget
{
    Q_OBJECT

public:
    explicit MatchItem(QWidget *parent = 0);
    ~MatchItem();
    MatchItem* date(QString date);
    MatchItem* duration(int m);
    MatchItem* icon(QPixmap icon);
    MatchItem* kda(int k, int d, int a);
    MatchItem* gold(int g);
    MatchItem* minions(int m);
    MatchItem* win(bool w);
    MatchItem* participants(QMap<int, QList<QString>>);
private:
    Ui::MatchItem *ui;
    QMap<int, QList<QString>> pcs;
    void getAndSetIcon(QString name, QLabel* label);
    void paintEvent(QPaintEvent *) override;
    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
};

#endif // MATCHITEM_H
