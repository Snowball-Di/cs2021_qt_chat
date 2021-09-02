#include "addnew.h"
#include "ui_addnew.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>

addNew::addNew(QWidget *parent, bool addfriend) :
    QDialog(parent),
    ui(new Ui::addNew),
    addfriend(addfriend)
{
    ui->setupUi(this);
}

addNew::~addNew()
{
    delete ui;
}

void addNew::setInfo()
{
    this->ui->id->setText(this->id);
    this->ui->text->setText(this->text);
}

void addNew::clearinfo()
{
    this->id = "";
    this->text = "";
    this->ui->text->clear();
    this->ui->id->clear();
}

void addNew::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        p = e->globalPos() - this->frameGeometry().topLeft();
    }
}

void addNew::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton)
    {
        move(e->globalPos() - p);
    }

}

void addNew::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        emit ui->save->clicked();
    }
}

void addNew::on_save_clicked()
{
    this->id = ui->id->text();
    this->text = ui->text->toPlainText();
    QMessageBox::information(this, tr("提示"), tr("您填写的信息已保存"), QMessageBox::Yes);
    this->hide();
}

void addNew::on_send_clicked()
{
    if (this->ui->id->text() == "")
        {
            QMessageBox::information(this, tr("提示"), tr("帐号不能为空"), QMessageBox::Yes);
            ui->id->clear();
            ui->id->setFocus();
        }
        else if (this->ui->text->toPlainText() == "")
        {
            QMessageBox::information(this, tr("提示"), tr("验证消息不能为空"), QMessageBox::Yes);
            ui->text->clear();
            ui->text->setFocus();
        }
        else{

            bool ok;
            int id = ui->id->text().toInt(&ok, 10);
            if (!ok)
            {
                QMessageBox::information(this, tr("提示"), tr("帐号格式有误，请检查格式"), QMessageBox::Yes);
                ui->id->setFocus();
            }
            else
            {
                QString text = ui->text->toPlainText();
                if (this->addfriend)

                    emit this->signal_friendReq(id, text);

                else
                   emit this->signal_groupReq(id, text);

                this->hide();
            }
        }
}

void addNew::on_back_clicked()
{
    this->close();
}


