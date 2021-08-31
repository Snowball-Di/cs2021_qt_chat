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
    resize(800, 600);    
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
    // TO DELETE
    Moredetail *more = new Moredetail(this);
    more->show();


    emit this->signal_func();
}

void UsrMain::on_search_clicked()
{
    this->showdialog();
}

void UsrMain::on_logout_clicked()
{
    emit signal_logout();
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
    ChatWindow *chat = new ChatWindow(this);
    chat->adjustSize();
    chat->show();
}

void UsrMain::on_friendlist_clicked()
{
    if(ui->friendlist->isChecked())
    {
        this->showfriend = true;
        emit this->signal_groupList();
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