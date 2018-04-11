#ifndef MATCHITEM_H
#define MATCHITEM_H

#include <QWidget>
#include <QString>
#include <QLabel>

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
private:
    Ui::MatchItem *ui;

    void getAndSetIcon(QString name, QLabel* label);
    void paintEvent(QPaintEvent *) override;
};

#endif // MATCHITEM_H
