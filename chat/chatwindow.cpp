#include "chatwindow.h"
#include "ui_chatwindow.h"
#include <QDateTime>
#include <QDebug>
#include <QMouseEvent>


ChatWindow::ChatWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ChatWindow)
{
    ui->setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
}

ChatWindow::~ChatWindow()
{
    delete ui;
}

void ChatWindow::setUi(QString name, QString id)
{
    this->ui->id->setText(id);
    this->ui->name->setText(name);
    int num = id.toInt() % 5;
    switch(num){
        case 0:
            this->ui->label->setPixmap(QPixmap(":/icon/icon/icon1.png"));
                break;
        case 1:
            this->ui->label->setPixmap(QPixmap(":/icon/icon/icon2.png"));
                break;
        case 2:
            this->ui->label->setPixmap(QPixmap(":/icon/icon/icon3.png"));
                break;
        case 3:
            this->ui->label->setPixmap(QPixmap(":/icon/icon/icon4.png"));
                break;
        case 4:
            this->ui->label->setPixmap(QPixmap(":/icon/icon/icon5.png"));
                break;
    }
    this->ui->label->setScaledContents(true);
}

void ChatWindow::on_send_clicked()
{
    QString msg = ui->textEdit->toPlainText();
    ui->textEdit->clear();
    time_t time = QDateTime::currentDateTime().toTime_t(); //时间戳

    emit this->signal_send(this->groupid, msg);

    bool isSending = true; // 发送中

    QPixmap pic1;
    switch(myid % 5){
        case 0:
            pic1 = QPixmap(":/icon/icon/icon1.png");
        break;
        case 1:
            pic1 = QPixmap(":/icon/icon/icon2.png");
                    break;
        case 2:
            pic1 = QPixmap(":/icon/icon/icon3.png");
                    break;
        case 3:
            pic1 = QPixmap(":/icon/icon/icon4.png");
                    break;
        case 4:
            pic1 = QPixmap(":/icon/icon/icon5.png");
                    break;
    }

    qDebug()<<"addMessage" << msg << time << ui->listWidget->count();

        if(isSending) {
            dealMessageTime(time);

            chatmessagebox* messageW = new chatmessagebox(ui->listWidget->parentWidget());
            QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
            messageW->setPixmap(pic1, pic1);
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
                messageW->setPixmap(pic1, pic1);
                dealMessage(messageW, item, msg, time, chatmessagebox::User_Me);
                messageW->setItemSuccess();
            }
        }

    ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
}

void ChatWindow::mousePressEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
    {
        p = e->globalPos() - this->frameGeometry().topLeft();
    }
}

void ChatWindow::mouseMoveEvent(QMouseEvent *e)
{
    if(e->buttons() & Qt::LeftButton)
    {
        move(e->globalPos() - p);
    }

}

void ChatWindow::keyPressEvent(QKeyEvent *event)
{
    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        emit ui->send->clicked();
    }
}

void ChatWindow::dealMessage(chatmessagebox *messageW, QListWidgetItem *item, QString text, time_t time,  chatmessagebox::Text_Type type)
{
    messageW->setFixedWidth(this->width());
    QSize size = messageW->fontRect(text);
    item->setSizeHint(size);
    messageW->setItem(text, time, size, type);
    ui->listWidget->setItemWidget(item, messageW);
}

void ChatWindow::on_clear_clicked()
{
    ui->textEdit->clear();
}

void ChatWindow::on_back_clicked()
{
    this->hide();
}


void ChatWindow::dealMessageTime(time_t curMsgTime)
{
    bool isShowTime = false;
    if(ui->listWidget->count() > 0) {
        QListWidgetItem* lastItem = ui->listWidget->item(ui->listWidget->count() - 1);
        chatmessagebox* messageW = (chatmessagebox*)ui->listWidget->itemWidget(lastItem);
        int lastTime = messageW->time();
        int curTime = curMsgTime;
        qDebug() << "curTime lastTime:" << curTime - lastTime;
        isShowTime = ((curTime - lastTime) > 120); // 两个消息相差两分钟
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
    QPixmap pic1;
    QPixmap pic2;

    switch(usrid % 5){
        case 0:
            pic1 = QPixmap(":/icon/icon/icon1.png");
        break;
        case 1:
            pic1 = QPixmap(":/icon/icon/icon2.png");
                    break;
        case 2:
            pic1 = QPixmap(":/icon/icon/icon3.png");
                    break;
        case 3:
            pic1 = QPixmap(":/icon/icon/icon4.png");
                    break;
        case 4:
            pic1 = QPixmap(":/icon/icon/icon5.png");
                    break;
    }

    this->ui->listWidget->clear();
    for(int i = 0; i< list.length(); i++)
    {
        if(list[i].senderID != usrid)
        {
            chatmessagebox* messageW = new chatmessagebox(ui->listWidget->parentWidget());
            switch(list[i].senderID % 5){
            case 0:
                pic2 = QPixmap(":/icon/icon/icon1.png");
                        break;
            case 1:
                pic2 = QPixmap(":/icon/icon/icon2.png");
                        break;
            case 2:
                pic2 = QPixmap(":/icon/icon/icon3.png");
                        break;
            case 3:
                pic2 = QPixmap(":/icon/icon/icon4.png");
                        break;
            case 4:
                pic2 = QPixmap(":/icon/icon/icon5.png");
                        break;
        }
        messageW->setPixmap(pic2, pic2);
        QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
        dealMessage(messageW, item, list[i].text, list[i].sendTime, chatmessagebox::User_She);
        }
        else
        {
            chatmessagebox* messageW = new chatmessagebox(ui->listWidget->parentWidget());
            switch(list[i].senderID % 5){
                case 0:
                    pic2 = QPixmap(":/icon/icon/icon1.png");
                            break;
                case 1:
                    pic2 = QPixmap(":/icon/icon/icon2.png");
                            break;
                case 2:
                    pic2 = QPixmap(":/icon/icon/icon3.png");
                            break;
                case 3:
                    pic2 = QPixmap(":/icon/icon/icon4.png");
                            break;
                case 4:
                    pic2 = QPixmap(":/icon/icon/icon5.png");
                            break;
            }
            messageW->setPixmap(pic1, pic1);
            QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
            dealMessage(messageW, item, list[i].text, list[i].sendTime, chatmessagebox::User_Me);
        }
    }
    ui->listWidget->setCurrentRow(ui->listWidget->count()-1);
}

void ChatWindow::she_sendingMsg(QString text, time_t time)
{
    dealMessageTime(time);
    chatmessagebox* messageW = new chatmessagebox(ui->listWidget->parentWidget());
    QListWidgetItem* item = new QListWidgetItem(ui->listWidget);
    dealMessage(messageW, item, text, time, chatmessagebox::User_She);
}
