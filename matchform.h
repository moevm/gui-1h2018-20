#ifndef MATCHFORM_H
#define MATCHFORM_H

#include <QLabel>
#include <QWidget>

namespace Ui {
class MatchForm;
}

class MatchForm : public QWidget
{
    Q_OBJECT

public:
    MatchForm(QMap<int, QList<QString>>& participants);
    ~MatchForm();

private:
    Ui::MatchForm *ui;

    void getAndSetIcon(QString name, QLabel *label);
};

#endif // MATCHFORM_H
