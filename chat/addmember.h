#ifndef ADDMEMBER_H
#define ADDMEMBER_H

#include <QWidget>

namespace Ui {
class addMember;
}

class addMember : public QWidget
{
    Q_OBJECT

public:
    explicit addMember(QWidget *parent = nullptr);
    ~addMember();

private:
    Ui::addMember *ui;
};

#endif // ADDMEMBER_H
