#ifndef ACCEPTREQ_H
#define ACCEPTREQ_H

#include <QDialog>

namespace Ui {
class acceptReq;
}

class acceptReq : public QDialog
{
    Q_OBJECT

public:
    explicit acceptReq(QWidget *parent = nullptr);
    ~acceptReq();

signals:
    void signal_acceptReq(bool);

private slots:
    void on_accept_clicked();
    void on_refuse_clicked();

private:
    Ui::acceptReq *ui;
};

#endif // ACCEPTREQ_H
