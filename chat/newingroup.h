#ifndef NEWINGROUP_H
#define NEWINGROUP_H

#include <QDialog>

namespace Ui {
class newinGroup;
}

class newinGroup : public QDialog
{
    Q_OBJECT

public:
    explicit newinGroup(QWidget *parent = nullptr);
    ~newinGroup();
    void setinfo(QString groupid, QString groupname, QString newid, QString newname);
    void on_know_clicked();

private:
    Ui::newinGroup *ui;
};

#endif // NEWINGROUP_H
