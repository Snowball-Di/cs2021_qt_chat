#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QListWidgetItem>
#include "chatmessagebox.h"
#include <QDialog>

namespace Ui {
class ChatWindow;
}

class ChatWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ChatWindow(QWidget *parent = 0);
    ~ChatWindow();

    void dealMessage(chatmessagebox *messageW, QListWidgetItem *item, QString text, QString time, chatmessagebox::Text_Type type);
    void dealMessageTime(QString curMsgTime);
protected:
private slots:
    void on_send_clicked();

private:
    Ui::ChatWindow *ui;
};

#endif // MAINWINDOW_H
