#ifndef MATCHITEM_H
#define MATCHITEM_H

#include <QWidget>

namespace Ui {
class matchItem;
}

class matchItem : public QWidget
{
    Q_OBJECT

public:
    explicit matchItem(QWidget *parent = 0);
    ~matchItem();

private:
    Ui::matchItem *ui;
};

#endif // MATCHITEM_H
