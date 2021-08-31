#include "register.h"
#include "ui_register.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>

Register::Register(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Register)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    setAttribute(Qt::WA_TranslucentBackground);
    this->clearInfo();
}

Register::~Register()
{
delete ui;
}

void Register::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        p = e->globalPos() - this->frameGeometry().topLeft();
    }
}

void Register::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton)
    {
        move(e->globalPos() - p);
    }

}

void Register::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        emit ui->handin->clicked();
    }
}

void Register::on_cancel_clicked()
{
    emit this->signal_cancel();
}


void Register::on_handin_clicked()
{
    if(this->ui->name->text() == "")
    {
        QMessageBox::information(this, tr("提示"), tr("注册用户名不能为空"), QMessageBox::Yes);
        ui->name->clear();
        ui->name->setFocus();
    }
    else if(this->ui->password->text() == "")
    {
        QMessageBox::information(this, tr("提示"), tr("密码不能为空"), QMessageBox::Yes);
        ui->password->clear();
        ui->password->setFocus();
    }
    else
    {
        QString name = ui->name->text();
        QString pw = ui->password->text();
        this->clearInfo();
        emit this->signal_register(name, pw);
    }
}

void Register::clearInfo()
{
    this->ui->name->clear();
    this->ui->password->clear();
}
