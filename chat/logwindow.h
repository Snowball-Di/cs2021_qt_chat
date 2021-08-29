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

protected:

    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);

private slots:

private:
    Ui::LogWindow *ui;
    QPoint p;
};
#endif // LogWindow_H
