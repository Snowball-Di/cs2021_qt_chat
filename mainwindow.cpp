/*
 * @Author: your name
 * @Date: 2021-08-24 22:54:55
 * @LastEditTime: 2021-08-24 23:14:50
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: \cs2021\mainwindow.cpp
 */
#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

