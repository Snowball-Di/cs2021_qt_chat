#include "newgroup.h"
#include "ui_newgroup.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>

newGroup::newGroup(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newGroup)
{
    ui->setupUi(this);
}

newGroup::~newGroup()
{
    delete ui;
}

void newGroup::on_save_clicked()
{
    this->name = this->ui->lineEdit->text();
    QMessageBox::information(this, tr("提示"), tr("保存成功！"), QMessageBox::Yes);
}

void newGroup::on_up_clicked()
{
    if(this->ui->lineEdit->text() == "")
    {
        QMessageBox::information(this, tr("提示"), tr("群名字不能为空！"), QMessageBox::Yes);
    }
    else
    {
        emit this->signal_newGroup(this->ui->lineEdit->text());
//        QMessageBox::information(this, tr("提示"), tr("提交成功"), QMessageBox::Yes);
//        this->ui->lineEdit->clear();
//        this->name.clear();
    }
}

void newGroup::on_back_clicked()
{
    this->close();
}

void newGroup::setinfo()
{
    this->ui->lineEdit->setText(name);
}
