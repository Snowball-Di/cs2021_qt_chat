#ifndef USRMAIN_H
#define USRMAIN_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class UsrMain; }
QT_END_NAMESPACE

class UsrMain : public QWidget
{
    Q_OBJECT

public:
    explicit UsrMain(QWidget *parent = nullptr);
    ~UsrMain();

protected:
//    void resizeEvent(QResizeEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);

private slots:
    void on_morefunc_clicked();
    void on_logout_clicked();
    void on_search_clicked();

private:
    Ui::UsrMain *ui;
    QPoint p;
};

#endif // USRMAIN_H
