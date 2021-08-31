#include "logwindow.h"
#include "ui_logwindow.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>

LogWindow::LogWindow(QWidget *parent):QDialog(parent),ui(new Ui::LogWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    setAttribute(Qt::WA_TranslucentBackground);   
//    connect(res, SIGNAL(sendid(QString)), this, SLOT(setid(QString)));
//    connect(res, SIGNAL(sendpw(QString)), this, SLOT(setpw(QString)));

}

LogWindow::~LogWindow()
{
    delete ui;
}

//void LogWindow::setid(QString id)
//{
//    this->ui->id->setText(id);
//}

//void LogWindow::setpw(QString pw)
//{
//    this->ui->password->setText(pw);
//}

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

void LogWindow::on_shutdown_clicked(){
    exit(EXIT_SUCCESS);
}

//初始化Ui框信息
void LogWindow::setUi(int id, bool checked)
{
    if(id != 0)
    {
        QString str_id = QString(id);
        this->ui->id->setText(str_id);
        if(checked)
            this->ui->checkBox->setCheckState(Qt::CheckState::Checked);
        else
            this->ui->checkBox->setCheckState(Qt::CheckState::Unchecked);
    }
}

// 登录行为
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
        ui->password->setFocus();
    }
    else{

        bool ok;
        int id = ui->id->text().toInt(&ok, 10);
        if (!ok)
        {
            QMessageBox::information(this, tr("提示"), tr("帐号格式有误，请检查格式"), QMessageBox::Yes);
            ui->id->setFocus();
            ui->password->clear();
        }
        QString pw = ui->password->text();
        bool checked = ui->checkBox->isChecked();
        emit this->signal_login(id, pw, checked);
    }
}

//void LogWindow::on_checkBox_clicked()
//{
//    int remember = this->ui->checkBox->checkState();
//    if(remember == 1)
//    {
//        this->remember_id = true;
//    }
//    else
//    {
//        this->remember_id = false;
//    }

//    //TODO 设置缓存帐号记住信息

//}

void LogWindow::on_newid_clicked()
{
    emit this->signal_to_register();
}


