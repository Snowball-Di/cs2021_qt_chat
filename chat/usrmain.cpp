#include "usrmain.h"
#include "ui_usrmain.h"
#include <QPainter>
#include <QMouseEvent>
#include <QMessageBox>
#include "moredetail.h"
#include "ui_moredetail.h"

UsrMain::UsrMain(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::UsrMain)
{
    ui->setupUi(this);
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

}

void UsrMain::on_search_clicked()
{

}

void UsrMain::on_logout_clicked()
{
    
}
