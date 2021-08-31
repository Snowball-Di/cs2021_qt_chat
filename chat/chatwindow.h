#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QListWidgetItem>
#include "chatmessagebox.h"
#include <QDialog>
#include "manager.h"

namespace Ui {
class ChatWindow;
}

class ChatWindow : public QDialog
{
    Q_OBJECT

public:

    int groupid;
    int friendid;

    explicit ChatWindow(QWidget *parent = 0);
    ~ChatWindow();

    void loadMessageHis(QVector<Msg>&, int);
    void she_sendingMsg(QString text, time_t time);

    void dealMessage(chatmessagebox *messageW, QListWidgetItem *item, QString text, time_t time, chatmessagebox::Text_Type type);
    void dealMessageTime(time_t curMsgTime);

    void setUi(QString name, QString id);

signals:
    void signal_send(int, QString);

protected:
private slots:
    void on_send_clicked();

private:
    Ui::ChatWindow *ui;
};

#endif // MAINWINDOW_H
