#include "logWindow.h"
#include "ui_LogWindow.h"
#include <QPainter>
#include <QMouseEvent>

LogWindow::LogWindow(QWidget *parent):QWidget(parent), ui(new Ui::LogWindow)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    setAttribute(Qt::WA_TranslucentBackground);
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
