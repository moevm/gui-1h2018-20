#include "matchform.h"
#include "ui_matchform.h"

#include <QLabel>
#include <QList>

MatchForm::MatchForm(QMap<int, QList<QString>>& participants) : QWidget(0), ui(new Ui::MatchForm)
{
    ui->setupUi(this);
    QFont font("Monospace");
    font.setStyleHint(QFont::TypeWriter);
    for(int i = 1; i < 6; i++) {
        QLabel* a = new QLabel(participants[i][0] + " " + participants[i][1].rightJustified(12));
        a->setFont(font);
        a->setAlignment(Qt::AlignRight);
        ui->teamBlue->addWidget(a);
    }

    for(int i = 6; i < 11; i++) {
        QLabel* a = new QLabel(participants[i][1].leftJustified(12) + " " + participants[i][0]);
        a->setFont(font);
        a->setAlignment(Qt::AlignLeft);
        ui->teamRed->addWidget(a);
    }
}

MatchForm::~MatchForm()
{
    delete ui;
}
