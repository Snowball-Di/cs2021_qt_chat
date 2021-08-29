#include "addmember.h"
#include "ui_addmember.h"

addMember::addMember(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addMember)
{
    ui->setupUi(this);
}

addMember::~addMember()
{
    delete ui;
}
