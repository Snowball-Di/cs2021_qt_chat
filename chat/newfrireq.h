#ifndef NEWFRIREQ_H
#define NEWFRIREQ_H

#include <QDialog>

namespace Ui {
class newFriReq;
}

class newFriReq : public QDialog
{
    Q_OBJECT

public:
    explicit newFriReq(QWidget *parent = nullptr);
    ~newFriReq();
    void setinfo(QString senderid, QString sendername);
    void on_know_clicked();

private:
    Ui::newFriReq *ui;
};

#endif // NEWFRIREQ_H
