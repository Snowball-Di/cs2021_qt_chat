#ifndef USRMAIN_H
#define USRMAIN_H

#include <QWidget>

namespace Ui {
class UsrMain;
}

class UsrMain : public QWidget
{
    Q_OBJECT

public:
    explicit UsrMain(QWidget *parent = nullptr);
    ~UsrMain();

private:
    Ui::UsrMain *ui;
};

#endif // USRMAIN_H
