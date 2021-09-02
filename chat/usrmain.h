#ifndef USRMAIN_H
#define USRMAIN_H

#include <QWidget>
#include <QListWidgetItem>
#include "listitem.h"
#include "moredetail.h"
#include "manager.h"
#include <QPainter>
#include <QMouseEvent>

QT_BEGIN_NAMESPACE
namespace Ui { class UsrMain; }
QT_END_NAMESPACE

class UsrMain : public QDialog
{
    Q_OBJECT

public:
    explicit UsrMain(QWidget *parent = nullptr);
    ~UsrMain();

    void load_friendlist(QVector<Friend>&);
    void load_grouplist(QVector<Group>&);

    void insert_friend(Friend&);
    void insert_group(Group&);

    int Usrid;
    int Name;
    int pic;
    bool showfriend; // 0:显示好友列表, 1显示群组列表
    listItem* items; //item数组
    int len; //item长度
    void showdialog();


protected:
    void setDialogItem(listItem *dialog,
        QListWidgetItem *item, 
        QString text, QPixmap pic, 
        listItem::List_Type type);

    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);

signals:
    void signal_dialog(int);
    void signal_func();
    void signal_logout();
    void signal_friendList();
    void signal_groupList();

private slots:
    void on_morefunc_clicked();
    void on_logout_clicked();
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);
    void on_friendlist_clicked();
    void on_grouplist_clicked();

private:
    Ui::UsrMain *ui;
    QPoint p;

};


#endif // USRMAIN_H
