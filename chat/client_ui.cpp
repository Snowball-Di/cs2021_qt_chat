#include "client_ui.h"
#include "ui_client_ui.h"

client_ui::client_ui(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::client_ui)
{
    ui->setupUi(this);
}

client_ui::~client_ui()
{
    delete ui;
}
