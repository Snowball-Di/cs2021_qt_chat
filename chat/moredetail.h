#ifndef MOREDETAIL_H
#define MOREDETAIL_H

#include <QDialog>
#include "addnew.h"
#include "newgroup.h"
namespace Ui {
class Moredetail;
}

class Moredetail : public QDialog
{
    Q_OBJECT

public:
    explicit Moredetail(QWidget *parent = nullptr);
    ~Moredetail();


    void setUser(QString name, QString id, QPixmap pic);

private:
    Ui::Moredetail *ui;
    addNew *addfriend = new addNew(this, true);
    addNew *addgroup = new addNew(this, false);
    newGroup *newgroup = new newGroup(this);

private slots:
    void on_close_clicked();
    void on_addfriend_clicked();
    void on_addgroup_clicked();
    void on_newgroup_clicked();
};

#endif // MOREDETAIL_H
