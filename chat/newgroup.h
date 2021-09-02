#ifndef NEWGROUP_H
#define NEWGROUP_H

#include <QDialog>

namespace Ui {
class newGroup;
}

class newGroup : public QDialog
{
    Q_OBJECT

public:
    explicit newGroup(QWidget *parent = nullptr);
    ~newGroup();
    void setinfo();
    void clearinfo();

signals:
    void signal_newGroup(QString);

private slots:
    void on_back_clicked();
    void on_save_clicked();
    void on_up_clicked();

private:
    Ui::newGroup *ui;
    QString name;
};

#endif // NEWGROUP_H
