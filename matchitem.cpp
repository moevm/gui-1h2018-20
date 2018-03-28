#include "matchitem.h"
#include "ui_matchitem.h"

matchItem::matchItem(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::matchItem)
{
    ui->setupUi(this);
}

matchItem::~matchItem()
{
    delete ui;
}
