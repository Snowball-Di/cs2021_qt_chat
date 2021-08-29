#ifndef LogWindow_H
#define LogWindow_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class LogWindow; }
QT_END_NAMESPACE

class LogWindow : public QWidget
{
    Q_OBJECT

public:

    LogWindow(QWidget *parent = nullptr);
    ~LogWindow();
    void keyPressEvent(QKeyEvent *event);

protected:
    
    
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);


public slots:
    void on_login_clicked();
    void on_checkBox_clicked();
    
private:
    Ui::LogWindow *ui;
    QPoint p;
    bool remember_id;

};
#endif // LogWindow_H
