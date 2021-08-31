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
    explicit ChatWindow(QWidget *parent = 0);
    ~ChatWindow();

    void loadMessageHis(QVector<Msg>&, int);
    void addMessage(Msg& m);

    void dealMessage(chatmessagebox *messageW, QListWidgetItem *item, QString text, time_t time, chatmessagebox::Text_Type type);
    void dealMessageTime(time_t curMsgTime);


signals:
    void signal_send(int, QString);

protected:
private slots:
    void on_send_clicked();

private:
    Ui::ChatWindow *ui;
};

#endif // MAINWINDOW_H
