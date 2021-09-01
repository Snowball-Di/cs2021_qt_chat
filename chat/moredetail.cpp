#include "moredetail.h"
#include "ui_moredetail.h"
#include <QPainter>
#include <QMouseEvent>

Moredetail::Moredetail(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Moredetail)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());

}

Moredetail::~Moredetail()
{
    delete ui;
}

void Moredetail::setUser(QString name, QString id, QPixmap pic)
{
    this->ui->id->setText(id);
    this->ui->name->setText(name);
    this->ui->pic->setPixmap(pic);
}

void Moredetail::on_close_clicked()
{
    this->close();
}

void Moredetail::on_addgroup_clicked()
{
    this->addgroup->setInfo();
    this->addgroup->show();
}

void Moredetail::on_addfriend_clicked()
{
    this->addfriend->setInfo();
    this->addfriend->show();
}

void Moredetail::on_newgroup_clicked()
{
    this->newgroup->setinfo();
    this->newgroup->show();
}

void Moredetail::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        p = e->globalPos() - this->frameGeometry().topLeft();
    }
}

void Moredetail::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton)
    {
        move(e->globalPos() - p);
    }

}
