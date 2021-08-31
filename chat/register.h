#ifndef REGISTER_H
#define REGISTER_H

#include <QDialog>

namespace Ui {
class Register;
}

class Register : public QDialog
{
    Q_OBJECT

public:
    explicit Register(QWidget *parent = nullptr);
    ~Register();
    void clearInfo();

protected:

    void keyPressEvent(QKeyEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);

signals:
    void signal_register(QString, QString);
    void signal_cancel();

private slots:
    void on_cancel_clicked();
    void on_handin_clicked();
    
private:
    Ui::Register *ui;
    QPoint p;
};

#endif // REGISTER_H
