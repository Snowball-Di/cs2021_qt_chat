#ifndef LogWindow_H
#define LogWindow_H

#include <QDialog>
#include <register.h>

QT_BEGIN_NAMESPACE
namespace Ui { class LogWindow; }
QT_END_NAMESPACE

class LogWindow : public QDialog
{
    Q_OBJECT

public:

    LogWindow(QWidget *parent = nullptr);
    ~LogWindow();
    void setUi(int id, bool checked);

signals:
    void signal_login(int, QString, bool); //请求登录
    void signal_to_register(); //跳转注册页面

protected:
    
    void keyPressEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);

private slots:
    void on_login_clicked();
    void on_newid_clicked();

private:
    Ui::LogWindow *ui;
    QPoint p;
};
#endif // LogWindow_H
