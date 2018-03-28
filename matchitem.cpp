#include "matchitem.h"
#include "ui_matchitem.h"

MatchItem::MatchItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MatchItem)
{
    ui->setupUi(this);
}

MatchItem::~MatchItem()
{
    delete ui;
}

MatchItem* MatchItem::date(QString date)
{
    ui->label->setText(date);
    return this;
}
