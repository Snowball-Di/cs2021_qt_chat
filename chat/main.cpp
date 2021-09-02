/*
 * @Author: your name
 * @Date: 2021-08-27 16:20:30
 * @LastEditTime: 2021-09-02 10:57:20
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: \undefinedd:\Files\Resource\Github\cs2021_qt_chat\chat\main.cpp
 */
#include "chatwindow.h"
#include "logwindow.h"
#include "manager.h"
#include "QDebug"
#include "usrmain.h"
#include <QApplication>
#include "client.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    Client* client = Client::client_init();
    client->connectToServer();

    return a.exec();
}
