#ifndef MOREDETAIL_H
#define MOREDETAIL_H

#include <QWidget>

namespace Ui {
class Moredetail;
}

class Moredetail : public QWidget
{
    Q_OBJECT

public:
    explicit Moredetail(QWidget *parent = nullptr);
    ~Moredetail();

private:
    Ui::Moredetail *ui;
};

#endif // MOREDETAIL_H
