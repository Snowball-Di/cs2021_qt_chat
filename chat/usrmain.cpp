#include "usrmain.h"
#include "ui_usrmain.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include "moredetail.h"
#include "ui_moredetail.h"
#include "chatwindow.h"
#include "ui_chatwindow.h"

UsrMain::UsrMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UsrMain)
{
    this->showfriend = true;
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    setAttribute(Qt::WA_TranslucentBackground);
    ui->friendlist->setChecked(true);
}

UsrMain::~UsrMain()
{
    delete ui;
}

void UsrMain::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        p = e->globalPos() - this->frameGeometry().topLeft();
    }
}

void UsrMain::mouseMoveEvent(QMouseEvent *e)
{
    if(e->button() & Qt::LeftButton)
    {
        move(e->globalPos() - p);
    }
}


void UsrMain::on_morefunc_clicked()
{
    emit this->signal_func();
}


void UsrMain::on_logout_clicked()
{
    emit signal_logout();
    exit(EXIT_SUCCESS);
}

void UsrMain::setDialogItem(listItem *dialog,
        QListWidgetItem *item, 
        QString name, QPixmap pic, 
        listItem::List_Type type)
{
    dialog->setFixedWidth(this->width());
    QSize size = dialog->fontRect(name);
    item->setSizeHint(size);
    dialog->setItem(name, pic, type);
    ui->listWidget->setItemWidget(item, dialog);
}

void UsrMain::showdialog()
{
    // TODO 接入最新的对话窗口列表
    listItem* items[20];

    for(int i = 0; i < 20; i++)
    {
        items[i] = new listItem(ui->listWidget->parentWidget());
        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
        UsrMain::setDialogItem(items[i], item, QString("snowball"), QPixmap(":/img/img/log_icon.png"), listItem::Friend);
    }
}



void UsrMain::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
       int d_id = ((listItem*)(item->listWidget()->itemWidget(item)))->get_dialog_id();
       ((listItem*)(item->listWidget()->itemWidget(item)))->setItemDone();
       emit this->signal_dialog(d_id);
}

void UsrMain::on_friendlist_clicked()
{
    if(ui->friendlist->isChecked())
    {
        this->showfriend = true;
        emit this->signal_friendList();
        ui->grouplist->setChecked(false);
    }
}

void UsrMain::on_grouplist_clicked()
{
    if(ui->grouplist->isChecked())
    {
        this->showfriend = false;
        emit this->signal_groupList();
        ui->friendlist->setChecked(false);
    }
}

void UsrMain::load_grouplist(QVector<Group>& list)
{
    this->ui->listWidget->clear();
    listItem *groups[20];
    for(int i = 0; i < list.length(); i++)
    {
        groups[i] = new listItem(ui->listWidget->parentWidget(), list[i].group_id);
        // TODO 接入头像
        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
        UsrMain::setDialogItem(groups[i], item, list[i].name, QPixmap(":/img/img/log_icon.png"), listItem::Group);
    }
}

void UsrMain::load_friendlist(QVector<Friend>& list)
{
    this->ui->listWidget->clear();
    listItem *friends[20];
    for(int i = 0; i < list.length(); i++)
    {
        friends[i] = new listItem(ui->listWidget->parentWidget(), list[i].group_id, list[i].id);
        // TODO 接入头像
        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
        UsrMain::setDialogItem(friends[i], item, list[i].name, QPixmap(":/img/img/log_icon.png"), listItem::Group);
    }
}

void UsrMain::insert_friend(Friend &fri)
{
    listItem *data = new listItem(ui->listWidget->parentWidget());
    QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
    UsrMain::setDialogItem(data, item, fri.name, QPixmap(":/img/img/log_icon.png"), listItem::Friend);
}

void UsrMain::insert_group(Group& gro)
{
    listItem *data = new listItem(ui->listWidget->parentWidget());
    QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
    UsrMain::setDialogItem(data, item, gro.name, QPixmap(":/img/img/log_icon.png"), listItem::Group);
}






