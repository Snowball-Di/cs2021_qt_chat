#ifndef ADDNEW_H
#define ADDNEW_H

#include <QDialog>

namespace Ui {
class addNew;
}

class addNew : public QDialog
{
    Q_OBJECT

public:
    explicit addNew(QWidget *parent = nullptr, bool addfriend = true);
    ~addNew();
    void setInfo();
    void clearinfo();

protected:
    void keyPressEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);

signals:
    void signal_friendReq(int, QString);
    void signal_groupReq(int, QString);

   private slots:
        void on_save_clicked();
        void on_send_clicked();
        void on_back_clicked();

private:
    Ui::addNew *ui;
    bool addfriend; //1:好友 0:群组
    QString id;
    QString text; //保存的已填写消息
    QPoint p;
};

#endif // ADDNEW_H
