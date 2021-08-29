#include "logwindow.h"
#include "ui_logwindow.h"
#include "usrmain.h"
#include "ui_usrmain.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>

LogWindow::LogWindow(QWidget *parent):QWidget(parent), ui(new Ui::LogWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    setAttribute(Qt::WA_TranslucentBackground);

    
    //默认记住帐号信息
    
    //TODO 从接入本地缓存数据账号信息、头像、记住帐号等信息
    this->remember_id = true;
    
    if (this->remember_id)
    {
        ui->id->setText("972659");
        this->remember_id = false;
        this->ui->label->setPixmap(QPixmap(":/img/img/log_icon.png"));
    }
    
}

LogWindow::~LogWindow()
{
    delete ui;
}

void LogWindow::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        p = e->globalPos() - this->frameGeometry().topLeft();
    }
}

void LogWindow::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton)
    {
        move(e->globalPos() - p);
    }

}

void LogWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        emit ui->login->clicked();
    }
}

void LogWindow::on_login_clicked()
{
    if (this->ui->id->text() == "")
    {
        QMessageBox::information(this, tr("提示"), tr("帐号不能为空"), QMessageBox::Yes);
        ui->id->clear();
        ui->id->setFocus();
    }
    else if (this->ui->password->text() == "")
    {
        QMessageBox::information(this, tr("提示"), tr("密码不能为空"), QMessageBox::Yes);
        ui->password->clear();
        ui->id->setFocus();
    }
    else{
        //TODO 接入接口校验帐号密码
        UsrMain usr;
        usr.show();
        this->close();
    }
}

void LogWindow::on_checkBox_clicked()
{
    int remember = this->ui->checkBox->checkState();
    if(remember == 1)
    {
        this->remember_id = true;
    }
    else
    {
        this->remember_id = false;
    }

    //TODO 设置缓存帐号记住信息

}


