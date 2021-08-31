#include "chatwindow.h"
#include "ui_chatwindow.h"
#include <QDateTime>
#include <QDebug>


ChatWindow::ChatWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);

    resize(800, 600);
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::on_send_clicked()
{
    QString msg = ui->textEdit->toPlainText();
    ui->textEdit->clear();
    time_t time = QDateTime::currentDateTime().toTime_t(); //时间戳

    emit this->signal_send(this->groupid, msg);

    bool isSending = true; // 发送中

    qDebug()<<"addMessage" << msg << time << ui->listWidget->count();

        if(isSending) {
            dealMessageTime(time);

            chatmessagebox* messageW = new chatmessagebox(ui->listWidget->parentWidget());
            QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
            dealMessage(messageW, item, msg, time, chatmessagebox::User_Me);
        } else {
            bool isOver = true;
            for(int i = ui->listWidget->count() - 1; i > 0; i--) {
                chatmessagebox* messageW = (chatmessagebox*)ui->listWidget->itemWidget(ui->listWidget->item(i));
                if(messageW->text() == msg) {
                    isOver = false;
                    messageW->setItemSuccess();
                }
            }
            if(isOver) {
                dealMessageTime(time);

                chatmessagebox* messageW = new chatmessagebox(ui->listWidget->parentWidget());
                QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
                dealMessage(messageW, item, msg, time, chatmessagebox::User_Me);
                messageW->setItemSuccess();
            }
        }

    ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
}

void ChatWindow::dealMessage(chatmessagebox *messageW, QListWidgetItem *item, QString text, time_t time,  chatmessagebox::Text_Type type)
{
    messageW->setFixedWidth(this->width());
    QSize size = messageW->fontRect(text);
    item->setSizeHint(size);
    messageW->setItem(text, time, size, type);
    ui->listWidget->setItemWidget(item, messageW);
}


void ChatWindow::dealMessageTime(time_t curMsgTime)
{
    bool isShowTime = false;
    if(ui->listWidget->count() > 0) {
        QListWidgetItem* lastItem = ui->listWidget->item(ui->listWidget->count() - 1);
        chatmessagebox* messageW = (chatmessagebox*)ui->listWidget->itemWidget(lastItem);
        int lastTime = messageW->time().toInt();
        int curTime = curMsgTime;
        qDebug() << "curTime lastTime:" << curTime - lastTime;
        isShowTime = ((curTime - lastTime) > 60); // 两个消息相差一分钟
//        isShowTime = true;
    } else {
        isShowTime = true;
    }
    if(isShowTime) {
        chatmessagebox* messageTime = new chatmessagebox(ui->listWidget->parentWidget());
        QListWidgetItem* itemTime = new QListWidgetItem(ui->listWidget);

        QSize size = QSize(this->width(), 40);
        messageTime->resize(size);
        itemTime->setSizeHint(size);
        QString timetext = QDateTime::fromTime_t(curMsgTime).toString("hh:mm");
        messageTime->setItem(timetext, curMsgTime, size, chatmessagebox::User_Time);
        ui->listWidget->setItemWidget(itemTime, messageTime);
    }
}

void ChatWindow::loadMessageHis(QVector<Msg>&list, int usrid)
{
    this->ui->listWidget->clear();
    for(int i = 0; i< list.length(); i++)
    {
        if(list[i].senderID != usrid)
        {
        chatmessagebox* messageW = new chatmessagebox(ui->listWidget->parentWidget());
        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
        dealMessage(messageW, item, list[i].text, list[i].sendTime, chatmessagebox::User_She);
        }
        else
        {
            chatmessagebox* messageW = new chatmessagebox(ui->listWidget->parentWidget());
            QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
            dealMessage(messageW, item, list[i].text, list[i].sendTime, chatmessagebox::User_Me);
        }
    }
}

void ChatWindow::she_sendingMsg(QString text, time_t time)
{
    dealMessageTime(time);
    chatmessagebox* messageW = new chatmessagebox(ui->listWidget->parentWidget());
    QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
    dealMessage(messageW, item, text, time, chatmessagebox::User_She);
}
