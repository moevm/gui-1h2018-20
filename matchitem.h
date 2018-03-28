#ifndef MATCHITEM_H
#define MATCHITEM_H

#include <QWidget>

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
private:
    Ui::MatchItem *ui;
};

#endif // MATCHITEM_H
